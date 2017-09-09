#include "const.h"
#include "tracehighlight.h"

#include <QPainter>

traceHighlight::traceHighlight() :
    pix(":/pic/traceHL")
{
    setCacheMode(ItemCoordinateCache);
    setAcceptedMouseButtons(0);
    setZValue(-10);
}

QRectF traceHighlight::boundingRect() const
{ return QRectF( -CELL_R, -CELL_R, 2*CELL_R, 2*CELL_R ); }

void traceHighlight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(-CELL_R, -CELL_R, 2*CELL_R, 2*CELL_R, pix);
}
