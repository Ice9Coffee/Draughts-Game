#include "const.h"
#include "piecehighlight.h"

#include <QPainter>

pieceHighlight::pieceHighlight() :
    pix(":/pic/pieceHL")
{
    setCacheMode(ItemCoordinateCache);
    setAcceptedMouseButtons(0);
    setZValue(-10);
}

QRectF pieceHighlight::boundingRect() const
{ return QRectF( -CELL_R, -CELL_R, 2*CELL_R, 2*CELL_R ); }

void pieceHighlight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(-CELL_R, -CELL_R, 2*CELL_R, 2*CELL_R, pix);
}
