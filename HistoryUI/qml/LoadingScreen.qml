import QtQuick 1.0
import com.nokia.meego 1.0
import "HistoryConstants.js" as HistoryConstants;

Page {
    id: loadingScreen;
    property string pageId;
    orientationLock: PageOrientation.LockPortrait;

// Makes sense only for symbian
//    tools: ToolBarLayout {
//            id: toolBarlayout
//            Component.onCompleted:
//                createBackToolButton(toolBarlayout)
//    }

    BorderImage {
        id: name
        source: skin.bkgImage;
        anchors.fill: parent;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }


    Row {
        id: appnameContainer
        width: 200;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.verticalCenter: parent.verticalCenter;
        spacing: 0;

        Text {
            id: appname1;
            smooth: true;
            text: "HIST";
            font.pixelSize: 60;
        }

        BusyIndicator {
            id: busy;
            running: true;
            anchors.verticalCenter: parent.verticalCenter;
            width: 35;
        }

        Text {
            id: appname2;
            smooth: true;
            text: "RY";
            font.pixelSize: 60;
        }
    }

    Component.onCompleted: {
        pageId = HistoryConstants.loadingScreenPageId;
    }
}
