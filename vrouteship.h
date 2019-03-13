#ifndef VROUTESHIP_H
#define VROUTESHIP_H

#include <QMessageBox>
#include <vroute.h>

class VRouteShip : public VRoute
{
    VPointPaintArray *paintPoints;
    QVector<QPointF> *points;
    VPaintMultiLine *paintLines;
    QGraphicsScene *vScene;
    bool f_create_line;
    //bool f_lines;
    float speed;
    UIDType uid;
    bool route_builded;

public:
    VRouteShip();
    ~VRouteShip();  

    void setArea(VArea *){}
    void setVScene(QGraphicsScene *vScene);

    VPointPaintArray *getPaintPoints();
    QVector<QPointF> *getPoints();
    void setPaintPoints(VPaintPoint *point);
    void createFromNet(QVector<QPointF> *points, UIDType uid);
    void remove();
    VPaintMultiLine *getLine();
    void addPointFromCursor(QPointF &pos);

    void setUID(UIDType uid){this->uid = uid;}
    UIDType getUID(){return uid;}

    bool getFlagBuild(){return route_builded;}
    void setFlagBuild(bool f){route_builded = f;}

public slots:
    void create();
    void setDefaultColor();
    void setNumPointClick(int num);
    void set(QPointF);
    void deletePoint(int num);

};

#endif // VROUTESHIP_H
