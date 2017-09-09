#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsItem>
#include "gamestate.h"
class GameView;

class Piece : public QGraphicsItem
{
public:
    Piece(GameView *gameView, Draughts draught);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    Draughts drt;

private:
    QPointF newPos;
    GameView *game;    
};

#endif // PIECE_H
