#include "clientwindow.h"

#include "user.h"
#include "server.h"

#include "defines.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent), _resourceId(0)
{

    RpcHub *hub = new RpcHub(this);
    hub->setObjectName("hub");
    hub->setAutoReconnect(true);
    hub->setValidateToken(RPC_TOKEN);

    Server *server = new Server(this);
    server->setObjectName("server");
    server->setHub(hub);

    user = new User();
    user->setObjectName("user");
    user->setHub(hub);

    setupUi(this);
}

void ClientWindow::changeEvent(QEvent *e)
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

void ClientWindow::keyPressEvent(QKeyEvent *e)
{
    if(textEditMessage->hasFocus() && e->key() == Qt::Key_Return)
        on_pushButtonSend_clicked();
}

void ClientWindow::on_pushButtonLogin_clicked()
{

    user->connectToServer(lineEditServer->text(), spinBoxPort->value());

    user->setUsername(lineEditUsername->text());
    labelUsername->setText(user->username());
    stackedWidget->setCurrentIndex(1);
}

void ClientWindow::on_pushButtonSend_clicked()
{
    user->sendMessageAsync(textEditMessage->toPlainText());
    textEditMessage->setPlainText("");
}

void ClientWindow::on_pushButtonChangeAvator_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));

    if(!fileName.isNull()){
        QPixmap pixmap(fileName);
        if(!pixmap.isNull()){
            user->setAvator(pixmap);
            labelAvator->setPixmap(pixmap);
        }
    }
}

void ClientWindow::on_server_userJoinedHandle(QString username)
{
    textEdit->append(QString("<span style=\" font-weight:600; color:#aa0000;\">%1 has joined to channel.</span>")
                     .arg(username));
}

void ClientWindow::on_server_imageSentHandle(QString username, QPixmap image)
{
    QString url = QString("res://image_%1.png").arg(_resourceId++);
    textEdit->document()->addResource(QTextDocument::ImageResource,
                                      QUrl(url),
                                      QVariant(image));

    textEdit->append("<b>" + username + "</b> Sent an image:<br /><img src=\"" + url + "\" />");
}

void ClientWindow::on_server_broadcastMessageHandle(QString message)
{
    QMessageBox::information(this, "Broadcast", message);
}

void ClientWindow::on_hub_isConnectedChanged(bool isConnected)
{
    if(isConnected)
        statusBar()->showMessage("Connected");
    else
        statusBar()->showMessage("Disonnected");
}

void ClientWindow::on_user_messageRecivedHandle(QString username, QString message)
{
    textEdit->append(QString("<b><font color=%3>%1 :</font></b> %2")
                     .arg(username)
                     .arg(message)
                     .arg(username == user->username() ? "red" : "blue"));
}

void ClientWindow::on_user_roomMessageHandle(QString message)
{
    textEdit->append(QString("<span style=\" font-weight:600; color:#aa0000;\">%1</span>")
                     .arg(message));
}

void ClientWindow::imageSent()
{
    QMessageBox::information(this, "Send image", "Image was sent");
}

void ClientWindow::on_pushButtonSendImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));

    if(!fileName.isNull()){
        QPixmap pixmap(fileName);
        if(!pixmap.isNull())
            //user->sendImage(pixmap, this, &ClientWindow::imageSent);
            user->sendImage(pixmap, this, "imageSent");

            /*
             * if c++11 is enabled flowing code will be send image and show a messagebox
            user->sendImage(pixmap, [this](){
                QMessageBox::information(this, "Send image", "Image was sent");
            });
            */
    }
}
