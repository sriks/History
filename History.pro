TEMPLATE = subdirs

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/postinst \
    qtc_packaging/debian_harmattan/prerm

SUBDIRS += \
    HistoryUI

contains(MEEGO_EDITION,harmattan) {
    SUBDIRS += HistorySync # event feed integration
}







