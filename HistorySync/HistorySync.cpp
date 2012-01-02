#include <QDebug>
//#include <LogMacros.h>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QUrl>
#include <meventfeed.h>
#include "HistorySync.h"
#include "HistoryEngine.h"

class HistorySyncPrivate {
public:
    HistorySyncPrivate() {
        engine = 0;
    }
    ~HistorySyncPrivate() {
        delete engine;
    }
    HistoryEngine* engine;
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
    QTimer::singleShot(500,d->engine,SLOT(start()));
//    QTimer::singleShot(10000,this,SLOT(test()));
    return true;


//    if(d->update())
//        qDebug()<<Q_FUNC_INFO<<"Updated event feed.";
//    else
//        qDebug()<<Q_FUNC_INFO<<"Updated event feed FAILED.";

////    updateResults(Buteo::SyncResults(QDateTime::currentDateTime(),
////                  Buteo::SyncResults::SYNC_RESULT_SUCCESS,
////                  Buteo::SyncResults::NO_ERROR));
//    //Notify Sync FW of result - Now sync fw will call uninit and then will unload plugin
//    emit success(getProfileName(), "Success!!");
//    return true;
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
    HistoryInfo* info = qobject_cast<HistoryInfo*>(update.value<QObject*>());
    QString title = "Unable to update";
    qlonglong id = -1;
    MEventFeed::instance()->removeItemsBySourceName("historysync");
    if(info && !info->title().isEmpty()) {
        title = info->title();
        id = MEventFeed::instance()->addItem(QString("icon-m-transfer-sync"),
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
}

// eof
