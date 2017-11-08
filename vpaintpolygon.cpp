#include "vpaintpolygon.h"

VPaintPolygon::VPaintPolygon(){}

VPaintPolygon::VPaintPolygon(QVector<QPointF> *pointsPolygon,
                              QColor *color,
                              qreal penWidth,
                              Qt::PenStyle penStyle,
                              QColor *colorPen):
    color ( color),
    penWidth ( penWidth),
    penStyle ( penStyle),
    pointsPolygon ( pointsPolygon),
    poly( new QPolygonF(*pointsPolygon)),
    colorPen ( colorPen),
    prevVisibleState (true)
{

}

VPaintPolygon::~VPaintPolygon()
{

}

QRectF VPaintPolygon::boundingRect() const
{
    return poly->boundingRect();
}

QPainterPath VPaintPolygon::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void VPaintPolygon::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    QBrush mBrush(*color);
    QPen polPen(QBrush(*colorPen), penWidth, penStyle);
    polPen.setCosmetic(true);
    p->setBrush(mBrush);
    p->setPen(polPen);
    p->drawPolygon(*this->poly);
}

void VPaintPolygon::setPoly(QPolygonF &poly)
{
    this->poly = new QPolygonF(poly);
}

QPolygonF& VPaintPolygon::getPoly()
{
    return *poly;
}


void VPaintPolygon::setColorPen(QColor *colorPen)
{
    this->colorPen = colorPen;
}

void VPaintPolygon::setColor(QColor *color)
{
    this->color->setNamedColor(color->name());
    update(boundingRect());
}

QColor *VPaintPolygon::getColor()
{
    return this->color;
}
