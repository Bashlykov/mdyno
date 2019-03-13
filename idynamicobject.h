#ifndef IDYNAMICOBJECT_H
#define IDYNAMICOBJECT_H
#include <QtWidgets>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <QVector>
#include <QRect>
#include <QIODevice>
#include <QDataStream>

#include <common.h>
#include <vpaintline.h>
#include <vpaintmultiline.h>
#include <vpaintpolygon.h>
#include <varea.h>
#include <vroute.h>

class IDynamicObject : public QGraphicsObject
{
    Q_OBJECT

    QVector<QPointF> route;
    QRectF _bound;

public:
    IDynamicObject(){}
    IDynamicObject(UIDType uid);
    ~IDynamicObject() override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual void setParentShape(IDynamicObject *parent) = 0;
    virtual void setListThisObjects(QList<IDynamicObject*> *list) = 0;
    virtual QList<IDynamicObject*> *getListThisObjects() = 0;
    virtual void createChild(VArea *area) = 0;
    virtual IDynamicObject *getCurrentChild() = 0;
    virtual void setCurrentChild(IDynamicObject *currentChild) = 0;
    virtual QList<IDynamicObject *> *getListChild() = 0;
    virtual QList<IDynamicObject *> *getListChildFromNet() = 0;
    virtual void setVArea(VArea *area) = 0;
    virtual VArea *getVArea() = 0;
    virtual void calcRoute() = 0;
    virtual uint getCountChild() = 0;
    virtual QByteArray serialize() = 0;
    virtual void deserialize(QByteArray &ba) = 0;

    virtual VRoute* getRoute() = 0;
    virtual QList<VRoute*> *getListVRouteChild() = 0;

    void setPause();
    void setUID(UIDType uid);
    UIDType getUID();

    void set_bound(QRectF _bound);
    QRectF get_bound();
    QVector<QPointF> get_route();
    void set_route(QVector<QPointF> route);

    void setTypeObj(TypeObject type_object);
    TypeObject getTypeObj();

    bool f_end_route;
    bool f_route_created;
    bool f_from_net;

protected:
    void advance(int) override;
    QGraphicsScene* vScene;
    TypeObject type_object;
    UIDType uid;

public slots:
    virtual void removeThis(bool);
    virtual void triggeredSetupRouteAction(){}
    virtual void removeData(){}
    virtual void createRouteChild(){}
    virtual void createdRoute(bool){}
    virtual void getProper(bool){}
    virtual void setStartPosition(){}
    void setCoords(QPointF &p);

signals:
    void endRoute();
    void clickedShape(IDynamicObject*);
    void runRouteAction();

};

#endif // IDYNAMICOBJECT_H
