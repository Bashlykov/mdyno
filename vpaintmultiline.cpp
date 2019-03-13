#include "vpaintmultiline.h"

VPaintMultiLine::VPaintMultiLine(QVector<QPointF> *pointsLine,
                                  QBrush *brush,
                                  qreal penWidth,
                                  Qt::PenStyle penStyle ):
    prevVisibleState(true),
    brush(brush),
    penWidth(penWidth),
    penStyle(penStyle)
{
    this->poly = new QPolygonF(*pointsLine);
}

VPaintMultiLine::~VPaintMultiLine()
{

}

QRectF VPaintMultiLine::boundingRect() const
{
    return poly->boundingRect();
}

QPainterPath VPaintMultiLine::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void VPaintMultiLine::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen linePen(*brush, penWidth, penStyle);
    linePen.setCosmetic(true);
    p->setPen(linePen);
    p->drawPolyline(*this->poly);
}

void VPaintMultiLine::advance(int step)
{
    if (!step)
        return;
}

