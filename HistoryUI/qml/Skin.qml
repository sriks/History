import QtQuick 1.0
import com.nokia.meego 1.0

    Item {
        id: auroraSkin
        property string bkgImage: "qrc:/images/bkg.jpg";//"qrc:/images/bkg_h4.jpg";
        property string menuIcon: "qrc:/images/tb_menu.svg";
        property string homeIcon: "qrc:/images/tb_home.svg";
        property string favIcon: "qrc:/images/tb_favorite.svg";
        property string backIcon: "qrc:/images/tb_back.svg"
        property string closeIcon: "qrc:/images/tb_close.svg";
        property string deleteIcon: "qrc:/images/tb_delete.svg";
        property string shareIcon: "qrc:/images/tb_share5.svg"
        property color fontColor: "#661100"
        property int titleFontSize: 41;
        property int subTitleFontSize: 31;
        property int contentFontSize: 25;
        property int contentMargin: 8;
    }
