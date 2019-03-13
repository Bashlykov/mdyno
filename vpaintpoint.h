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
#include <QMenu>

class VPaintPoint : public QGraphicsObject
{
    Q_OBJECT

    int num;
    bool f_move;
    bool prevVisibleState;
    Qt::GlobalColor color;
    Qt::GlobalColor penColor;
    Qt::GlobalColor tmpColor;
    Qt::GlobalColor tmpPenColor;
    QRect size;
    //UIDType uid;

    QMenu menu;
    QAction *removeAction;

public:
    VPaintPoint( QRect size,
                 bool f_move = false,
                 Qt::GlobalColor color = Qt::blue,
                 Qt::GlobalColor penColor = Qt::black,
                 int num = 0 );

    ~VPaintPoint() override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *,
               QWidget *widget) override;

    int getNum(){return num;}
    //void setUID(UIDType uid){this->uid = uid;}
    //UIDType getUID(){return uid;}
    void saveVisibleState(){prevVisibleState = isVisible();}
    void restoreVisibleState(){setVisible(prevVisibleState);}

protected:
    void advance(int step) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;


private slots:
    void slotForDelSignal(bool);

public slots:
    void setDefaultColor();


signals:
    void setNewPos(QPointF);
    void setNumNewPos(int num, QPointF);
    void numPoint(int num);
    void numPointNumGroup(int num, int numGroup);
    void deletingThis(int);

};

typedef QVector<VPaintPoint*> VPointPaintArray;

#endif // VPAINTPOINT_H
