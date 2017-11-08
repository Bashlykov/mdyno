#ifndef VROUTESHIP_H
#define VROUTESHIP_H

#include <QMessageBox>
#include <vroute.h>

class VRouteShip : public VRoute
{
    VPointPaintArray *pointsPaintRoute;
    QVector<QPointF> *pointsRoute;
    VPaintMultiLine *linesRouteShip;
    QGraphicsScene *vScene;
    bool f_createLineRoute;
    bool f_linesRoute;
    float speed;
    UIDType uid;
    bool routeBuilded;

public:
    VRouteShip();
    ~VRouteShip();  

    void setArea(VArea *){}
    void setVScene(QGraphicsScene *vScene);

    VPointPaintArray *getPaintPoints();
    QVector<QPointF> *getPoints();
    void setPaintPoints(VPaintPoint *point, double speed);
    void createFromNet(QVector<QPointF> *points, UIDType uid);
    void remove();
    VPaintMultiLine *getLine();
    void addPointFromCursor(QPointF &pos);

    void setSpeed(float speed);
    void setUID(UIDType uid){this->uid = uid;}
    UIDType getUID(){return uid;}

    bool getFlagBuild(){return routeBuilded;}
    void setFlagBuild(bool f){routeBuilded = f;}

public slots:
    void create();
    void setDefaultColor();
    void setNumPointClick(int num);
    void set(QPointF);

};

#endif // VROUTESHIP_H
