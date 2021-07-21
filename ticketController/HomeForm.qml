import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import QtLocation 5.5
import QtPositioning 5.5

Page {
    width: 1024
    height: 600

    title: qsTr("Location")

    Label {
        text: qsTr("You are on the home page.")
        anchors.centerIn: parent
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map{
        id: map
        anchors.fill:parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(10.762622,106.660172)
        zoomLevel:10

        MapItemView{
            model: planner.uavModel
            delegate: MapQuickItem {
                id:uavPos
                coordinate: QtPositioning.coordinate(model.position.latitude, model.position.longitude)
                sourceItem: Image
                      {
                         id: image
                         height: 35
                         width: 35
                         source: "qrc:/geotag.png"
                      }
            }
        }

    }

}
