#include "mainwindow.h"

#include <RpcServer>

#include "user.h"
#include "server.h"
#include "defines.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    serverManager = new RpcServer(PORT, this);
    serverManager->setIsMultiThread(true);
    serverManager->registerType<User*>();
    serverManager->setObjectName("serverManager");
    serverManager->setValidateToken(RPC_TOKEN);

    server = new Server(this);
    server->setHub(serverManager);

    setupUi(this);

    /*
     Or, ... with a c++11 simple way
    connect(serverManager, &ServerPeersManager::peerConnected, this, [this] (RpcPeer *p) {
        populatePeersList();

        User *peer = qobject_cast<User*>(p);
        connect(peer, &UserBase::usernameChanged, this, [this] () {
            populatePeersList();

            foreach (RpcPeer *p, serverManager->peers()) {
                User *peer = qobject_cast<User*>(p);
                if(peer != sender()){
                    User *s = qobject_cast<User*>(sender());
                    peer->roomMessage(s->username() + " joined room.");
                }
            }
        });
        connect(peer, &UserBase::sendMessageSignal, this, [this] (QString message) {
            User  *s = qobject_cast<User*>(sender());
            foreach (RpcPeer *p, serverManager->peers()) {
                User *peer = qobject_cast<User*>(p);
                peer->messageRecivedAsync(s->username(), message);
            }
        });
    });
    */
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::populatePeersList()
{
    QVariantList users;
    listWidgetOnlineUsers->clear();
    foreach (QObject *p, serverManager->peers()) {
        User *peer = qobject_cast<User*>(p);

        QListWidgetItem *item = new QListWidgetItem("User: " + peer->username());
        item->setIcon(QIcon(peer->avator()));
        listWidgetOnlineUsers->addItem(item);

        QVariantMap userMap;
        userMap["username"] = peer->username();
        userMap["avator"] = peer->avator();
        users.append(userMap);
    }
    server->setUsers(users);
}

void MainWindow::on_serverManager_peerConnected(RpcPeer *peer)
{
    User *user = qobject_cast<User*>(peer);
    populatePeersList();

    connect(user, &User::avatorChanged, this, &MainWindow::populatePeersList);
    connect(user, &User::usernameChanged, this, &MainWindow::user_usernameChanged);
    connect(user, &User::sendMessageSignal, this, &MainWindow::user_sendMessageSignal);
    connect(user, &User::sendImageSignal, this, &MainWindow::user_sendImageSignal);
}

void MainWindow::on_serverManager_peerDisconnected(RpcPeer *peer)
{
    Q_UNUSED(peer);
    populatePeersList();
}

void MainWindow::user_sendImageSignal(QPixmap image)
{
    User *user = qobject_cast<User*>(sender());
    server->imageSent(user->username(), image);
}

void MainWindow::user_sendMessageSignal(QString message)
{
    User *s = qobject_cast<User*>(sender());
    foreach (RpcPeer *p, serverManager->peers()) {
        User *peer = qobject_cast<User*>(p);
        peer->messageRecivedAsync(s->username(), message);
    }
}

void MainWindow::user_usernameChanged()
{
    User *messageSender = qobject_cast<User*>(sender());
    populatePeersList();

    foreach (RpcPeer *peer, serverManager->peers()) {
        if(messageSender != sender()){
            qobject_cast<User*>(peer)->roomMessage(messageSender->username() + " joined room.");
        }
    }

    server->userJoined(messageSender->username());
}

void MainWindow::on_pushButtonSendBroadcast_clicked()
{
    server->broadcastMessageAsync(lineEditMessage->text());
}
