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
    ~DynamicDron();
    void paint(QPainter *p, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setParentShape(IDynamicObject *parent);
    void setListThisObjects(QList<IDynamicObject*> *){}
    QList<IDynamicObject*> *getListThisObjects(){return nullptr;}
    void createChild(VArea *){}
    IDynamicObject *getCurrentChild(){return nullptr;}
    void setCurrentChild(IDynamicObject *){}
    QList<IDynamicObject *> *getListChild(){return nullptr;}
    QList<IDynamicObject *> *getListChildFromNet(){return nullptr;}
    void setVArea(VArea *area);
    VArea *getVArea();
    void calcRoute(){}
    uint getCountChild(){return 0;}

    VRoute* getRoute(){return routeDron;}
    QList<VRoute*> *getListVRouteChild(){return nullptr;}

    QByteArray serialize();
    void deserialize(QByteArray &ba);
    friend QDataStream& operator>>( QDataStream& stream, DynamicDron& mapObj );
    friend QDataStream& operator<<( QDataStream& stream, DynamicDron& mapObj );

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

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
