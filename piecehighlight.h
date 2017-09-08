#ifndef PIECEHIGHLIGHT_H
#define PIECEHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPixmap>

class pieceHighlight : public QGraphicsItem
{
public:
    pieceHighlight();

    enum { Type = UserType + 3 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPixmap pix;

};

#endif // PIECEHIGHLIGHT_H
