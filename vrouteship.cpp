#include "vrouteship.h"

VRouteShip::VRouteShip() :
    pointsRoute( new QVector<QPointF>),
    pointsPaintRoute (new VPointPaintArray),
    linesRouteShip(nullptr),
    f_createLineRoute ( false),
    f_linesRoute ( false),   
    routeBuilded ( false),
    speed( 5.0f)
{

}

VRouteShip::~VRouteShip()
{
    pointsRoute->clear();
    pointsPaintRoute->clear();
    delete pointsRoute;
    delete pointsPaintRoute;
    delete linesRouteShip;
}

void VRouteShip::setSpeed(float speedShip)
{
    this->speed = speedShip;
}

void VRouteShip::addPointFromCursor(QPointF &pos)
{
    VPaintPoint *newPoint = new VPaintPoint( QRect(-4, -4, 8, 8), true,
                                    Qt::red, Qt::red,
                                             pointsPaintRoute->size());

    newPoint->setPos(pos);

    vScene->addItem(newPoint);

    pointsPaintRoute->append(newPoint);
    pointsRoute->append(pos);

    connect(newPoint, SIGNAL(setNewPos(QPointF)), this, SLOT(set(QPointF)));
    connect(newPoint, SIGNAL(numPoint(int)), this, SLOT(setNumPointClick(int)));

    emit createPoint(newPoint);

    if ( pointsRoute->size() > 1 )
    {
        set(pos);
    }

    if (pointsPaintRoute->size() == 1)
    {
        emit addFirstPoint(pos);
    }
    else if (pointsPaintRoute->size() == 2)
    {
        emit addedSecondPoint(true);
    }
}

void VRouteShip::setVScene(QGraphicsScene *vScene)
{
    this->vScene = vScene;
}


void VRouteShip::setNumPointClick(int num)
{
    for ( int i = 0; i < pointsPaintRoute->size(); i++ )
    {
        if ( num != pointsPaintRoute->at(i)->getNum() )
        {
            pointsPaintRoute->at(i)->setDefaultColor();
        }
    }
}

void VRouteShip::setDefaultColor()
{
    for ( int i = 0; i < pointsPaintRoute->size(); i++ )
    {
        pointsPaintRoute->at(i)->setDefaultColor();
    }
}


void VRouteShip::setPaintPoints(VPaintPoint *point, double speed)
{
    this->pointsPaintRoute->append(point);
    this->pointsRoute->append( QPointF(point->x(), point->y()) );
}


VPaintMultiLine *VRouteShip::getLine()
{
    return this->linesRouteShip;
}

QVector<QPointF> *VRouteShip::getPoints()
{
    return this->pointsRoute;
}

void VRouteShip::remove()
{
    try
    {
        if ( f_createLineRoute )
        {
            for( int i = 0; i < pointsPaintRoute->size(); i++ )
            {
                vScene->removeItem(pointsPaintRoute->at(i));
            }
            vScene->removeItem(linesRouteShip);
            pointsPaintRoute->clear();
            pointsRoute->clear();

            f_createLineRoute = false;
        }
    }
    catch(std::exception &exept)
    {
        qDebug() << "exception" << exept.what();
        QString strMsg = "Exception! " + QString(exept.what());
        QMessageBox msg;
        msg.warning(0, "Warning!", strMsg);
    }
}

void VRouteShip::create()
{

    if ( speed <= 0 )
        return;

    if (pointsRoute->size() > 1)
    {
        linesRouteShip->setUID(uid);
        routeBuilded = true;
        emit created(true);
    }
}

VPointPaintArray *VRouteShip::getPaintPoints()
{
    return pointsPaintRoute;
}

void VRouteShip::set(QPointF)
{
    try
    {
        pointsRoute->clear();

        for( int i = 0; i < pointsPaintRoute->size(); i++ )
        {
            pointsRoute->append(pointsPaintRoute->at(i)->pos());
        }

        if ( f_createLineRoute )
            vScene->removeItem(linesRouteShip);

        linesRouteShip = new VPaintMultiLine(pointsRoute,
                                             new QBrush(Qt::red), 2, Qt::DotLine);

        vScene->addItem(linesRouteShip);
        f_createLineRoute = true;
    }
    catch(std::exception &exept)
    {
        qDebug() << "exception" << exept.what();
        QString strMsg = "Exception! " + QString(exept.what());
        QMessageBox msg;
        msg.warning(0, "Warning!", strMsg);
    }
}

void VRouteShip::createFromNet(QVector<QPointF> *points, UIDType uid)
{
    this->pointsRoute = points;

    for(QPointF p: *pointsRoute)
    {
            VPaintPoint *point = new VPaintPoint(QRect(-4, -4, 8, 8),
                                             false,
                                             Qt::red,
                                             Qt::red
                                             );

            vScene->addItem(point);
            point->setPos(p);
    }

    for(QGraphicsItem *item: vScene->items())
    {
        if (VPaintMultiLine* poly = dynamic_cast<VPaintMultiLine*>(item ) )
        {
            if(poly->getUID() == uid)
           {
                vScene->removeItem(item);
                break;
           }
        }
    }

    linesRouteShip = new VPaintMultiLine(points,
                                         new QBrush(Qt::red),
                                         2, Qt::DotLine);
    linesRouteShip->setUID(uid);

    vScene->addItem(linesRouteShip);
    f_createLineRoute = true;
}
