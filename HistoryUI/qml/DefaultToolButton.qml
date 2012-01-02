import QtQuick 1.0
import com.nokia.meego 1.0

/*ToolButton*/ ToolIcon {
    id: defaultToolButton;
    property string toolId;
    property Item actionHandler;
//    flat: true
    onClicked: actionHandler.handleAction(toolId);
}
