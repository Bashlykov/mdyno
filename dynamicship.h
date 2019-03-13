#ifndef DYNAMICSHIP_H
#define DYNAMICSHIP_H
#include "idynamicobject.h"
#include <vrouteship.h>
#include <dynamicdron.h>
#define EPS 1e-5

class DynamicShip : public IDynamicObject
{
    QVector<QPointF> *vps;
    QColor color;
    QMenu menu;
    QAction *removeAction;
    QAction *setupRouteAction;
    QAction *saveRouteAction;
    QAction *propRouteAction;
    VRouteShip *vRoute;

    VArea *area;
    QList<DynamicShip*> *listThisObjects;
    QList<DynamicDron *> *listChild;
    QList<DynamicDron *> *listChildFromNet;
    DynamicDron *currentChild;

    uint countChild;
    struct steps_struct steps;
    int i_p;
    double prevAngle;
   // bool f_created_child;
   // int currentIdChild;

public:
    DynamicShip(UIDType uid);
    ~DynamicShip() override;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void advance(int step) override;

    void setParentShape(IDynamicObject *) override{}
    void setListThisObjects(QList<IDynamicObject*> *list) override;
    QList<IDynamicObject*> *getListThisObjects() override;
    void createChild(VArea *area) override;
    IDynamicObject *getCurrentChild() override;
    void setCurrentChild(IDynamicObject *currentChild) override;
    QList<IDynamicObject *> *getListChild() override;
    QList<IDynamicObject *> *getListChildFromNet() override{return nullptr;}
    void setVArea(VArea *area) override;
    VArea *getVArea() override;
    void calcRoute() override;
    uint getCountChild() override;

    VRoute* getRoute() override;
    QList<VRoute*> *getListVRouteChild() override { return nullptr;}

    QByteArray serialize() override;
    void deserialize(QByteArray &ba) override;
    friend QDataStream& operator>>( QDataStream& stream, DynamicShip& mapObj );
    friend QDataStream& operator<<( QDataStream& stream, DynamicShip& mapObj );

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public slots:
    void removeThis(bool) override;
    void removeData() override;
    void createRouteChild() override;
    void createdRoute(bool f_created) override;
    void setStartPosition() override;   
    void triggeredSetupRouteAction() override {}
    void getProper(bool) override {}
};

#endif // DYNAMICSHIP_H
