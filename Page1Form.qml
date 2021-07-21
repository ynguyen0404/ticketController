import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
//import "Global.js" as GlobalJS

Page {
    id: pagee
    width: 1024
    height: 600
    title: qsTr("Validate Card Information")

    property int modelItem: 1
        ScrollView
        {
            width: 500
            height: 500
            anchors.centerIn: parent
            anchors.fill: parent.Center
            Column {

//                Repeater {
//                    id: rpt
//                    objectName: "Repeater"
//                    model: modelItem
                    Rectangle {
                            id: rtg
                            width: 450
                            height: 250
                            color: "white"
                            border.color: "green"
                            border.width: 3
                            radius: 10
                                    TextArea {
                                        id: textReceive
                                        objectName: "infoCardValidate"
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.centerIn: parent
                                        text: "Card Validate Info: "
//                                        text: GlobalJS.sharedVariable
                                        readOnly: true
                                    }
                    }
                }
            }
//        }
}

