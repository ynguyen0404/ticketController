import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
//import "Global.js" as GlobalJS

ApplicationWindow {
    id: window
    width: 1024
    height: 600
    visible: true
    title: qsTr("TicketController")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight
        RowLayout
        {
            anchors.fill: parent
            ToolButton {
                id: toolButton
                text: stackView.depth > 1 ? "\u25C0" : "\u2630"
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                    } else {
                        drawer.open()
                    }
                }
            }
            Label {
                id: lblTitle
                text: stackView.currentItem.title
                anchors.centerIn: parent
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            Label {
                id: labelIP
                text: "IP Address: "
                objectName: "ipAddress"
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignRight

            }
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Get Location")
                width: parent.width
                onClicked: {
//                    stackView.push("Page1Form.ui.qml")HomeForm.ui.qml
                    stackView.push("HomeForm.qml")
                    drawer.close()
                }
            }
    }
}

    StackView {
        id: stackView
        initialItem: "Page1Form.qml"
        anchors.fill: parent
    }

//invoke method
//    function myQmlFunc(msg){
//    //        console.log("Got mess: ", msg)
//        lblTitle.text = msg
//        return "some return value"
//    }

    function qmlCard(msg)
    {
       //GlobalJS.sharedVariable = msg;
        textReceive.text = msg;
    }

//    Text {
//        id: textElement
//        anchors.centerIn: parent
//        text: "defaultParameter"
//    }

//        Connections {
//                target: myobj
//                sigReceivedData: {
//                    lblTitle.text = msg // Set the counter to a text label
//                }
//        }


}
