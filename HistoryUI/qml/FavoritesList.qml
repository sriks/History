import QtQuick 1.1
import com.nokia.meego 1.0
import "HistoryConstants.js" as HistoryConstants;

Page {
    id: favoritesList;
    property Item deleteAllToolButton;
    property string pageId;
    property variant favModel;
    orientationLock: PageOrientation.LockPortrait;
    tools: ToolBarLayout {
            id: toolBarlayout
            Component.onCompleted: {
                createBackToolButton(toolBarlayout);
                if(engine.favoritesCount())
                    deleteAllToolButton = createDeleteAllFavsToolButton(toolBarlayout);
                createMenuToolButton(toolBarlayout);
            }
        }

    BorderImage {
        id: bkg
        source: skin.bkgImage;
        anchors.fill: parent;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }

    Text {
        id: emptyText
        anchors.centerIn: parent;
        text: HistoryConstants.favsEmptyText
        font.pixelSize: skin.subTitleFontSize;
        visible: false;
    }

    ListView {
        id: favList
        spacing: 10;
        anchors.fill: parent
        model: 1
        delegate: favListView

        Component {
            id: favListView;
            Column {
                id: favListColumn
                width: parent.width;
            Text {
                id: title;
                text: HistoryConstants.myFavoritesText;
                font.pixelSize: skin.titleFontSize;
                color: skin.fontColor;
                wrapMode: Text.WordWrap;
                horizontalAlignment: Text.AlignHCenter;
                width: parent.width;
                anchors.margins: skin.contentMargin;
                }

            Repeater {
                id: listItemRepeater
                model: favModel;
                delegate: listItem;
            }

            Component {
                id: listItem
            Item {
                width: parent.width;
                height: favItemText.height + 25;

                Rectangle {
                    width: parent.width-10;
                    height: 2;
                    border.color: "white";
                    anchors.horizontalCenter: parent.horizontalCenter;
                }

                Text {
                    id: favItemText;
                    text: modelData
                    width: parent.width - 10;
                    wrapMode: Text.WordWrap;
                    color: skin.fontColor;
                    font.pixelSize: skin.contentFontSize;
                    anchors.margins: 5
                    anchors.centerIn: parent;
                }

                Rectangle {
                    width: parent.width-10;
                    height: 2;
                    border.color: "white";
                    anchors.horizontalCenter: parent.horizontalCenter;

                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: loadFavorite(index);
                    }
            } }

            } // column
        }// component
    }// listview

    onStatusChanged: {
        if(PageStatus.Active == status)
            updatePage();
    }

    Component.onCompleted: {
        pageId = HistoryConstants.favListPageId;
    }

    function updatePage() {
        favModel = engine.favorites();
        if(!engine.favoritesCount()) {
            emptyText.visible = true;
            deleteAllToolButton.visible = false;
        }
        else {
            emptyText.visible = false;
            deleteAllToolButton.visible = true;
        }
    }
}

// eof
