#include "dynamicdron.h"

DynamicDron::DynamicDron(UIDType id) :
                        IDynamicObject(id),
                        vls(new QVector<QLineF>),
                        routeDron(new VRouteDron),
                        parentShape(nullptr),                                               
                        area(nullptr)
{
    *vls    << QLineF(-6, -10, 6, 10)
                << QLineF(-6, 10, 6, -10)
                << QLineF(0, 0, -20, 0);

    setFlags(ItemIsMovable | ItemIgnoresTransformations);
    setAcceptHoverEvents(true);    

    propRouteAction  = menu.addAction("Свойства");
    setupRouteAction = menu.addAction("Построить траекторию");
    removeAction = menu.addAction("Удалить");

    connect(propRouteAction, SIGNAL(triggered(bool)), this, SLOT(getProper(bool)));
    connect(setupRouteAction, SIGNAL(triggered(bool)), this, SLOT(triggeredSetupRouteAction()));
    connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(removeThis(bool)));
    connect(this, SIGNAL(endRoute()), this, SLOT(setStartPosition()));
}

DynamicDron::~DynamicDron()
{

}

void DynamicDron::setVArea(VArea *area)
{
    this->area = area;
    this->area->setVScene(scene());
    routeDron->setArea(area);
    routeDron->setVScene(scene());
}

VArea *DynamicDron::getVArea()
{
    return area;
}

void DynamicDron::setStartPosition()
{
    setPos(parentShape->get_route().first());
}

void DynamicDron::getProper(bool)
{
    QMessageBox msg;
    msg.information(nullptr, QString("Свойства"),
                    QString("Id БПЛА: %1\nId Судна: %2").arg(getUID()).arg(parentShape->getUID()),
                    nullptr, nullptr);
}

void DynamicDron::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen shipPen(Qt::green, 2, Qt::SolidLine);
    shipPen.setCosmetic(true);

    p->setPen(shipPen);
    p->setBrush(Qt::transparent);
    p->drawLines(*vls);
}

void DynamicDron::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clickedShape(this);
    qDebug() << "dron id" << getUID();
}

void DynamicDron::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    menu.exec(event->screenPos());
}

void DynamicDron::triggeredSetupRouteAction()
{
    if ( !parentShape->f_route_created )
    {
        QMessageBox msg;
        msg.warning(dynamic_cast<QWidget*>(this),
                    QString("Внимание!"),
                    QString("Для построения траектории БПЛА нужно сохранить траекторию судна!"),
                    QMessageBox::Ok
                    );
        return;
    }

    routeDron->create();
}

void DynamicDron::removeThis(bool)
{
    scene()->removeItem(this);
    routeDron->remove();
}

void DynamicDron::setParentShape(IDynamicObject *parent)
{
    this->parentShape = parent;
}

//Для передачи данных по сети
QByteArray DynamicDron::serialize()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);
    out << *this;

    return ba;
}

void DynamicDron::deserialize(QByteArray& ba)
{
    QDataStream in(&ba, QIODevice::ReadOnly);
    in >> *this;
}

QDataStream& operator>>( QDataStream& stream, DynamicDron& dron )
{
    UIDType uid_dron;
    QPointF pos(0.0, 0.0);
    double angle = 0.0;
    UIDType uid_area;
    VArea *area = new VArea;
    QVector<QPointF> pointsArea;
    QVector<QPointF> pointsRouteDron;

    stream >> uid_dron;
    stream >> pos;
    stream >> angle;
    stream >> uid_area;
    stream >> pointsArea;
    stream >> pointsRouteDron;

    //uid_dron устанавливается при создании в менедежере
    dron.setPos(pos);
    dron.setRotation(angle);
    dron.setVArea(area);
    dron.getVArea()->createPolyFromNet(&pointsArea, uid_area);
    dron.getRoute()->createFromNet(&pointsRouteDron, uid_area);

    return stream;
}

QDataStream& operator<<( QDataStream& stream, DynamicDron& dron )
{
    stream << dron.getUID();
    stream << dron.pos();
    stream << dron.rotation();
    //stream << dron.getVArea()->getPolyArea()->getUID();
    stream << *dron.getVArea()->getPoints();
    stream << *dron.getRoute()->getPoints();
    return stream;
}
