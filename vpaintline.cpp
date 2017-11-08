#include "vpaintline.h"

VPaintLine::VPaintLine(QPointF p1, QPointF p2):
    prevVisibleState( true)
{
    this->p1.setX(p1.x());
    this->p1.setY(p1.y());
    this->p2.setX(p2.x());
    this->p2.setY(p2.y());
    this->line = new QLineF(p1, p2);
}

VPaintLine::~VPaintLine()
{

}

QRectF VPaintLine::boundingRect() const
{
    return QRectF(0, 0, 1000, 1000);
}

QPainterPath VPaintLine::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void VPaintLine::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
        QPen linePen(Qt::black, 1, Qt::SolidLine);
        linePen.setCosmetic(true);
        p->setPen(linePen);
        p->drawLine(*this->line);
}

void VPaintLine::advance(int step)
{
    if (!step)
        return;
}
