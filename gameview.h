#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include "gamestate.h"
#include "piece.h"
#include "wayhighlight.h"
#include "piecehighlight.h"
#include "tracehighlight.h"
#include "gamewindow.h"

class GameView : public QGraphicsView
{
    Q_OBJECT

public:
    GameView(QWidget *parent = 0);
    void initGame(Draughts *initBoard = nullptr);

    //For vs.Network mode
    void setRivalColor(Draughts c);

    void tempo(Piece *p, const QPoint from, const QPoint to);
    void remoteTempo(const QPoint from, const QPoint to);

    void hlWay(wayNode* root);
    void hlWay(QPoint pos);
    void hlTrace(QPoint pos);
    void hlPiece(wayNode* nd);
    void hdWayHL();
    void hdPieceHL();
    void hdTraceHL();

signals:
    void tempoData(const QPoint from, const QPoint to);
    void endSignal(int result);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    GameState *state;
    QGraphicsScene *scene;

    wayNode* way;
    Draughts movingColor;
    Draughts rivalColor;

    QPixmap bgPix;

    void turn();

};

#endif // GAMEVIEW_H
