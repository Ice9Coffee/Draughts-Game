#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include "serverdlg.h"
#include "clientdlg.h"
#include "bgmthread.h"

enum Magic {
    tempo = 0x1ce9,
    admitDefeat = 0x0990,
    drawRequest = 0xddd0,
    drawAccept = 0xddd1,
    drawRefuse = 0xddd2
};

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

public slots:
    void initServer(quint16 port);
    void connectHost(QString ip, quint16 port);

private slots:
    void endGame(int result);

    void acceptConnection();
    void recvData();
    void sendTempoData(const QPoint from, const QPoint to);
    void sendAdmitDefeat();
    void sendDrawRequest();

    void on_serverAction_triggered();
    void on_clientAction_triggered();
    void on_personalAction_triggered();
    void on_testExampleAction_triggered();

    void on_admitDefeatAction_triggered();

    void on_drawRequestAction_triggered();

private:
    Ui::GameWindow *ui;

    bool onlineMode;
    QTcpServer *listenSocket;
    QTcpSocket *rwSocket;

    BgmThread* bgm;

    void agreeDraw(bool agree);

};

#endif // GAMEWINDOW_H
