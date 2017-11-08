#include "dynamicship.h"

DynamicShip::DynamicShip(UIDType uid) :
            IDynamicObject(uid),
            vps(new QVector<QPointF>),
            vRoute(new VRouteShip),
            listChild(new QList<DynamicDron*>),
            listChildFromNet(new QList<DynamicDron*>),
            listThisObjects(new QList<DynamicShip*>),

            f_created_child(false),
            countChild(0),
            currentIdChild(-1),
            i_p(0)

{
    *vps    << QPointF(-10, -7)
                 << QPointF(-10, 7)
                 << QPointF(10, 7)
                 << QPointF(20, 0)
                 << QPointF(10, -7);

    vRoute->setUID(uid);

    setFlags(ItemIsMovable | ItemIgnoresTransformations);
    setAcceptHoverEvents(true); 

    propRouteAction  = menu.addAction("Свойства");
    setupRouteAction = menu.addAction("Задать траекторию");
    saveRouteAction = menu.addAction("Сохранить траекторию");
    removeAction = menu.addAction("Удалить");

    connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(removeThis(bool)));
    connect(setupRouteAction, SIGNAL(triggered(bool)), this, SIGNAL(runRouteAction()));
    connect(saveRouteAction, SIGNAL(triggered(bool)), vRoute, SLOT(create()));
 
    connect(vRoute, SIGNAL(addFirstPoint(QPointF&)), this, SLOT(setCoords(QPointF&)));
    connect(vRoute, SIGNAL(created(bool)), this, SLOT(createdRoute(bool)));
    connect(this, SIGNAL(endRoute()), this, SLOT(setStartPosition()));
}

DynamicShip::~DynamicShip()
{
    vps->clear();
    listChild->clear();
    listChildFromNet->clear();
    listThisObjects->clear();
    delete vRoute;
    delete vps;
    delete listChild;
    delete listChildFromNet;
    delete listThisObjects;
}

void DynamicShip::calcRoute()
{
    vect2D v1, v2;
    steps.x.clear();
    steps.y.clear();
    steps.angle.clear();
    for(int i=0; i<vRoute->getPoints()->length()-1; i++)
    {
        const QPointF p1 = vRoute->getPoints()->at(i);
        const QPointF p2 = vRoute->getPoints()->at(i+1);
        v1.x = p1.x();
        v1.y = 0.0;
        v2.x = p2.x() - p1.x();
        v2.y = p2.y() - p1.y();

        double L = v2.length();
        double step_x = v2.x / L;
        double step_y = v2.y / L;

        double dot = v1*v2;
        double det = v1.det(v2);
        double angle = atan2(det, dot);
        angle = v1.x < 0 ? angle - M_PI : angle;
        steps.angle.append(angle);

        double x = p1.x()+step_x;
        double y = p1.y()+step_y;
        steps.x.append(x);
        steps.y.append(y);

        for(int j=0; j < L; j++)
        {
            x += step_x;
            y += step_y;
            steps.x.append(x);
            steps.y.append(y);
            steps.angle.append(angle);
        }
    }
}

void DynamicShip::advance(int step)
{
    if (!step)
       return;

    if (f_end_route)
        return;

    setPos(steps.x.at(i_p), steps.y.at(i_p));

// Angle rotate of current dot
    if ( prevAngle != steps.angle.at(i_p) )
    {
        prevAngle = steps.angle.at(i_p);
        setRotation( (steps.angle.at(i_p) * 180 / M_PI) );
        qDebug() << "rotation" << rotation() << "deg";
    }

    i_p++;

    if ( i_p == steps.x.length() )
    {
        qDebug() << "finish route";
        f_end_route = true;
        emit endRoute();
        return;
    }
}

void DynamicShip::setStartPosition()
{
    QPointF startPos = vRoute->getPoints()->first();
    prevAngle = 0.0;
    i_p = 0;
    setPos(startPos);
    setRotation(0.0f);
    for( DynamicDron *dron: *listChild )
    {
        dron->setPos(startPos);
        dron->setRotation(0.0f);
        dron->show();
    }
}

void DynamicShip::removeThis(bool)
{
    for( DynamicDron *dron: *listChild )
    {
        scene()->removeItem(dron);
        dron->getRoute()->remove();
    }
    listChild->clear();

    removeData();
    scene()->removeItem(this);
    vRoute->remove();
    listThisObjects->removeAll(this);
}

