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

//#define LOG_TO_FILE

class HistorySyncPrivate {
public:
    HistorySyncPrivate() {
        engine = 0;
#ifdef LOG_TO_FILE
        f = new QFile("/home/user/historysync.log");
        if(f->open(QIODevice::Append))
            writeLog(QString("New Log Starts: ") + QDateTime::currentDateTime().toString());
#endif
    }
    ~HistorySyncPrivate() {
        delete engine;
#ifdef LOG_TO_FILE
        f->close();
        delete f;
#endif
    }

    void writeLog(QString msg) {
#ifdef LOG_TO_FILE
        if(f->isOpen())
            f->write(msg.toAscii());
#endif
    }

    HistoryEngine* engine;

#ifdef LOG_TO_FILE
    QFile* f;
#endif
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
    QTimer::singleShot(100,d->engine,SLOT(start()));
    return true;
}

bool HistorySync::init() {
    if(!d->engine) {
        d->engine = new HistoryEngine;
        QObject::connect(d->engine,SIGNAL(updateReady(QVariant)),
                         this,SLOT(updateReady(QVariant)),Qt::UniqueConnection);
    }
    return true;
}

bool HistorySync::uninit() {
    return true;
}

void HistorySync::connectivityStateChanged(Sync::ConnectivityType aType, bool aState) {}

void HistorySync::updateReady(QVariant update) {
    d->writeLog(QString("HistorySync::updateReady"));
    HistoryInfo* info = qobject_cast<HistoryInfo*>(update.value<QObject*>());
    QString title;
    qlonglong id = -1;
    MEventFeed::instance()->removeItemsBySourceName("historysync");
    if(info && !info->title().isEmpty()) {
        title = info->title();
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
