#ifndef TRACEHIGHLIGHT_H
#define TRACEHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPixmap>

class traceHighlight : public QGraphicsItem
{
public:
    traceHighlight();

    enum { Type = UserType + 4 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPixmap pix;

};

#endif // TRACEHIGHLIGHT_H
