#ifndef VPAINTPOINT_H
#define VPAINTPOINT_H
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsScene>
#include <QObject>

class VPaintPoint : public QGraphicsObject
{
    Q_OBJECT

    int num;
    bool f_move;
    bool prevVisibleState;
    Qt::GlobalColor color;
    QPointF dragStartPosition;
    Qt::GlobalColor penColor;
    Qt::GlobalColor tmpColor;
    Qt::GlobalColor tmpPenColor;
    QRect size;
    //UIDType uid;

public:
    VPaintPoint( QRect size,
                 bool f_move = false,
                 Qt::GlobalColor color = Qt::blue,
                 Qt::GlobalColor penColor = Qt::black,
                 int num = 0 );

    ~VPaintPoint();
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *,
               QWidget *widget) Q_DECL_OVERRIDE;

    int getNum(){return num;}
    //void setUID(UIDType uid){this->uid = uid;}
    //UIDType getUID(){return uid;}
    void saveVisibleState(){prevVisibleState = isVisible();}
    void restoreVisibleState(){setVisible(prevVisibleState);}

protected:
    void advance(int step) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void selectFromRow(int num);
    void setDefaultColor();

signals:
    void setNewPos(QPointF);
    void setNumNewPos(int num, QPointF);
    void numPoint(int num);
    void numPointNumGroup(int num, int numGroup);

};

typedef QVector<VPaintPoint*> VPointPaintArray;

#endif // VPAINTPOINT_H
