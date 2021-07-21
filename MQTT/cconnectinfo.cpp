#include "cconnectinfo.h"

cConnectInfo::cConnectInfo(){}

cConnectInfo::cConnectInfo(const cConnectInfo &other)
:m_HostName(other.m_HostName),
  m_Port(other.m_Port),
  m_ClientId(other.m_ClientId),
  m_UserName(other.m_UserName),
  m_password(other.m_password),
  m_QoS(other.m_QoS),
  m_TopicPublicNoResponse(other.m_TopicPublicNoResponse),
  m_TopicSubscribeNoResponse(other.m_TopicSubscribeNoResponse)
{}

cConnectInfo::~cConnectInfo(){}

QString cConnectInfo::getHostName() const
{
    return m_HostName;
}

quint16 cConnectInfo::getPort() const
{
    return m_Port;
}

QString cConnectInfo::getClientId() const
{
    return m_ClientId;
}

QString cConnectInfo::getUserName() const
{
    return m_UserName;
}

QString cConnectInfo::getPassword() const
{
    return m_password;
}

quint8 cConnectInfo::getQoS() const
{
    return m_QoS;
}

QString cConnectInfo::getTopicPublicNoResponse() const
{
    return m_TopicPublicNoResponse;
}

QString cConnectInfo::getTopicSubscribeNoResponse() const
{
    return m_TopicSubscribeNoResponse;
}

QStringList cConnectInfo::getNodeIDs() const
{
    return m_NodeID;
}

QStringList cConnectInfo::getAppID() const
{
    return m_AppID;
}

void cConnectInfo::setHostName(QString hostname)
{
    m_HostName = hostname;
}

void cConnectInfo::setPort(quint16 port)
{
    m_Port= port;
}

void cConnectInfo::setClientId(QString clientId)
{
    m_ClientId= clientId;
}

void cConnectInfo::setUserName(QString user)
{
    m_UserName = user;
}

void cConnectInfo::setPassword(QString password)
{
    m_password = password;
}

void cConnectInfo::setQoS(quint8 qos)
{
    m_QoS = qos;
}

void cConnectInfo::setTopicPublicNoResponse(QString topic)
{
    m_TopicPublicNoResponse = topic;
}

void cConnectInfo::setTopicSubscribeNoResponse(QString topic)
{
    m_TopicSubscribeNoResponse = topic;
}

void cConnectInfo::setNodeID(QStringList nodeid)
{
    m_NodeID = nodeid;
}

void cConnectInfo::setAppID(QStringList appID)
{
    m_AppID = appID;
}
