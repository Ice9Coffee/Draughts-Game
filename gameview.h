#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include "gamestate.h"
#include "piece.h"

class GameView : public QGraphicsView
{
    Q_OBJECT

public:
    GameView(QWidget *parent = 0);

    GameState *state;

    void tempo(Piece *p, QPoint from, QPoint to);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QGraphicsScene *scene;

    QPixmap bgPix;

    wayNode* way;

    Draughts movingColor;

    void turn();

};

#endif // GAMEVIEW_H
