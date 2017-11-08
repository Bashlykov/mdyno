#ifndef VPAINTLINEPAINT_H
#define VPAINTLINEPAINT_H
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsScene>


class VPaintLine : public QGraphicsItem
{
    //UIDType uid;
    bool prevVisibleState;
    QColor color;
    QPointF p1, p2;
    QLineF *line;
public:
    VPaintLine(QPointF p1, QPointF p2);
    ~VPaintLine();
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;

    //void setUID(UIDType uid){this->uid = uid;}
    //UIDType getUID(){return uid;}
    void saveVisibleState(){prevVisibleState = isVisible();}
    void restoreVisibleState(){setVisible(prevVisibleState);}

protected:
    void advance(int step) Q_DECL_OVERRIDE;

};

#endif // VPAINTLINEPAINT_H
