#ifndef USER_H
#define USER_H

#if __cplusplus < 201103L
#   include <functional>
#endif
#include "rpcpeer.h"

#include <QPixmap>

class User : public RpcPeer{
   Q_OBJECT
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QPixmap avator READ avator WRITE setAvator NOTIFY avatorChanged)
    Q_PROPERTY(bool typing READ typing WRITE setTyping NOTIFY typingChanged)

    bool m_typing;
    QString m_username;
    QPixmap m_avator;

signals:
    void usernameChanged();
    void avatorChanged();
    void typingChanged();
    void sendMessageHandle(QString message);
    void messageRecivedHandle(QString username, QString message);
    void roomMessageHandle(QString message);
    void sendImageHandle(QPixmap image);

public:
    Q_INVOKABLE User(QObject *parent = 0);
    QString username() const;
    QPixmap avator() const;
    bool typing() const;

public slots:
    void setUsername(QString username);
    void setAvator(QPixmap avator);
    void setTyping(bool typing);
	// sendMessage
    void sendMessageSlot(QString message);
    void sendMessage(QString message);
    void sendMessageAsync(QString message);
    void sendMessage(QString message, const QObject *obj, char *slotName);
    void sendMessage(QString message, const QObject *obj, const QMetaMethod *method);
#if __cplusplus >= 201103L
    void sendMessage(QString message, std::function<void(void)> func);
#endif
	// messageRecived
    void messageRecivedSlot(QString username, QString message);
    void messageRecived(QString username, QString message);
    void messageRecivedAsync(QString username, QString message);
    void messageRecived(QString username, QString message, const QObject *obj, char *slotName);
    void messageRecived(QString username, QString message, const QObject *obj, const QMetaMethod *method);
#if __cplusplus >= 201103L
    void messageRecived(QString username, QString message, std::function<void(void)> func);
#endif
	// roomMessage
    void roomMessageSlot(QString message);
    void roomMessage(QString message);
    void roomMessageAsync(QString message);
    void roomMessage(QString message, const QObject *obj, char *slotName);
    void roomMessage(QString message, const QObject *obj, const QMetaMethod *method);
#if __cplusplus >= 201103L
    void roomMessage(QString message, std::function<void(void)> func);
#endif
	// sendImage
    void sendImageSlot(QPixmap image);
    void sendImage(QPixmap image);
    void sendImageAsync(QPixmap image);
    void sendImage(QPixmap image, const QObject *obj, char *slotName);
    void sendImage(QPixmap image, const QObject *obj, const QMetaMethod *method);
#if __cplusplus >= 201103L
    void sendImage(QPixmap image, std::function<void(void)> func);
#endif
};
#endif // USER_H
