import QtQuick 1.1
import com.nokia.meego 1.0
import "HistoryConstants.js" as HistoryConstants;

Page {
    id: today;
    property string pageId;
    property bool errorInFetchingData;
    orientationLock: PageOrientation.LockPortrait;
    anchors.fill: parent;
    tools: ToolBarLayout {
            id: toolBarlayout
            Component.onCompleted: {
                //createBackToolButton(toolBarlayout); // no quit in harmattan
                createSaveAsFavToolButton(toolBarlayout);
                createShareToolButton(toolBarlayout);
                createMenuToolButton(toolBarlayout);
            }
        }

    BorderImage {
        id: name
        source: skin.bkgImage;
        anchors.fill: parent;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }

    Loader {
      id: infoLoader;
      anchors.fill: parent;
    }

    Text {
        id: errorText;
        anchors.centerIn: parent;
        wrapMode: Text.WordWrap;
        width: parent.width - 25;
        smooth: true;
        color: skin.fontColor;
        font.pixelSize: skin.contentFontSize;
    }

    Component.onCompleted: {
        pageId = HistoryConstants.todayPageId;
        var info = engine.historyInfo();
        infoLoader.source = "HistoryInfo.qml";
        infoLoader.item.viewTitle = HistoryConstants.todayTitleText;

        if(errorInFetchingData) {
            var err = HistoryConstants.netErrorText;
            if(engine.favoritesCount())
                err = err + "\n" + HistoryConstants.enjoyFavs;

            errorText.text = err;
        } else {
            infoLoader.item.info = engine.historyInfo();
        }
    }

} //page

