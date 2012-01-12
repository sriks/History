#-------------------------------------------------
#
# Project created by QtCreator 2011-12-28T17:54:44
#
#-------------------------------------------------

TARGET = historysync-client
TEMPLATE = lib
DEFINES += HISTORYSYNC_LIBRARY

DEPENDPATH += .
INCLUDEPATH += .  \
                 /usr/include/libsynccommon \
                 /usr/include/libsyncprofile
SOURCES += HistorySync.cpp
HEADERS += HistorySync.h\
        HistorySync_global.h
include(../HistoryEngine/HistoryEngine.pri)
INCLUDEPATH += ../HistoryEngine

LIBS += -lsyncpluginmgr -lsyncprofile
CONFIG += debug plugin meegotouchevents
QT += dbus network # why im adding network?
QT -= gui

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE9287F4D
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = HistorySync.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
OTHER_FILES += xml/*.xml
OTHER_FILES += xml/sync/*.xml
OTHER_FILES += xml/service/*.xml
#install
target.path = /usr/lib/sync/

client.path = /etc/sync/profiles/client
client.files = xml/historysync.xml

sync.path = /etc/sync/profiles/sync
sync.files = xml/sync/*

service.path = /etc/sync/profiles/service
service.files = xml/service/*

INSTALLS += target client sync service
}
