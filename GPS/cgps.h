#ifndef CGPS_H
#define CGPS_H

#include <QObject>
#include <QThread>

class cGPS : public QThread
{
    Q_OBJECT
public:
    explicit cGPS(QObject *parent = 0);
    double getLongitude();
    double getLattitude();
    bool isCoordinateAvailable();
    int readGPSDate();
    int initGPS();
    void deinitGPS();
    int getStatus();
    int getSatellitesVisible();
    int getSatellitesUsed();
    struct tm  getGPSTime();

private:
    double m_Longitude;
    double m_Lattitude;
    bool m_CoordinateAvailable;
    int m_Status;
    int m_SatellitesVisible;
    int m_SatellitesUsed;
    struct tm mGPS_time;

protected:
    void run();
signals:
    void signalCoordinate(double, double);

public slots:
};


#endif // CGPS_H
