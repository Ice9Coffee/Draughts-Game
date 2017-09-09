#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

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

    void initServer(quint16 port);
    void connectHost(QString ip, quint16 port);



signals:


private slots:
    void endGame(int result);

    void acceptConnection();
    void recvData();
    void sendTempoData(const QPoint from, const QPoint to);
    void sendAdmitDefeat();
    void sendDrawRequest();

private:
    Ui::GameWindow *ui;

    bool onlineMode;
    QTcpServer *listenSocket;
    QTcpSocket *rwSocket;

    void agreeDraw(bool agree);

};

#endif // GAMEWINDOW_H
