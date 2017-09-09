#include "const.h"
#include "piece.h"
#include "gameview.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QColor>
#include <QStyleOption>

#include <QDebug>

Piece::Piece(GameView *gameView, Draughts draught)
    : game(gameView),
      drt(draught)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    setZValue(-1);

}

QRectF Piece::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -PIECE_R - adjust, -PIECE_R - adjust, 2*PIECE_R+3 + 2*adjust, 2*PIECE_R+3 + 2*adjust );
}

QPainterPath Piece::shape() const
{
    QPainterPath path;
    path.addEllipse(-PIECE_R, -PIECE_R, 2*PIECE_R, 2*PIECE_R);
    return path;
}

void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    switch (drt) {
    case blackking:
        painter->setPen(QPen(QColor(255, 233, 0), 5));
        painter->setBrush(QBrush(Qt::black));
        break;
    case black:
        painter->setPen(QPen(Qt::white, 0));
        painter->setBrush(QBrush(Qt::black));
        break;
    case whiteking:
        painter->setPen(QPen(QColor(255, 233, 0), 5));
        painter->setBrush(QBrush(Qt::white));
        break;
    case white:
        painter->setPen(QPen(Qt::black, 0));
        painter->setBrush(QBrush(Qt::white));
    default:
        break;
    }
    painter->drawEllipse(-PIECE_R, -PIECE_R, 2*PIECE_R, 2*PIECE_R);

    /*
    if(takable) {
        QPen pen(QColor(255, 128, 0), 3);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(-PIECE_R - 1, -PIECE_R - 1, 2*PIECE_R + 2, 2*PIECE_R + 2);
    }
    */

}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setZValue(0);
    setCursor(Qt::ClosedHandCursor);

    //QPointF pos = event->buttonDownScenePos(Qt::LeftButton);
    //VFX.
    QPoint p;
    p.rx() = (int)(pos().x() / (CELL_R * 2) + 5);
    p.ry() = (int)(pos().y() / (CELL_R * 2) + 5);
    game->hdPieceHL();
    game->hlWay(p);

    update();
    QGraphicsItem::mousePressEvent(event);
    /*
    if(takable) {
        setCursor(Qt::ClosedHandCursor);
        update();
        QGraphicsItem::mousePressEvent(event);
    }
    */
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    setZValue(-1);

    QPointF fPos = event->buttonDownScenePos(Qt::LeftButton);
    QPointF tPos = event->scenePos();
    QPoint from((int)(fPos.x() / (CELL_R * 2) + 5), (int)(fPos.y() / (CELL_R * 2) + 5));
    QPoint to((int)(tPos.x() / (CELL_R * 2) + 5), (int)(tPos.y() / (CELL_R * 2) + 5));

    this->game->tempo(this, from, to);

    /*/for debug...
    //qDebug() << this << "from" << from << "to" << to;
    //qDebug() << this << "to" << tPos << "in Scene";
    qDebug() << "board state:";
    for(int i = 0; i < 10;++i){
        qDebug()<< this->game->state->board[10*i + 0]
                << this->game->state->board[10*i + 1]
                << this->game->state->board[10*i + 2]
                << this->game->state->board[10*i + 3]
                << this->game->state->board[10*i + 4]
                << this->game->state->board[10*i + 5]
                << this->game->state->board[10*i + 6]
                << this->game->state->board[10*i + 7]
                << this->game->state->board[10*i + 8]
                << this->game->state->board[10*i + 9];
    }
    //*/

    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


