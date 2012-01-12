/*
  Author: Srikanth Sombhatla
  Developed for: Dreamcode
  July 2011
  srikanthsombhatla@gmail.com
*/

/*!
  \class HistoryEngine
  \brief Engine to manage history content.
**/
//#define APPEND_TEST_TITLES

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessage>
#include <QMessageService>
#include "HistoryEngine.h"
#include "rssmanager.h"
#include "rssparser.h"

QTM_USE_NAMESPACE

const QUrl KTDIHUrl("http://www.history.com/this-day-in-history/rss");
const QString KFavoritesFolder("favorites");

struct HistoryEnginePrivate {
    RSSManager* rssManager;
    HistoryInfo* historyInfo;
    QMessageService* messageService;
    bool invalidateFavList;
    bool started;
};


HistoryEngine::HistoryEngine(QObject *parent) :
               QObject(parent)
{
    d = new HistoryEnginePrivate;
    d->messageService = NULL;
    d->invalidateFavList = true;
    d->rssManager = new RSSManager(this);
    connect(d->rssManager,SIGNAL(updateAvailable(QUrl,int)),
    this,SLOT(handleUpdateAvailable(QUrl,int)));
    connect(d->rssManager,SIGNAL(error(QString,QUrl)),this,SIGNAL(error(QString,QUrl)));
}

HistoryEngine::~HistoryEngine() {
    delete d;
}

/*!
  Starts feed. Use this method to get updated in periodic intervals.
  Use update() to get a single shot update.
  **/
void HistoryEngine::start() {
    // add subscription
    d->started = d->rssManager->add(KTDIHUrl,60*1000*24);
    d->rssManager->updateAll();
}

QVariant HistoryEngine::historyInfo() const {
    return qVariantFromValue( (QObject*)d->historyInfo);
}

void HistoryEngine::handleUpdateAvailable(QUrl sourceUrl, int newItemsCount) {
    Q_UNUSED(newItemsCount)
    // Retrieve item
    RSSParser* parser = d->rssManager->parser(sourceUrl); // ownership is transfered
    d->historyInfo = parseInfo(parser);
    parser->deleteLater();
    emit updateReady(qVariantFromValue( (QObject*)d->historyInfo));
}

/*!
  Updates feed. This is a singleshot update.
  For periodic updates use start()
  **/
void HistoryEngine::update() {
    if(d->rssManager) {
#ifdef SCREENSAVER
        d->rssManager->start(KTDIHUrl);
#else
        if(!d->started)
            start();
        else
            d->rssManager->update(KTDIHUrl);
#endif
    }
}

QStringList HistoryEngine::favoriteTitles() {
    static QStringList favTitles;
    if(d->invalidateFavList || favTitles.isEmpty()) {
        favTitles.clear();
       QStringList favFiles = favFileList();
       RSSParser* parser = new RSSParser(this);
       for(int i=0;i<favFiles.count();++i) {
           QFile f(KFavoritesFolder+"/"+favFiles.at(i));
           if(f.open(QIODevice::ReadOnly)) {
               parser->setSource(&f);
               favTitles.append(RSSParser::decodeHtml(parser->itemElement(1,RSSParser::title)).trimmed());
           }
           f.close();
       }
       delete parser;
    }

#ifdef APPEND_TEST_TITLES
    qDebug()<<__PRETTY_FUNCTION__<<" appending test titles.";
    for(int i=0;i<15;i++)
        favTitles.append("Test title");
#endif
return favTitles;
}

QVariant HistoryEngine::favorites() {
    return qVariantFromValue(favoriteTitles());
}

//bool HistoryEngine::isFavorite(const HistoryInfo& info)
//{
//    QString fileToCheck = fileNameForKey(info.title);
//    return favFileList().contains(fileToCheck);
//}

bool HistoryEngine::saveAsFavorite() {
    // TODO: error handling is really confusing, fix it.
    bool error = false;
    QVariant v = historyInfo();
    HistoryInfo* info = qobject_cast<HistoryInfo*>(v.value<QObject*>());
    QDir dir;
    if(!dir.exists(KFavoritesFolder))
      error = !(dir.mkdir(KFavoritesFolder));

    if(!error) {
        // TODO: check if file exists.
        // copy existing file into fav folder with required name.
        QString target = KFavoritesFolder+"/"+fileNameForKey(info->title());
        QFileInfo fi(target);
        qDebug()<<Q_FUNC_INFO<<fi.absoluteFilePath();
        if(!QFile::exists(target))
            error = !QFile::copy(d->rssManager->feedFileName(KTDIHUrl),target);
    }

    if(!error)
        d->invalidateFavList = true;

return !error;
}

QObject* HistoryEngine::favorite(int favIndex) {
    HistoryInfo* info;
    QString favTitle = favoriteTitles().at(favIndex);
        QFile f(KFavoritesFolder+"/"+fileNameForKey(favTitle));
        if(f.open(QIODevice::ReadOnly)) {
            RSSParser* parser = new RSSParser(this);
            parser->setSource(&f);
            info = parseInfo(parser);
        }
        else {
            qWarning()<<__PRETTY_FUNCTION__<<" unable to open "<<f.fileName();
        }
return info;
}

QString HistoryEngine::fileNameForKey(const QString& key) {
    return (QString().setNum(qHash(key))+".xml");
}

QStringList HistoryEngine::favFileList() {
    static QStringList favList;
    if(d->invalidateFavList || favList.isEmpty()) { // TODO: fix cache invalidation
        favList.clear();
        QDir d;
        d.setPath(KFavoritesFolder);
        if(d.exists()) {
           favList = d.entryList(QStringList(),QDir::Files,QDir::Time);
        }
    }
    favList.removeDuplicates();
    d->invalidateFavList = favList.isEmpty();
    return favList;
}

HistoryInfo* HistoryEngine::parseInfo(RSSParser* parser) {
    HistoryInfo* info = new HistoryInfo(this);
    info->mTitle = RSSParser::decodeHtml(parser->itemElement(0,RSSParser::title)).trimmed();
    info->mDescription = RSSParser::decodeHtml(parser->itemElement(0,RSSParser::description)).trimmed();
    info->mLink = parser->itemElement(0,RSSParser::link);
    info->mEventDate = parser->itemElement(0,"eventDate").trimmed();
    return info;
}

int HistoryEngine::favoritesCount() {
    return favoriteTitles().count();
}

bool HistoryEngine::deleteFavorite(int index) {
    QString favToDel = favFileList().at(index);
    QDir dir;
    dir.setPath(KFavoritesFolder);
    d->invalidateFavList = true;
    return dir.remove(favToDel);
}

bool HistoryEngine::deleteAllFavorites() {
    bool success = true;
    int count = favoritesCount();
    for (int index=count-1;index>=0;--index)
        if(!deleteFavorite(index))
            success = false;
    return success;
}

/*!
  index -1 represents today info.
**/
bool HistoryEngine::share(int index) {
    QString title;
    bool result = false;
    if(-1 == index) // today
        title = d->historyInfo->title();
    else
        title = favoriteTitles().at(index);

    if(!title.isEmpty()) {
        QMessage m;
        m.setBody("This day in history:"+title+"."); // TODO: attach ovi app link
        m.setType(QMessage::Sms);
        if(!d->messageService)
            d->messageService = new QMessageService(this);
        result =  d->messageService->compose(m);
    }
return result;
}

// eof
