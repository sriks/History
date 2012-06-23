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
#include <QDateTime>
#ifdef DC_HARMATTAN
#include <meventfeed.h>
#include "ShareUi.h"
#endif

#include "HistoryEngine.h"
#include "rssmanager.h"
#include "rssparser.h"

#ifdef Q_OS_SYMBIAN
#include <QMessage>
#include <QMessageService>
QTM_USE_NAMESPACE
#endif

const QUrl KTDIHUrl("http://www.history.com/this-day-in-history/rss");
const QString KFAVORITES_FOLDER_NAME("favorites");

struct HistoryEnginePrivate {
    RSSManager* rssManager;
    HistoryInfo* historyInfo;
#ifdef Q_OS_SYMBIAN
    QMessageService* messageService;
#endif
    bool invalidateFavList;
    bool started;
};


HistoryEngine::HistoryEngine(QObject *parent) :
               QObject(parent)
{
    d = new HistoryEnginePrivate;
#ifdef Q_OS_SYMBIAN
    d->messageService = NULL;
#endif
    d->historyInfo = NULL;
    d->invalidateFavList = true;
    d->rssManager = new RSSManager("tdih",this);
    connect(d->rssManager,SIGNAL(updateAvailable(QUrl,int)),
            this,SLOT(handleUpdateAvailable(QUrl,int)));
    connect(d->rssManager,SIGNAL(error(QString,QUrl)),this,SIGNAL(error(QString,QUrl)));
}

HistoryEngine::~HistoryEngine() {
    d->rssManager->removeAll();
    delete d;
}

/*!
  Starts feed. Use this method to get updated in periodic intervals.
  Use update(QUrl) to get a single shot update.
  **/
void HistoryEngine::start() {
    // add subscription
    d->started = d->rssManager->add(KTDIHUrl,60*1000*12); // 12 hr interval
    d->rssManager->setUpdateOnNewItemsOnly(KTDIHUrl,false);
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
#ifdef DC_HARMATTAN
    updateEventFeed(qVariantFromValue( (QObject*)d->historyInfo));
#endif
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
           QFile f(this->favoritesPath()+"/"+favFiles.at(i));
           if(f.open(QIODevice::ReadOnly)) {
               parser->setSource(&f);
               favTitles.append(RSSParser::decodeHtml(parser->itemElement(0,RSSParser::title)).trimmed());
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

bool HistoryEngine::saveAsFavorite() {
    // TODO: error handling is really confusing, fix it.
    bool error = false;
    HistoryInfo* info = d->historyInfo;
    if(info && info->isValid()) {
        QDir dir(d->rssManager->storagePath());
        if(!dir.exists(KFAVORITES_FOLDER_NAME))
          error = !(dir.mkdir(KFAVORITES_FOLDER_NAME));

        if(!error) {
            // TODO: check if file exists.
            // copy existing file into fav folder with required name.
            QString target = this->favoritesPath()+"/"+fileNameForKey(info->title());
            QFileInfo fi(target);
            qDebug()<<Q_FUNC_INFO<<fi.absoluteFilePath();
            if(!QFile::exists(target))
                error = !QFile::copy(d->rssManager->feedFileName(KTDIHUrl),target);
        } else {
            qWarning()<<Q_FUNC_INFO<<"Unable to create favorites folder at "<<dir.absolutePath();
        }

        if(!error)
            d->invalidateFavList = true;
    } else {
        error = true;
    }
return !error;
}

QObject* HistoryEngine::favorite(int favIndex) {
    HistoryInfo* info = NULL;
    QString favTitle = favoriteTitles().at(favIndex);
        QFile f(this->favoritesPath()+"/"+fileNameForKey(favTitle));
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
        d.setPath(this->favoritesPath());
        if(d.exists()) {
           favList = d.entryList(QStringList(),QDir::Files,QDir::Time);
        }
    }
    favList.removeDuplicates();
    d->invalidateFavList = favList.isEmpty();
    return favList;
}

/*!
  \brief Returns favirites folder path according to the target OS.
  **/
QString HistoryEngine::favoritesPath() const {
    //In Symbian this returns private folder and using this should work.
    QString favPath = d->rssManager->storagePath();
    favPath.append("/"+KFAVORITES_FOLDER_NAME);
    return favPath;
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
    dir.setPath(this->favoritesPath());
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
        title = (d->historyInfo)?(d->historyInfo->title()):(QString());
    else
        title = favoriteTitles().at(index);

    if(!title.isEmpty()) {
#ifdef DC_HARMATTAN
        QString des("This day in history:"+title+".");
        ShareUi s;
        result = s.share(QString(),des,QString());
#endif

#ifdef Q_OS_SYMBIAN
        QMessage m;
        m.setBody("This day in history:"+title+"."); // TODO: attach ovi app link
        m.setType(QMessage::Sms);
        if(!d->messageService)
            d->messageService = new QMessageService(this);
        result =  d->messageService->compose(m);
#endif
    }
    return result;
}

#ifdef DC_HARMATTAN
void HistoryEngine::updateEventFeed(QVariant update) {
    HistoryInfo* info = qobject_cast<HistoryInfo*>(update.value<QObject*>());
    if(info && !info->title().isEmpty()) {
        QString title = info->title();
        MEventFeed::instance()->removeItemsBySourceName("historysync");
        MEventFeed::instance()->addItem(QString("/usr/share/icons/hicolor/80x80/apps/HistoryUI80.png"),
                              QString("This day in history"),
                              title,
                              QStringList(),
                              QDateTime::currentDateTime(),
                              QString(),
                              false,
                              QUrl("tdih://today"),
                              QString("historysync"),
                              QString("This day in history"));
     }
}
#endif
// eof
