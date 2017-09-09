#include "const.h"
#include "gamewindow.h"
#include "ui_gamewindow.h"

#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    onlineMode(false)
{
    ui->setupUi(this);

    connect(ui->gameView, SIGNAL(endSignal(int)), this, SLOT(endGame(int)));
    connect(ui->gameView, SIGNAL(tempoData(QPoint, QPoint)),
            this, SLOT(sendTempoData(QPoint, QPoint)));

    //ui->gameView->initGame(); //normal start.

    /*/for test...
    Draughts* tBoard = new Draughts[100];
    memset(tBoard, null, 100*sizeof(Draughts));
    tBoard[12] = white;
    tBoard[16] = black;
    tBoard[18] = black;
    tBoard[29] = white;
    tBoard[32] = white;
    tBoard[43] = black;
    tBoard[52] = white;
    tBoard[65] = white;
    tBoard[72] = white;
    tBoard[85] = black;
    ui->gameView->initGame(tBoard);
    //*/

    //ui->gameView->setRivalColor(black); //for test
    //ui->gameView->setRivalColor(white); //for test
    //ui->gameView->setRivalColor(null); //for test
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::initServer(quint16 port) {
    listenSocket = new QTcpServer;
    listenSocket->listen(QHostAddress::Any, port);
    connect(listenSocket, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
    qDebug() << "Sever established! Listening...";
    /*
    QMessageBox::information(this, TITLE,
        tr("Listening..."), QMessageBox::Cancel);
    listenSocket->close();
    delete listenSocket;
    */
}

void GameWindow::acceptConnection() {
    rwSocket = listenSocket->nextPendingConnection();
    connect(rwSocket, SIGNAL(readyRead()), this, SLOT(recvData()));

    ui->gameView->initGame();
    ui->gameView->setRivalColor(white);
    onlineMode = true;
}

void GameWindow::connectHost(QString ip, quint16 port)
{
    rwSocket = new QTcpSocket;
    rwSocket->connectToHost(QHostAddress(ip), port);
    connect(rwSocket, SIGNAL(readyRead()), this, SLOT(recvData()));

    ui->gameView->initGame();
    ui->gameView->setRivalColor(black);
    onlineMode = true;
}

void GameWindow::recvData()
{
    QByteArray *bytes;
    bytes = new QByteArray(rwSocket->readAll());

    int Madoka;
    QPoint from, to;
    QDataStream in(bytes, QIODevice::ReadOnly);
    in >> Madoka;

    switch (Madoka) {
    case tempo:
        in >> from.rx() >> from.ry() >> to.rx() >> to.ry();
        qDebug() << "Recieve remote tempo" << "from" << from
                 << "to" << to;
        ui->gameView->remoteTempo(from, to);
        break;
    case admitDefeat:
        QMessageBox::information(this, TITLE,
            tr("Your rival admitted defeat!"));
        endGame(1);
        break;
    case drawRequest:
        if(QMessageBox::Yes == QMessageBox::question(this, TITLE,
            tr("Your rival ask to DRAW with you.\nDo you agree?"),
            QMessageBox::Yes, QMessageBox::No))
        {
            agreeDraw(true);
            endGame(0);
        }
        else { agreeDraw(false); }
        break;
    case drawAccept:
        QMessageBox::information(this, TITLE,
            tr("Your rival agreed to DRAW!"));
        endGame(0);
        break;
    case drawRefuse:
        QMessageBox::critical(this, TITLE,
            tr("Your rival refused to DRAW!"));
        break;
    default:
        qDebug() << "Unknown magic!!!";
        break;
    }

}

void GameWindow::sendTempoData(const QPoint from, const QPoint to)
{
    if(!onlineMode) return;
    QByteArray *bytes = new QByteArray;
    QDataStream out(bytes, QIODevice::WriteOnly);
    out << Magic::tempo << from.x() << from.y() << to.x() << to.y();

    int byteCount = rwSocket->write(*bytes);
    qDebug() << byteCount << "bytes" << "sendTempoData:" << &bytes;
}

void GameWindow::sendAdmitDefeat()
{
    if(!onlineMode) return;
    QByteArray *bytes = new QByteArray;
    QDataStream out(bytes, QIODevice::WriteOnly);
    out << Magic::admitDefeat;

    int byteCount = rwSocket->write(*bytes);
    qDebug() << byteCount << "bytes" << "sendAdmitDefeat:" << &bytes;
}

void GameWindow::sendDrawRequest()
{
    if(!onlineMode) return;
    QByteArray *bytes = new QByteArray;
    QDataStream out(bytes, QIODevice::WriteOnly);
    out << Magic::drawRequest;

    int byteCount = rwSocket->write(*bytes);
    qDebug() << byteCount << "bytes" << "sendDrawRequest:" << &bytes;
}

void GameWindow::agreeDraw(bool agree)
{
    if(!onlineMode) return;
    QByteArray *bytes = new QByteArray;
    QDataStream out(bytes, QIODevice::WriteOnly);
    out << (agree ? Magic::drawAccept : Magic::drawRefuse);

    int byteCount = rwSocket->write(*bytes);
    qDebug() << byteCount << "bytes" << "agreeDraw:" << &bytes;
}

void GameWindow::endGame(int result) {
    switch (result) {
    case black:
        QMessageBox::information(this, TITLE,
            tr("Black WIN!"));
        break;
    case white:
        QMessageBox::information(this, TITLE,
            tr("White WIN!"));
        break;
    case 1:
        QMessageBox::information(this, TITLE,
            tr("Winner! Winner! Chicken Dinner!\nYou WIN!"));
        break;
    case 0:
        QMessageBox::information(this, TITLE,
            tr("DRAW!"));
        break;
    case -1:
        QMessageBox::information(this, TITLE,
            tr("Checkmate!\nYou LOSE!"));
        break;
    default: //should never reach.
        break;
    }
    ui->gameView->setEnabled(false);
}

void GameWindow::on_serverAction_triggered() {
    ServerDlg* serverDlg = new ServerDlg(this);
    connect(serverDlg, SIGNAL(serverSet(quint16)), this, SLOT(initServer(quint16)));
    serverDlg->exec();
}

void GameWindow::on_clientAction_triggered()
{
    ClientDlg* clientDlg = new ClientDlg(this);
    connect(clientDlg, SIGNAL(clientSet(QString, quint16)), this, SLOT(connectHost(QString,quint16)));
    clientDlg->exec();
}
