#include "vrouteship.h"

VRouteShip::VRouteShip() :

    paintPoints (new VPointPaintArray),
    points( new QVector<QPointF>),
    paintLines(nullptr),
    f_create_line ( false),
    //f_lines ( false),
    speed( 5.0f),
    route_builded ( false)
{

}

VRouteShip::~VRouteShip()
{
    points->clear();
    paintPoints->clear();
    delete points;
    delete paintPoints;
    delete paintLines;
}

void VRouteShip::addPointFromCursor(QPointF &pos)
{
    VPaintPoint *newPoint = new VPaintPoint( QRect(-4, -4, 8, 8),
                                             true,
                                             Qt::red, Qt::red,
                                             paintPoints->size());

    newPoint->setPos(pos);
    vScene->addItem(newPoint);
    paintPoints->append(newPoint);
    points->append(pos);

    connect(newPoint, SIGNAL(setNewPos(QPointF)), this, SLOT(set(QPointF)));
    connect(newPoint, SIGNAL(numPoint(int)), this, SLOT(setNumPointClick(int)));
    connect(newPoint, SIGNAL(deletingThis(int)), this, SLOT(deletePoint(int)));

    emit createPoint(newPoint);

    if ( points->size() > 1 )
    {
        set(pos);
    }

    if (paintPoints->size() == 1)
    {
        emit addFirstPoint(pos);
    }
    else if (paintPoints->size() == 2)
    {
        emit addedSecondPoint(true);
    }
}

void VRouteShip::deletePoint(int num)
{
    for(int i=0; i<paintPoints->size(); i++)
    {
        if (paintPoints->at(i)->getNum() == num)
        {
            vScene->removeItem(paintPoints->at(i));
            paintPoints->removeAll(paintPoints->at(i));
            points->removeAt(i);
            set(QPointF());
        }
    }
}

void VRouteShip::setVScene(QGraphicsScene *vScene)
{
    this->vScene = vScene;
}


void VRouteShip::setNumPointClick(int num)
{
    for ( int i = 0; i < paintPoints->size(); i++ )
    {
        if ( num != paintPoints->at(i)->getNum() )
        {
            paintPoints->at(i)->setDefaultColor();
        }
    }
}

void VRouteShip::setDefaultColor()
{
    for ( int i = 0; i < paintPoints->size(); i++ )
    {
        paintPoints->at(i)->setDefaultColor();
    }
}


void VRouteShip::setPaintPoints(VPaintPoint *point)
{
    this->paintPoints->append(point);
    this->points->append( QPointF(point->x(), point->y()) );
}


VPaintMultiLine *VRouteShip::getLine()
{
    return this->paintLines;
}

QVector<QPointF> *VRouteShip::getPoints()
{
    return this->points;
}

void VRouteShip::remove()
{
    try
    {
        if ( f_create_line )
        {
            for( int i = 0; i < paintPoints->size(); i++ )
            {
                vScene->removeItem(paintPoints->at(i));
            }
            vScene->removeItem(paintLines);
            paintPoints->clear();
            points->clear();

            f_create_line = false;
        }
    }
    catch(std::exception &exept)
    {
        qDebug() << "exception" << exept.what();
        QString strMsg = "Exception! " + QString(exept.what());
        QMessageBox msg;
        msg.warning(dynamic_cast<QWidget*>(this), "Warning!", strMsg);
    }
}

void VRouteShip::create()
{

    if ( speed <= 0 )
        return;

    if (points->size() > 1)
    {
        paintLines->setUID(uid);
        route_builded = true;
        emit created(true);
    }
}

VPointPaintArray *VRouteShip::getPaintPoints()
{
    return paintPoints;
}

void VRouteShip::set(QPointF)
{
    try
    {
        points->clear();

        for( int i = 0; i < paintPoints->size(); i++ )
        {
            points->append(paintPoints->at(i)->pos());
        }

        if ( f_create_line )
            vScene->removeItem(paintLines);

        paintLines = new VPaintMultiLine(points,
                                         new QBrush(Qt::red),
                                         2, Qt::DotLine);

        vScene->addItem(paintLines);
        f_create_line = true;
    }
    catch(std::exception &exept)
    {
        qDebug() << "exception" << exept.what();
        QString strMsg = "Exception! " + QString(exept.what());
        QMessageBox msg;
        msg.warning(dynamic_cast<QWidget*>(this), "Warning!", strMsg);
    }
}

void VRouteShip::createFromNet(QVector<QPointF> *points, UIDType uid)
{
    this->points = points;

    for(QPointF p: *points)
    {
            VPaintPoint *point = new VPaintPoint(QRect(-4, -4, 8, 8),
                                             false,
                                             Qt::red,
                                             Qt::red);

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

    paintLines = new VPaintMultiLine(points,
                                         new QBrush(Qt::red),
                                         2, Qt::DotLine);
    paintLines->setUID(uid);

    vScene->addItem(paintLines);
    f_create_line = true;
}
