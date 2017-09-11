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
    setFlag(ItemIgnoresTransformations);
    setCacheMode(DeviceCoordinateCache);
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    setZValue(-1);
}

QRectF Piece::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -PIECE_VR - adjust, -PIECE_VR - adjust, 2*PIECE_VR+3 + 2*adjust, 2*PIECE_VR+3 + 2*adjust );
}

QPainterPath Piece::shape() const
{
    QPainterPath path;
    path.addEllipse(-PIECE_VR, -PIECE_VR, 2*PIECE_VR, 2*PIECE_VR);
    return path;
}

void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    switch (drt) {
    case blackking:
        painter->drawPixmap(-PIECE_VR, -PIECE_VR, 2*PIECE_VR, 2*PIECE_VR, QPixmap(":/pic/blackking"));
        break;
    case black:
        painter->drawPixmap(-PIECE_VR, -PIECE_VR, 2*PIECE_VR, 2*PIECE_VR, QPixmap(":/pic/black"));
        break;
    case whiteking:
        painter->drawPixmap(-PIECE_VR, -PIECE_VR, 2*PIECE_VR, 2*PIECE_VR, QPixmap(":/pic/whiteking"));
        break;
    case white:
        painter->drawPixmap(-PIECE_VR, -PIECE_VR, 2*PIECE_VR, 2*PIECE_VR, QPixmap(":/pic/white"));
    default:
        break;
    }
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setZValue(0);
    setCursor(Qt::ClosedHandCursor);

    //VFX.
    QPoint p;
    p.rx() = (int)(pos().x() / (CELL_R * 2) + 5);
    p.ry() = (int)(pos().y() / (CELL_R * 2) + 5);
    game->hdPieceHL();
    game->hlWay(p);
    game->hdTraceHL();

    game->playMoveSE();

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setZValue(-1);
    setCursor(Qt::OpenHandCursor);

    game->playMoveSE();

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


