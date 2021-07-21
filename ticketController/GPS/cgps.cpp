#include "cgps.h"
#include <QDebug>
#include <QMutex>
#include <cmath>
#include <gps.h>

#include <stdio.h>
#include <stdlib.h>
#include <QThread>

struct gps_data_t cgpsdata;

cGPS::cGPS(QObject *parent) : QThread(parent){

}

double cGPS::getLongitude()
{
    return m_Longitude;
}

double cGPS::getLattitude()
{
    return m_Lattitude;
}

int cGPS::getStatus()
{
    return m_Status;
}

int cGPS::getSatellitesVisible()
{
    return m_SatellitesVisible;
}

int cGPS::getSatellitesUsed()
{
    return m_SatellitesUsed;
}

struct tm  cGPS::getGPSTime()
{
    return mGPS_time;
}

bool cGPS::isCoordinateAvailable()
{
    return m_CoordinateAvailable;
}

int cGPS::readGPSDate()
{
    int rc = -1;
    char msg[] = "\0";
    int msg_len = 1;
    while(1)
    {
        if (gps_waiting (&cgpsdata, 2000000)) {
                /* read data */
                qDebug() << "GPS Fix Mode: " << cgpsdata.fix.mode;
                if ((rc = gps_read(&cgpsdata,msg, msg_len)) == -1) {
                    qDebug() << "Error occured reading gps data. Reason: "<< gps_errstr(rc);
                    qDebug() << "rc = -1";
                } else {
                    /* Display data from the GPS receiver. */
                    if ((cgpsdata.status == STATUS_FIX) &&
                        (cgpsdata.fix.mode == MODE_2D || cgpsdata.fix.mode == MODE_3D) &&
                        !std::isnan(cgpsdata.fix.latitude) &&
                        !std::isnan(cgpsdata.fix.longitude)) {
                            //gettimeofday(&tv, NULL); //EDIT: tv.tv_sec isn't actually the timestamp!
                            //fprintf(stdout,"GPS Status: %i, latitude: %f, longitude: %f, speed: %f, timestamp: %lf\n", cgpsdata.status, cgpsdata.fix.latitude, cgpsdata.fix.longitude, cgpsdata.fix.speed, cgpsdata.fix.time); //EDIT: Replaced tv.tv_sec with gps_data.fix.time
                            m_Lattitude = cgpsdata.fix.latitude;
                            m_Longitude = cgpsdata.fix.longitude;

                            emit cGPS::signalCoordinate(m_Lattitude, m_Longitude);

                            //qDebug() << "m_Lattitude: " << m_Lattitude;
                            //qDebug() << "m_Longitude: " << m_Longitude;
                            qDebug() << "GPS Status: " << cgpsdata.status << "        " << "Latitude: " << cgpsdata.fix.latitude \
                                     << "        " << "Longitude: " << cgpsdata.fix.longitude \
                                     << "        " << "Speed: " << cgpsdata.fix.speed;

                    } else {
                        qDebug() << "no GPS data available";
                    }
                }
            }
        msleep(500);
    }

    gps_stream(&cgpsdata, WATCH_DISABLE, NULL);
    gps_close (&cgpsdata);
    qDebug() << "Close gpsthread!";
     return rc;
}

int cGPS::initGPS()
{
    int rc = 0;
    int retry = 3;

_begin:
    qDebug() << "Info: GPS port: " << DEFAULT_GPSD_PORT;
    rc = gps_open("localhost", DEFAULT_GPSD_PORT, &cgpsdata);
    if (rc)
    {
        if(retry > 0)
        {
            retry--;
            system("/usr/sbin/gpsd -n -N -G /dev/ttyUSB0");
            goto _begin;
        }
        qDebug() << "Error: Cannot open GPS port: " << DEFAULT_GPSD_PORT;
        return -1;
    }
    if ((rc = gps_open("localhost", "2947", &cgpsdata)) == -1) {
        qDebug() << "code: " << rc << ", " << "reason: " << gps_errstr(rc);
        return EXIT_FAILURE;
    }

    gps_stream(&cgpsdata, WATCH_ENABLE | WATCH_JSON, NULL);
    return 0;
}

void cGPS::deinitGPS()
{
    gps_stream(&cgpsdata, WATCH_DISABLE, NULL);
    gps_close(&cgpsdata);
}

void cGPS::run(){
    cGPS::readGPSDate();
}
