#ifndef VPAINTPOLYGON_H
#define VPAINTPOLYGON_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <common.h>

class VPaintPolygon : public QGraphicsObject
{
    Q_OBJECT
    bool prevVisibleState;
    QColor *color;
    qreal penWidth;
    Qt::PenStyle penStyle;
    QVector<QPointF> stuff;
    QColor *colorPen;
    UIDType uid;
    QPolygonF *poly;
    QVector<QPointF> *pointsPolygon;

protected:
    void setPoly(QPolygonF& poly);

public:
    VPaintPolygon();
    VPaintPolygon(QVector<QPointF> *pointsPolygon,
                   QColor *color = new QColor(Qt::black),
                   qreal penWidth = 1,
                   Qt::PenStyle penStyle = Qt::SolidLine,
                   QColor *colorPen = new QColor(Qt::black) );
    ~VPaintPolygon();
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *,
                                           QWidget *widget) Q_DECL_OVERRIDE;

    void setColorPen(QColor *colorPen);
    void setColor(QColor *color);
    QColor *getColor();

    void setUID(UIDType uid){this->uid = uid;}
    UIDType getUID(){return uid;}

    void saveVisibleState(){prevVisibleState = isVisible();}
    void restoreVisibleState(){setVisible(prevVisibleState);}

    QPolygonF& getPoly();
};

#endif // VPAINTPOLYGON_H
