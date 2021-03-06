#ifndef CLIENT
#define CLIENT

class Client : NoronPeer
{
Q_OBJECT
Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged USER true)




public slots:
    #ifdef QT_QML_LIB
    int getRandomNumber(QJSValue callbackFunction);
    #endif
    #if __cplusplus >= 201103L
    int getRandomNumber(std::function<void(int)> callbackFunction);
    #endif
    Q_INVOKABLE int getRandomNumber(const QObject *obj, char *callbackSlot);
    int getRandomNumber();
    int getRandomNumber(const QObject *obj, const QMetaMethod *callbackMethod);
    int getRandomNumberAsync();
    int getRandomNumberSlot();

signals:
    int getRandomNumberSignal(int returnValue);

};
#endif // CLIENT
