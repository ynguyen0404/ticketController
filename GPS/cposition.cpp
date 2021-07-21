#include "cposition.h"

ccPosition::ccPosition(QObject* parent) : QObject(parent), m_uavModel(new QStandardItemModel(this))
{
    m_uavModel->setItemRoleNames({{PositionRole, "position"}});
    for(int i =0; i < 4; i++){
            QStandardItem *item = new QStandardItem;
            item->setData(QVariant::fromValue(QGeoCoordinate()), PositionRole);
            m_uavModel->appendRow(item);
        }
}


QObject* ccPosition::uavModel() const {
    return m_uavModel;
}

void ccPosition::triggerPosition(){
    if(QStandardItem *item = m_uavModel->item(0)){
        QGeoCoordinate uav_id;
        uav_id.setLatitude(Lati);
        uav_id.setLongitude(Longi);
        uav_id.setAltitude(5);
        item->setData(QVariant::fromValue(uav_id), PositionRole);
    }
}

void ccPosition::coordinateReceive(double La, double Long)
{
    Lati = La;
    Longi = Long;
    //qDebug() << "Latitude receive: " << Lati;
    //qDebug() << "Longitude receice: " << Longi;
    emit sigupdateCoordinate(La,Long);
}
