#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class RpcServer;
class RpcPeer;
class Server;
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    RpcServer *serverManager;
    Server *server;
public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
    void populatePeersList();

public slots:
    void server_peerConnected(RpcPeer *peer);
    void server_peerDisconnected(RpcPeer *peer);

    void user_sendImageHandle(QPixmap image);
    void user_sendMessageHandle(QString message);
    void user_usernameChanged();
    void user_avatorChanged();

private slots:
    void on_pushButtonSendBroadcast_clicked();
};

#endif // MAINWINDOW_H
