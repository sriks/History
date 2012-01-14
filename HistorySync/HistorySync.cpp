#include <QDebug>
//#include <LogMacros.h>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QUrl>
#include <QFile>
#include <meventfeed.h>
#include "HistorySync.h"
#include "HistoryEngine.h"

class HistorySyncPrivate {
public:
    HistorySyncPrivate() {
        engine = 0;
        f = new QFile("/home/user/historysync.log");
        if(f->open(QIODevice::Append))
            writeLog(QString("New Log Starts: ") + QDateTime::currentDateTime().toString());
    }
    ~HistorySyncPrivate() {
        delete engine;
        f->close();
        delete f;
    }
    void writeLog(QString msg) {
        if(f->isOpen())
            f->write(msg.toAscii());
    }

    HistoryEngine* engine;
    QFile* f;
};

HistorySync* createPlugin(const QString& aPluginName,
                          const Buteo::SyncProfile& aProfile,
                          Buteo::PluginCbInterface *aCbInterface) {
    return new HistorySync(aPluginName,aProfile,aCbInterface);
}

void destroyPlugin(HistorySync* aClient) {
    delete aClient;
}

HistorySync::HistorySync(const QString& aPluginName,
            const Buteo::SyncProfile& aProfile,
            Buteo::PluginCbInterface *aCbInterface) :
    Buteo::ClientPlugin(aPluginName,aProfile,aCbInterface) {
    d = new HistorySyncPrivate;

}

HistorySync::~HistorySync() {
    delete d;
}

bool HistorySync::startSync() {
    d->writeLog(QString("HistorySync::startSync()"));
    int id = MEventFeed::instance()->addItem(QString("/usr/share/icons/hicolor/80x80/apps/HistoryUI80.png"),
                          QString("This day in history"),
                                             QString("This is a test!"),
                          QStringList(),
                          QDateTime::currentDateTime(),
                          QString(),
                          false,
                          QUrl("tdih://today"),
                          QString("historysync"),
                          QString("This day in history"));

    d->writeLog(QString("test eventfeed id:")+QString().setNum(id));
    QTimer::singleShot(100,d->engine,SLOT(start()));
    return true;
}

bool HistorySync::init() {
    qDebug()<<Q_FUNC_INFO;
    if(!d->engine) {
        d->engine = new HistoryEngine;
        QObject::connect(d->engine,SIGNAL(updateReady(QVariant)),
                         this,SLOT(updateReady(QVariant)),Qt::UniqueConnection);
    }
    return true;
}

bool HistorySync::uninit() {
    qDebug()<<Q_FUNC_INFO;
    return true;
}

void HistorySync::connectivityStateChanged(Sync::ConnectivityType aType, bool aState) {
    qDebug()<<Q_FUNC_INFO;
}

void HistorySync::updateReady(QVariant update) {
    d->writeLog(QString("HistorySync::updateReady"));
    HistoryInfo* info = qobject_cast<HistoryInfo*>(update.value<QObject*>());
    QString title = "Unable to update";
    qlonglong id = -1;
    MEventFeed::instance()->removeItemsBySourceName("historysync");
    if(info && !info->title().isEmpty()) {
        title = info->title();
        // icon-m-transfer-sync
        id = MEventFeed::instance()->addItem(QString("/usr/share/icons/hicolor/80x80/apps/HistoryUI80.png"),
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

      if (id != -1)
          emit success(getProfileName(), "Success.");
      else
          emit error(getProfileName(), "Error.", Buteo::SyncResults::SYNC_RESULT_FAILED);
      d->writeLog(title);
      d->writeLog(QString("HistorySync::updateReady: ")+QString().setNum(id));
}

// eof
