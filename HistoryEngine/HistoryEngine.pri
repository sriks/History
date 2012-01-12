INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/HistoryEngine.h
SOURCES += $$PWD/HistoryEngine.cpp
CONFIG += mobility
MOBILITY += messaging
#include(../../rsssuite/rssmanager/rssmanager.pri)
include(../../rsssuite/rsssuite.pri)
