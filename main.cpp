#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QQmlComponent>
#include <QDateTime>
#include <QQmlContext>
#include <QQuickView>
#include <QTimer>
#include <QTextCodec>
#include "GPS/cgps.h"
#include "GPS/cposition.h"
#include "MQTT/cmqtt.h"
#include <QJsonDocument>
#include <QJsonObject>

QString GetIPDevice(){
    QString IPaddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            qDebug() << address.toString();
            IPaddress = address.toString();
        }
    }
    return IPaddress;
};
QString GetDateTime(){
    QString datetime = QDateTime::currentDateTime().toString();
    return datetime;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QObject *object = component.create();


    cMQTT* mqtt = cMQTT::instance(&app);
    mqtt->connectToServer();

    //GetIpDevice + Datetime
    QString ip_Datetime = GetDateTime() + "    IP: " + GetIPDevice();
    QObject* ipAdd = object->findChild<QObject*>("ipAddress");
    if(ipAdd)
    {
        ipAdd->setProperty("text", ip_Datetime);
    }



        cMQTT cmqtt;
        QByteArray msg;
        cmqtt.receivedMessage(msg);

        QString strmsg = QTextCodec::codecForMib(106)->toUnicode(msg);
        qDebug() << "String conver from Qtbytearray: " << strmsg;
        QJsonDocument doc = QJsonDocument::fromJson(strmsg.toUtf8());
        QJsonObject jObject = doc.object();
        QString uid = jObject.value("uid").toString();
        qDebug() << "UID: " << uid;
        QString ip = jObject.value("ip").toString();
        qDebug() << "IP: " << ip;

    int NodeConnected = 0;
    QObject* rpt = object->findChild<QObject*>("Repeater");
    QObject* card = object->findChild<QObject*>("infoCardValidate");
    if(cmqtt.ReceivedState == true)
    {
        NodeConnected = 5;
        if(rpt)
        {
            rpt->setProperty("model",NodeConnected);
        }

        if(card)
        {
            card->setProperty("text", "uid: " + uid + "\n" + "ip: " + ip);
        }
        cmqtt.ReceivedState = false;
    }

//    QVariant name = uid + ip;
//    QMetaObject::invokeMethod(object, "qmlCard",Q_ARG(QVariant, name));


    //Start GPS
    QThread threadPos;
    ccPosition cposition;
    cGPS cgps;
    cgps.initGPS();

    QObject::connect(&cgps, &cGPS::signalCoordinate, &cposition, &ccPosition::coordinateReceive);
    QObject::connect(&threadPos, &QThread::started, &cgps, &cGPS::readGPSDate);
    cgps.moveToThread(&threadPos);
    threadPos.start();

    QQmlContext* context = engine.rootContext();
    context->setContextProperty("planner", &cposition);
    QTimer timer;
    timer.setInterval(1000);
    QObject::connect(&timer, &QTimer::timeout, &cposition, &ccPosition::triggerPosition);
    timer.start();

    //delete object;
    return app.exec();
}
