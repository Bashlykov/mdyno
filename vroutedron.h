#ifndef VROUTEDRON_H
#define VROUTEDRON_H

#include <QMessageBox>
#include <qdebug.h>
#include <math.h>
#include <vroute.h>

class RouteDron;

class VRouteDron : public VRoute
{   
    VPointPaintArray *paintPointsRoute;
    QVector<QPointF> *pointsRoute;
    VPaintMultiLine *lineRoute;
    RouteDron *routeDron;
    bool f_createLineRouteParent;
    bool f_createRoute;

    QString *layerName;
    VArea *varea;
    QGraphicsScene *vScene;

    QString msgExept;
    QString &getMsgExcept();

public:
    VRouteDron();
    ~VRouteDron();
    void setArea(VArea *varea);
    void setVScene(QGraphicsScene *vScene);

    VPointPaintArray *getPaintPoints();
    QVector<QPointF> *getPoints();
    void setPaintPoints(VPaintPoint *point, double speed);
    void createFromNet(QVector<QPointF> *pointsRouteDron, UIDType uid = 0);
    void remove();
    VPaintMultiLine *getLine(){return nullptr;}
    void addPointFromCursor(QPointF &){}

    void setSpeed(float){}
    void setUID(UIDType ){}
    UIDType getUID(){return 0;}

    bool getFlagBuild(){return false;}
    void setFlagBuild(bool){}

public slots:
    void create();
    void setDefaultColor(){}
    void setNumPointClick(int){}
    void set(QPointF){}

private slots:
    void finish();   
    void _create();
    void msgWarning();
};


#endif // VROUTEDRON_H
