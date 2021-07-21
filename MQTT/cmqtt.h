#ifndef CMQTT_H
#define CMQTT_H

#include <QObject>
#include <QThread>
#include <QMqttClient>
#include "cconnectinfo.h"
#include "cparseconfigfile.h"

class cMQTT : public QObject
{
    Q_OBJECT
public:
    explicit cMQTT(QObject *parent = nullptr);

    cMQTT (const cMQTT &);
    cMQTT & operator = (const cMQTT &);

    static cMQTT *instance(QObject *parent = nullptr);
    static void drop();

    void connectToServer();
    void disconnectToServer();
    bool isConnected();
    QMqttClient::ClientState getstate();

    bool ReceivedState = false;

private:
    QMqttClient *m_client = nullptr;

    static cMQTT *m_Instance;
    bool m_isConnected = false;
    cParseConfigFile *m_ParseConfigureFile = nullptr;
    cConnectInfo m_connectInfo;

signals:
    void sigConnectedToServer();
    void sigDisconnectedFromServer();
    void sigDataToVuServer(QByteArray data, qint32 gwuid);

public slots:
    //void on_ConnectionStateChange();
    void on_ConnectionConnected();
    void on_ConnectionDisconnected();
    void on_ReceivedMessage(const QByteArray &message, const QMqttTopicName &topic);
    void receivedMessage(QByteArray &msg);


};


#endif // CMQTT_H
