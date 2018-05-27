#ifndef VAREA_H
#define VAREA_H

#include <math.h>
#include <QObject>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qmenu.h>
#include <qdebug.h>

#include <vpaintpolygon.h>
#include <vpaintpoint.h>
#include <vpaintline.h>
#include <vpaintmultiline.h>
#include <common.h>
#include <QVector2D>
#include <QGraphicsTextItem>

QVector2D &calcBezierPoint(float t, QVector2D &p0, QVector2D &p1, QVector2D &p2, QVector2D &p3);
QVector<QLineF> *drawBezier(QVector2D &p0, QVector2D &p1, QVector2D &p2, QVector2D &p3);
QVector<QLineF*> *clipping_lines(QVector<QLineF*> *lines_not_clip, QPolygonF &polygon);
void connect_points_bezier(QVector<QPointF> *path_dron_points,
                        QVector<QLineF*> *lines_clip);

void _connect_points(QVector<QPointF> *dron_points,
                        QVector<QLineF *> *lines_clip);

class VArea: public QObject
{   
    Q_OBJECT
//
    VPaintPolygon *r;
    QVector<QPointF> *v;
    QVector<VPaintLine*> *paint_lines;
    QVector<QPointF> *dron_points;
    QPolygonF *path_dron;


//
    VPointPaintArray *paintPoints;
    QVector<QPointF> *points;
    VPaintPolygon *paintPoly;
    VPaintMultiLine *paintLines;
    QPolygonF *poly;
    QGraphicsScene *vScene;
    QList<VArea*> *listThisObjects;
    UIDType uid;
    bool f_create_poly;
    bool f_lines;

    QAction *removeAction;
    QAction *addPointAction;
    QAction *saveAction;
    QAction *propertyAction;

public:
    VArea();
    ~VArea();
    VPointPaintArray *getPointsPaint();
    void setPolygon(QPolygonF *poly);
    QVector<QPointF> *getPoints();
    void setPointsPaint(VPointPaintArray *paintPoints);
    void createPoint(QPointF &p);
    void setPointPaint(VPaintPoint *point);
    void create(QPointF &pos);
    void setVScene(QGraphicsScene *vScene);
    VPaintPolygon *getPolygonPaint();
    void createPolyFromNet(QVector<QPointF> *points, UIDType uid);
    void setUID(UIDType uid){this->uid = uid;}
    UIDType getUID(){return uid;}
    void setListThisObjects(QList<VArea*> *listThisObjects){this->listThisObjects = listThisObjects;}
    QMenu menu;

    friend QDataStream& operator>>( QDataStream& stream, VArea& area );
    friend QDataStream& operator<<( QDataStream& stream, VArea& area );

private slots:
    void removeThis();
    void removePoly();
    void createPoly();
    void setPoly(QPointF);
    void deletePoint(int num);

public slots:
    void setDefaultColor();

signals:
    void createPointAreaSig(VPaintPoint *newPoint);
    void addingPoint();
    void savePoly();
};

#endif // VAREA_H
