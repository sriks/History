# History Engine

INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/HistoryEngine.h
SOURCES += $$PWD/HistoryEngine.cpp
include(../../feedparrot/feedparrot.pri)

symbian {
    # On Symbian only sms sharing.
    CONFIG += mobility
    MOBILITY += messaging
}

contains(MEEGO_EDITION,harmattan) {
    DEFINES += DC_HARMATTAN
    CONFIG += meegotouchevents
    # Using Harmattan provided share ui.
    include(../../shareui/shareui.pri)
}
