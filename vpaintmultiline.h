#ifndef VPAINTMULTILINE_H
#define VPAINTMULTILINE_H
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <common.h>

class VPaintMultiLine : public QGraphicsItem
{
private:
    bool prevVisibleState;
    QColor color;
    QVector<QPointF> pointsLine;
    QBrush *brush;
    qreal penWidth;
    Qt::PenStyle penStyle;
    UIDType uid;
    QPolygonF *poly;

public:
    VPaintMultiLine(QVector<QPointF> *pointsLine,
                     QBrush *brush = new QBrush(Qt::blue),
                     qreal penWidth = 1,
                     Qt::PenStyle penStyle = Qt::SolidLine
                    );
    ~VPaintMultiLine();
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *,
               QWidget *widget) Q_DECL_OVERRIDE;

    void setUID(UIDType uid){this->uid = uid;}
    UIDType getUID(){return uid;}

    void saveVisibleState(){prevVisibleState = isVisible();}
    void restoreVisibleState(){setVisible(prevVisibleState);}

protected:
    void advance(int step) Q_DECL_OVERRIDE;

};

#endif // VPAINTMULTILINE_H
