#-------------------------------------------------
#
# Project created by QtCreator 2011-12-27T17:46:06
#
#-------------------------------------------------

QT       -= gui

TARGET = historyengine
TEMPLATE = lib

DEFINES += SYNCTESTDATASUPPLIER_LIBRARY

INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/HistoryEngine.h
SOURCES += $$PWD/HistoryEngine.cpp
include(../../rsssuite/rssmanager/rssmanager.pri)

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE25F340F
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SyncTestDataSupplier.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

#contains(MEEGO_EDITION,harmattan) {
#    CONFIG += plugin meegotouchevents #debug
#    #install
#    target.path = /usr/lib/sync/

#    client.path = /etc/sync/profiles/client
#    client.files = xml/example.xml

#    sync.path = /etc/sync/profiles/sync
#    sync.files = xml/sync/*

#    service.path = /etc/sync/profiles/service
#    service.files = xml/service/*

#    settingsdesktop.path = /usr/share/duicontrolpanel/desktops
#    settingsdesktop.files = settings/feedexample.desktop

#    settingsxml.path = /usr/share/duicontrolpanel/uidescriptions
#    settingsxml.files = settings/feedexample.xml

#    INSTALLS += target client sync service settingsdesktop settingsxml
#}
