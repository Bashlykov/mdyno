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


class VArea: public QObject
{   
    Q_OBJECT

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