void DynamicShip::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen shipPen(Qt::red, 2, Qt::SolidLine);
    shipPen.setCosmetic(true);

    p->setPen(shipPen);
    p->setBrush(Qt::transparent);
    p->drawPolygon(QPolygonF(*vps));
}

void DynamicShip::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clickedShape(this);
    qDebug() << "ship id" << getUID();
}

void DynamicShip::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    menu.exec(event->screenPos());
}

void DynamicShip::removeData()
{
    getRoute()->remove();
}


///Работа с дронами///

void DynamicShip::createRouteChild()
{
    currentChild->createRouteChild();
}

void DynamicShip::setVArea(VArea *area)
{
    this->area = area;
}

VArea *DynamicShip::getVArea()
{
    return area;
}

void DynamicShip::createChild(VArea *area)
{
    qsrand(QTime::currentTime().msec());
    UIDType uid = (qrand() % 90000 + 10000);
    DynamicDron *dron = new DynamicDron(uid);
    scene()->addItem(dron);
    dron->setVArea(area);
    dron->setParentShape(this);
    dron->setTypeObj(TypeObject::Dron);
    dron->setPos(pos());

    listChild->append(dron);
    countChild++;
    this->area = area;
}


IDynamicObject *DynamicShip::getCurrentChild()
{
    return currentChild;
}

void DynamicShip::setCurrentChild(IDynamicObject *currentChild)
{
    this->currentChild = dynamic_cast<DynamicDron*>(currentChild);
}

void DynamicShip::createdRoute(bool f_created)
{
    f_route_created = f_created;
    set_route(*getRoute()->getPoints());
}

void DynamicShip::setListThisObjects(QList<IDynamicObject*> *list)
{
    this->listThisObjects = reinterpret_cast<QList<DynamicShip*>*>(list);
}

QList<IDynamicObject*> *DynamicShip::getListThisObjects()
{
    return reinterpret_cast<QList<IDynamicObject*>*>(listThisObjects);
}

QList<IDynamicObject *> *DynamicShip::getListChild()
{
    return reinterpret_cast<QList<IDynamicObject*>*>(listChild);
}


uint DynamicShip::getCountChild()
{
    return countChild;
}

VRoute* DynamicShip::getRoute()
{
    return vRoute;
}

//Для передачи данных по сети
QByteArray DynamicShip::serialize()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);
    out << *this;

    return ba;
}

void DynamicShip::deserialize(QByteArray& ba)
{
    QDataStream in(&ba, QIODevice::ReadOnly);
    in >> *this;
}

QDataStream& operator>>( QDataStream& stream, DynamicShip& ship )
{
    int countShip;
    UIDType uid_ship;
    QPointF pos;
    double angle;
    UIDType uid_route;
    QVector<QPointF> pointsRoute;
    int countDron;

    stream >> countShip;
    stream >> uid_ship;
    stream >> pos;
    stream >> angle;
    stream >> uid_route;
    stream >> pointsRoute;
    stream >> countDron;

    //uid_ship устанавливается при создании в менедежере
    ship.setPos(pos);
    ship.setRotation(angle);
    ship.getRoute()->createFromNet(&pointsRoute, uid_route );

    for(int i = 0; i < countDron; i++)
    {
        quint64 uid;
        stream >> uid;

        auto iter = std::find_if( ship.listChildFromNet->begin(),
                                 ship.listChildFromNet->end(),
                                 [uid](DynamicDron * d){ return d->getUID() == uid; });

        DynamicDron *dron;
        if(iter == ship.listChildFromNet->end())
        {
            dron = new DynamicDron(uid);
            ship.scene()->addItem(dron);
            dron->getRoute()->setVScene(ship.scene());
            dron->setParentShape(dynamic_cast<IDynamicObject*>(&ship));
            ship.listChildFromNet->append(dron);
            dron->f_from_net = true;
        }
        else
        {
            dron = *iter;
        }
        stream >> *dron;
    }

    return stream;
}

QDataStream& operator<<( QDataStream& stream, DynamicShip& ship )
{
    stream  << ship.listThisObjects->size();
    stream  << ship.getUID();
    stream  << ship.pos();
    stream  << ship.rotation();
    stream  << ship.getRoute()->getUID();
    stream  << *ship.getRoute()->getPoints();

    stream   << ship.listChild->size();
    for(DynamicDron *dron: *ship.listChild )
    {
        stream << dron->getUID()
                      << *dron;
    }

    return stream;
}
