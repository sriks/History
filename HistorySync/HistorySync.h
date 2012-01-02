#ifndef HISTORYSYNC_H
#define HISTORYSYNC_H

#include <libsyncpluginmgr/ClientPlugin.h>
#include <libsyncprofile/SyncResults.h>
#include <libsyncpluginmgr/PluginCbInterface.h>
#include <QVariant>
#include "HistorySync_global.h"

class HistorySyncPrivate;
class HISTORYSYNCSHARED_EXPORT HistorySync : public Buteo::ClientPlugin {
    Q_OBJECT
public:
    HistorySync(const QString& aPluginName,
                const Buteo::SyncProfile& aProfile,
                Buteo::PluginCbInterface *aCbInterface);
    virtual ~HistorySync();

    // From ClientPlugin
    virtual bool startSync();

    // From SyncPluginBase
    virtual bool init();
    virtual bool uninit();

public slots:
    // From SyncPluginBase
    virtual void connectivityStateChanged( Sync::ConnectivityType aType, bool aState );
    virtual void updateReady(QVariant update);
private:
    HistorySyncPrivate* d;
};

extern "C" HistorySync* createPlugin(const QString& aPluginName,
                                     const Buteo::SyncProfile& aProfile,
                                     Buteo::PluginCbInterface *aCbInterface);
extern "C" void destroyPlugin(HistorySync* aClient);
#endif // HISTORYSYNC_H
