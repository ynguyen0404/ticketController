#include "cmqtt.h"
#include <QMutex>
#include <QTimer>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

cMQTT *cMQTT::m_Instance = nullptr;

cMQTT::cMQTT(QObject *parent) : QObject(parent)
{

    m_client = new QMqttClient(parent);
    m_ParseConfigureFile = new cParseConfigFile();
    m_connectInfo = m_ParseConfigureFile->getConfigurationData();

    m_client->setHostname("test.mosquitto.org");
    m_client->setPort(1883);
    m_client->setKeepAlive(40);

    connect(m_client, &QMqttClient::messageReceived, this, &cMQTT::on_ReceivedMessage);
    connect(m_client, &QMqttClient::disconnected, this, &cMQTT::on_ConnectionDisconnected);
    connect(m_client, &QMqttClient::connected, this, &cMQTT::on_ConnectionConnected);
}

cMQTT *cMQTT::instance(QObject *parent)
{
    static QMutex mutex;
    if (m_Instance == nullptr) {
        mutex.lock();
        m_Instance = new cMQTT(parent);
        mutex.unlock();
    }
    return m_Instance;
}

void cMQTT::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = nullptr;
    mutex.unlock();
}

void cMQTT::connectToServer()
{
    if (m_client != nullptr) {
        QTimer::singleShot(700, m_client, SLOT(connectToHost()));
    }
}

void cMQTT::disconnectToServer()
{
    if (m_client != nullptr)
    {
       m_client->disconnectFromHost();
    }
}

bool cMQTT::isConnected()
{
    bool retVal = false;
    if (m_client->state() == QMqttClient::Connected)
        retVal = true;
    return retVal;
}


QMqttClient::ClientState cMQTT::getstate()
{
    return m_client->state();
}

void cMQTT::on_ConnectionDisconnected()
{
    qDebug() << "Discon From Server";
    emit sigDisconnectedFromServer();
}

void cMQTT::on_ReceivedMessage(const QByteArray &message, const QMqttTopicName &topic)
{
    const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" Received Topic: ")
                + topic.name()
                + QLatin1String(" Message: ")
                + message;

    qDebug() << "Received Message From Server: " << content;
    QJsonDocument dataDoc = QJsonDocument::fromJson(message);
    QJsonObject dataObj = dataDoc.object();
    QByteArray rawData = QByteArray::fromBase64(dataObj.take("data").toString().toUtf8());
    QJsonArray rxinfo = dataObj.take("rxInfo").toArray();
    quint32 gwuid = 0;
    bool ok = false;
    if (rxinfo.count() > 0) {
        QString gwUID = rxinfo.at(0).toObject().value("gatewayID").toString();
        qDebug() << "Original Gateway UID: " << gwUID;
        gwuid = gwUID.rightRef(7).toInt(&ok, 16);
        qDebug() << "Short Version of GW UID: " << gwuid << " Converted Status :" << ok;
    }

    qDebug() << "Data From Node: " << rawData;

    emit sigDataToVuServer(rawData, gwuid);
}

void cMQTT::on_ConnectionConnected()
{
    QStringList m_AppID = m_connectInfo.getAppID();
    QStringList m_nodeID = m_connectInfo.getNodeIDs();
    QStringList subscribeTopics;
    subscribeTopics << "terminal/register";
    subscribeTopics << "terminal/carduid";
    foreach (QString topic, subscribeTopics) {
        auto subscription = m_client->subscribe(QMqttTopicFilter(topic), m_connectInfo.getQoS());
        qDebug() << "Chirpstack:" << "Subscription Response Code: " << subscription;
        if (!subscription) {
            qDebug("Could not subscribe. Is there a valid connection?");
            m_client->disconnectFromHost();
            m_isConnected = false;
            emit sigDisconnectedFromServer();
            break;
        } else {
            m_isConnected = true;
        }
    }
    emit sigConnectedToServer();
}

void cMQTT::receivedMessage(QByteArray &msg){
    QByteArray mess = "{\n\"uid\": \"4a27b5f1fc6a4fb6988c0cff5ac0f37b\",\n\"ip\": \"192.168.0.1\"\n}\n";
    msg = mess;
    ReceivedState = true;
    qDebug() << "msg recieve: " << msg;

}
