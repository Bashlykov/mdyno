#ifndef DYNAMICDRON_H
#define DYNAMICDRON_H
#include "idynamicobject.h"
#include <vroutedron.h>
#include <varea.h>

class DynamicDron : public IDynamicObject
{
    QVector<QLineF> *vls;
    QColor color;
    QMenu menu;
    QAction *removeAction;
    QAction *setupRouteAction;
    QAction *propRouteAction;
    VRouteDron* routeDron;
    IDynamicObject *parentShape;
    VArea* area;

public:
    DynamicDron(UIDType uid);
    ~DynamicDron() override;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setParentShape(IDynamicObject *parent) override;
    void setListThisObjects(QList<IDynamicObject*> *) override{}
    QList<IDynamicObject*> *getListThisObjects() override{return nullptr;}
    void createChild(VArea *) override{}
    IDynamicObject *getCurrentChild() override{return nullptr;}
    void setCurrentChild(IDynamicObject *) override{}
    QList<IDynamicObject *> *getListChild() override{return nullptr;}
    QList<IDynamicObject *> *getListChildFromNet() override{return nullptr;}
    void setVArea(VArea *area) override;
    VArea *getVArea() override;
    void calcRoute() override{}
    uint getCountChild() override{return 0;}

    VRoute* getRoute() override{return routeDron;}
    QList<VRoute*> *getListVRouteChild() override{return nullptr;}

    QByteArray serialize() override;
    void deserialize(QByteArray &ba) override;
    friend QDataStream& operator>>( QDataStream& stream, DynamicDron& mapObj );
    friend QDataStream& operator<<( QDataStream& stream, DynamicDron& mapObj );

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *) override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public slots:
        void triggeredSetupRouteAction() override;
        void removeThis(bool) override;
        void getProper(bool) override;
        void setStartPosition() override;       
        void removeData() override{}
        void createRouteChild() override{}
        void createdRoute(bool) override{}        
};


#endif // DYNAMICDRON_H
