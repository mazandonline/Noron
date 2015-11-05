#ifndef RPCHUB_H
#define RPCHUB_H

#include <QtCore/QObject>
#include <QHash>
#include <QSet>

#define ID                  "_id"
#define CLASS_NAME          "_c"
#define METHOD_NAME         "_m"
#define MAP_TYPE            "_t"
#define MAP_RETURN_VALUE    "_return_value"
#define MAP_TYPE_REQUEST    "request"
#define MAP_TYPE_RESPONSE   "response"
#define MAP_TOKEN_ITEM  "_token"

#include "rpcglobal.h"
#include "rpchubbase.h"

class QTcpSocket;
class RpcPeer;
class RpcSerializerBase;
class TOOJ_EXPORT RpcHub : public RpcHubBase
{
    Q_OBJECT

    qint16 _port;
    QString _serverAddress;

    QSet<QString> _locks;

    QVariantList _buffer;
    QTcpSocket *_socket;
    bool _isTransaction;
//    bool _sendBlocked;
    qlonglong requestId;
    int reconnectTimerId;

public:
    explicit RpcHub(QObject *parent = 0);
    explicit RpcHub(RpcSerializerBase *serializer, QObject *parent = 0);


    Q_INVOKABLE void beginTransaction();
    Q_INVOKABLE bool isTransaction() const;
    Q_INVOKABLE void rollback();
    Q_INVOKABLE void commit();

    Q_INVOKABLE void connectToServer(QString address = QString::null, qint16 port = 0);
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE bool setSocketDescriptor(qintptr socketDescriptor);

    bool sendBlocked() const;
    void timerEvent(QTimerEvent *);
    void sync();

private:
    void addToMap(QVariantMap *map, QVariant var, int index);
    void procMap(QVariantMap map);
    bool response(qlonglong id, QString senderName, QVariant returnValue);
    void addValidateToken(QVariantMap *map);
    bool checkValidateToken(QVariantMap *map);
    QString MD5(QString text);
    QString MD5(QByteArray text);

signals:
    void disconnected();
    void sendBlockedChanged(bool);

public slots:
    void socket_connected();
    void socket_disconnected();
    void socket_onReadyRead();
    qlonglong invokeOnPeer(
            QString sender,
            QString methodName,
            QVariant val0 = QVariant(),
            QVariant val1 = QVariant(),
            QVariant val2 = QVariant(),
            QVariant val3 = QVariant(),
            QVariant val4 = QVariant(),
            QVariant val5 = QVariant(),
            QVariant val6 = QVariant(),
            QVariant val7 = QVariant(),
            QVariant val8 = QVariant(),
            QVariant val9 = QVariant());


};

#endif // RPCHUB_H
