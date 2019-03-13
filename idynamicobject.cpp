#include "idynamicobject.h"

IDynamicObject::IDynamicObject(UIDType id) :

            f_end_route(false),
            f_route_created(false),
            f_from_net(false),
            uid(id)
{
}

void IDynamicObject::setUID(UIDType uid)
{
    this->uid = uid;
}

UIDType IDynamicObject::getUID()
{
    return uid;
}

void IDynamicObject::removeThis(bool)
{
    scene()->removeItem(this);
}

IDynamicObject::~IDynamicObject(){}

QRectF IDynamicObject::boundingRect() const
{
    return QRectF(-24, -12, 44, 24);
}

QPainterPath IDynamicObject::shape() const
{
    QPainterPath path;
    path.addRect(-10, -10, 20, 20);
    return path;
}

void IDynamicObject::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void IDynamicObject::advance(int)
{

}

void IDynamicObject::setCoords(QPointF &p)
{
    setPos(p);
}

void IDynamicObject::setPause()
{

}

void IDynamicObject::set_bound(QRectF _bound)
{
    this->_bound = _bound;
}

QRectF IDynamicObject::get_bound()
{
    return _bound;
}

void IDynamicObject::set_route(QVector<QPointF> route)
{
    this->route = QVector<QPointF>(route);
}

QVector<QPointF> IDynamicObject::get_route()
{
    return route;
}

void IDynamicObject::setTypeObj(TypeObject type_object)
{
    this->type_object = type_object;
}

TypeObject  IDynamicObject::getTypeObj()
{
    return type_object;
}
