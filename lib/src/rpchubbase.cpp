#include "rpcpeer.h"
#include "rpchubbase.h"
#include "rpcserializerbase_p.h"

RpcHubBase::RpcHubBase(QObject *parent) : QObject(parent),
    m_autoReconnect(false),
    m_serializer(0)
{

}

void RpcHubBase::addSharedObject(RpcPeer *o)
{
    if(!o->inherits("RpcPeer")){
        qFatal("The class %s does not inherits from RpcPeer. Use RpcPeerGenerate tool for creating peer classes",
               o->metaObject()->className());
        return;
    }
    _classes[o->metaObject()->className()] = o;

//    if(o->hub() != this)
//        o->setHub(this);
}

void RpcHubBase::connectToServer(QString address, qint16 port)
{
    Q_UNUSED(address);
    Q_UNUSED(port);

    qWarning("RpcHubBase::connectToServer");
}

bool RpcHubBase::setSocketDescriptor(qintptr socketDescriptor)
{
    Q_UNUSED(socketDescriptor);

    qWarning("RpcHubBase::setSocketDescriptor");

    return false;
}

void RpcHubBase::beginTransaction()
{

}

void RpcHubBase::rollback()
{

}

void RpcHubBase::commit()
{

}

qint16 RpcHubBase::port() const
{
    return m_port;
}

QString RpcHubBase::serverAddress() const
{
    return m_serverAddress;
}

bool RpcHubBase::autoReconnect() const
{
    return m_autoReconnect;
}

bool RpcHubBase::isConnected() const
{
    return m_isConnected;
}

QString RpcHubBase::validateToken() const
{
    return m_validateToken;
}

RpcSerializerBase *RpcHubBase::serializer() const
{
    return m_serializer;
}

void RpcHubBase::setPort(qint16 port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(port);
}

void RpcHubBase::setServerAddress(QString serverAddress)
{
    if (m_serverAddress == serverAddress)
        return;

    m_serverAddress = serverAddress;
    emit serverAddressChanged(serverAddress);
}

void RpcHubBase::setAutoReconnect(bool autoReconnect)
{
    if (m_autoReconnect == autoReconnect)
        return;

    m_autoReconnect = autoReconnect;
    emit autoReconnectChanged(autoReconnect);
}

void RpcHubBase::setIsConnected(bool isConnected)
{
    if (m_isConnected == isConnected)
        return;

    m_isConnected = isConnected;
    emit isConnectedChanged(isConnected);
}

void RpcHubBase::setValidateToken(QString validateToken)
{

    if (m_validateToken == validateToken)
        return;

    m_validateToken = validateToken;
    emit validateTokenChanged(validateToken);
}

void RpcHubBase::setSerializer(RpcSerializerBase *serializerObject)
{
    if (m_serializer == serializerObject)
        return;

    m_serializer = serializerObject;
    emit serializerChanged(serializerObject);
}

