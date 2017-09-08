#include "const.h"
#include "wayhighlight.h"

#include <QPainter>

wayHighlight::wayHighlight() :
    pix(":/pic/wayHL")
{
    setCacheMode(ItemCoordinateCache);
    setAcceptedMouseButtons(0);
    setZValue(-10);
}

QRectF wayHighlight::boundingRect() const
{ return QRectF( -CELL_R, -CELL_R, 2*CELL_R, 2*CELL_R ); }

void wayHighlight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(-CELL_R, -CELL_R, 2*CELL_R, 2*CELL_R, pix);
}

