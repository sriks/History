# Add more folders to ship with the application, here
folder_01.source = qml
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0x200478B8

# Allow network access on Symbian
symbian {
TARGET.CAPABILITY += LocalServices ReadUserData WriteUserData NetworkServices UserEnvironment ReadDeviceData WriteDeviceData
symbian:ICON = historyappicon.svg
DEPLOYMENT.installer_header=0xA000D7CE
vendorinfo = "%{\"Dreamcode\"}" ":\"Dreamcode\""

my_deployment.pkg_prerules = vendorinfo
DEPLOYMENT += my_deployment
}

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the
# MOBILITY variable.
#CONFIG += mobility
#MOBILITY += messaging

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc

include(../HistoryEngine/HistoryEngine.pri)

OTHER_FILES += qml/main.qml \
    qml/HistoryInfo.qml \
    qml/Today.qml \
    qml/Skin.qml \
    qml/FavoritesList.qml \
    qml/Favorite.qml \
    qml/DefaultToolButton.qml \
    qml/HistoryConstants.js \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

symbian: {
    OTHER_FILES += qml/symbian/Banner.qml
}
contains(MEEGO_EDITION,harmattan) {
    OTHER_FILES += qml/harmattan/Banner.qml
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog


contains(MEEGO_EDITION,harmattan) {
    SUBDIRS += HistorySync

    icon.files = HistoryUI80.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon

    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile

    harmattandesktopfile.files = $${TARGET}_harmattan.desktop
    harmattandesktopfile.path = /usr/share/applications
    INSTALLS += harmattandesktopfile
}
