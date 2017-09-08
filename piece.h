#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsItem>
#include "gamestate.h"
class GameView;

class Piece : public QGraphicsItem
{
public:
    Piece(GameView *gameView, Draughts color);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void setTakable(bool t) { takable = t; }

private:
    QPointF newPos;
    GameView *game;
    Draughts m_color;
    bool takable;
};

#endif // PIECE_H
