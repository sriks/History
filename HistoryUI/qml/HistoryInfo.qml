import QtQuick 1.1

ListView {
    property QtObject info;
    property string viewTitle;
    id: historyInfo;
    model: 1;
    delegate: infoDelegate;
    Component {
        id: infoDelegate;

        Column {
            id: infoColumn
            spacing: 10;
            width: 480;
            Text {
                id: title;
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                text: viewTitle;
                font.pixelSize: skin.titleFontSize;
                color: skin.fontColor;
                wrapMode: Text.WordWrap;
                horizontalAlignment: Text.AlignHCenter;
                width: parent.width;
                anchors.margins: skin.contentMargin;
            }

            Text {
                id: subTitle;
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                text: info.title();
                font.pixelSize: skin.subTitleFontSize;
                color: skin.fontColor;
                wrapMode: Text.WordWrap;
                horizontalAlignment: Text.AlignHCenter;
                width: parent.width;
                anchors.margins: skin.contentMargin;
            }

            Text {
                id: description;
                text: info.description();
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                wrapMode: Text.WordWrap;
                //width: parent.width;
                width: parent.width;
                font.pixelSize: skin.contentFontSize
                color: skin.fontColor;
                anchors.margins: skin.contentMargin;

//                MouseArea {
//                    id: mouseArea;
//                    anchors.fill: parent;
//                    onDoubleClicked: {
//                        var fontSizeOffset = (description.font.pixelSize == skin.contentFontSize)?(1.5):(1);
//                        description.font.pixelSize = skin.contentFontSize*fontSizeOffset;
//                        }
//                    }
                }
        } // column
    }
}
