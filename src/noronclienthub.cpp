/**************************************************************************
**
** This file is part of Noron.
** https://github.com/HamedMasafi/Noron
**
** Noron is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Noron is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Noron.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include <QtNetwork/QTcpSocket>

#include "noronabstracthub_p.h"
#include "noronclienthub_p.h"
#include "noronclienthub.h"
#include "noronabstractserializer.h"
#include "noronpeer.h"

#ifdef QT_QML_LIB
#   include <QtQml>
#endif

QT_BEGIN_NAMESPACE

NoronClientHubPrivate::NoronClientHubPrivate(NoronClientHub *parent) : q_ptr(parent),
    port(0), serverAddress(QString::null), isAutoReconnect(false)
{

}

void NoronClientHubPrivate::sync()
{
    Q_Q(NoronClientHub);

    q->beginTransaction();
qDebug() << "sync";
    NoronPeer *peer = q->peer();

    int pcount = peer->metaObject()->propertyCount();
    for(int i = 0; i < pcount; i++){
        QMetaProperty p = peer->metaObject()->property(i);

        if(!p.isUser())
            continue;

        QString w = p.name();

        qDebug() << "set" + w<<
                p.read(peer);

        w[0] = w[0].toUpper();
        q->invokeOnPeer(peer->metaObject()->className(),
                     "set" + w,
                     p.read(peer));
    }
    q->commit();
}

NoronClientHub::NoronClientHub(QObject *parent) : NoronAbstractHub(parent),
    d_ptr(new NoronClientHubPrivate(this))
{
    connect(this, &NoronAbstractHub::isConnectedChanged, this, &NoronClientHub::isConnectedChanged);
}

QString NoronClientHub::serverAddress() const
{
    Q_D(const NoronClientHub);
    return d->serverAddress;
}

int NoronClientHub::port() const
{
    Q_D(const NoronClientHub);
    return d->port;
}

bool NoronClientHub::isAutoReconnect() const
{
    Q_D(const NoronClientHub);
    return d->isAutoReconnect;
}

#ifdef QT_QML_LIB
static QObject* create_singelton_object_client_hub(QQmlEngine *, QJSEngine *)
{
    return new NoronClientHub();
}

void NoronClientHub::registerQml(const char *uri, int versionMajor, int versionMinor)
{
    qmlRegisterType<NoronClientHub>(uri, versionMajor, versionMinor, "ClientHub");
    qmlRegisterUncreatableType<NoronAbstractHub>(uri, versionMajor, versionMinor, "AbstractHub", "Abstract class for ClientHub base");
    qmlRegisterUncreatableType<NoronPeer>(uri, versionMajor, versionMinor, "NoronPeer", "Abstract type used by custom generated peer");
}

void NoronClientHub::registerQmlSingleton(const char *uri, int versionMajor, int versionMinor)
{
    qmlRegisterSingletonType<NoronClientHub>(uri, versionMajor, versionMinor, "ClientHub", create_singelton_object_client_hub);
    qmlRegisterUncreatableType<NoronAbstractHub>(uri, versionMajor, versionMinor, "AbstractHub", "Abstract class for ClientHub base");
    qmlRegisterUncreatableType<NoronPeer>(uri, versionMajor, versionMinor, "NoronPeer", "Abstract type used by custom generated peer");
}
#endif

void NoronClientHub::timerEvent(QTimerEvent *)
{
    Q_D(NoronClientHub);

    if(socket->state() == QAbstractSocket::UnconnectedState){
        connectToHost();
    }else if(socket->state() == QAbstractSocket::ConnectedState){
        killTimer(d->reconnectTimerId);
        d->sync();
    }
}

void NoronClientHub::connectToHost(bool waitForConnected)
{
    connectToHost(QString::null, 0, waitForConnected);
}

void NoronClientHub::connectToHost(QString address, int port, bool waitForConnected)
{
    if(!address.isNull())
        setServerAddress(address);

    if(port)
        setPort(port);

    socket->connectToHost(this->serverAddress(), this->port());

    if(waitForConnected)
        socket->waitForConnected();
}

void NoronClientHub::disconnectFromHost()
{
    socket->disconnectFromHost();
}

void NoronClientHub::setServerAddress(QString serverAddress)
{
    Q_D(NoronClientHub);

    if (d->serverAddress == serverAddress)
        return;

    d->serverAddress = serverAddress;
    emit serverAddressChanged(serverAddress);
}

void NoronClientHub::setPort(int port)
{
    Q_D(NoronClientHub);

    if (d->port == port)
        return;

    d->port = port;
    emit portChanged(port);
}

void NoronClientHub::setAutoReconnect(bool isAutoReconnect)
{
    Q_D(NoronClientHub);

    if (d->isAutoReconnect == isAutoReconnect)
        return;

    d->isAutoReconnect = isAutoReconnect;
    emit isAutoReconnectChanged(isAutoReconnect);
}

void NoronClientHub::isConnectedChanged(bool isConnected)
{
    Q_D(NoronClientHub);

    if(!isConnected)
        if(isAutoReconnect()){
            connectToHost();
            d->reconnectTimerId = startTimer(500);
        }
}

QT_END_NAMESPACE
