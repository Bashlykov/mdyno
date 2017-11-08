#ifndef DYNAMICSHIP_H
#define DYNAMICSHIP_H
#include "idynamicobject.h"
#include <vrouteship.h>
#include <dynamicdron.h>


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
    bool f_created_child;
    int currentIdChild;

public:
    DynamicShip(UIDType uid);
    ~DynamicShip();
    void paint(QPainter *p, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void advance(int step) Q_DECL_OVERRIDE;

    void setParentShape(IDynamicObject *){}
    void setListThisObjects(QList<IDynamicObject*> *list);
    QList<IDynamicObject*> *getListThisObjects();
    void createChild(VArea *area);
    IDynamicObject *getCurrentChild();
    void setCurrentChild(IDynamicObject *currentChild);
    QList<IDynamicObject *> *getListChild();
    QList<IDynamicObject *> *getListChildFromNet(){return nullptr;}
    void setVArea(VArea *area);
    VArea *getVArea();
    void calcRoute();
    uint getCountChild();

    VRoute* getRoute();
    QList<VRoute*> *getListVRouteChild(){return nullptr;}

    QByteArray serialize();
    void deserialize(QByteArray &ba);
    friend QDataStream& operator>>( QDataStream& stream, DynamicShip& mapObj );
    friend QDataStream& operator<<( QDataStream& stream, DynamicShip& mapObj );

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

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
