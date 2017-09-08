#ifndef WAYHIGHLIGHT_H
#define WAYHIGHLIGHT_H

#include <QGraphicsItem>
#include <QPixmap>

class wayHighlight : public QGraphicsItem
{
public:
    wayHighlight();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPixmap pix;

};

#endif // WAYHIGHLIGHT_H
