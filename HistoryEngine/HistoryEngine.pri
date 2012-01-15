INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/HistoryEngine.h
SOURCES += $$PWD/HistoryEngine.cpp
CONFIG += mobility
MOBILITY += messaging
include(../../rsssuite/rsssuite.pri)

contains(MEEGO_EDITION,harmattan) {
    DEFINES += DC_HARMATTAN
    CONFIG += meegotouchevents
}
