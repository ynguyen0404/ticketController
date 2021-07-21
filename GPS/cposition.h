#ifndef CPOSITION_H
#define CPOSITION_H

#include <QObject>
#include <QDebug>
#include <GPS/cgps.h>
#include <QGeoCoordinate>
#include <QStandardItemModel>

class QStandardItemModel;
class QAbstractItemModel;

class ccPosition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* uavModel READ uavModel CONSTANT)
public:

    enum UAVRoles {
            PositionRole = Qt::UserRole + 1000
        };
    QObject *uavModel() const;

    explicit ccPosition(QObject *parent = 0);


private:
    QStandardItemModel* m_uavModel;
    double Lati = 0;
    double Longi = 0;

signals:
    void sigupdateCoordinate(double Latitude, double Longitude);

public slots:
    void coordinateReceive(double La, double Long);
    void triggerPosition();

};

#endif // CPOSITION_H
