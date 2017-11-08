#include "varea.h"

VArea::VArea():
    pointsArea( new QVector<QPointF> ),
    pointsPaintArea( new VPointPaintArray ),
    polyArea( new VPaintPolygon ),
    listThisObjects(nullptr),
    f_createPolyArea( false ),
    f_linesArea( false ),
    f_enableEditAreaRoute( false )
{
    propertyAction  = menu.addAction("Свойства");
    addPointAction = menu.addAction("Добавить точку");
    saveAction = menu.addAction("Сохранить");
    removeAction = menu.addAction("Удалить");

    connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(removeThis()));
    connect(addPointAction, SIGNAL(triggered(bool)), this, SIGNAL(addingPoint()));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(createPoly()));

}

VArea::~VArea()
{
    pointsArea->clear();
    pointsPaintArea->clear();
    delete pointsArea;
    delete pointsPaintArea;
    delete polyArea;
}

void VArea::setPointPaint(VPaintPoint *point)
{
    pointsPaintArea->append(point);
    pointsArea->append( QPointF(point->x(), point->y()) );
}

void VArea::createPoint(QPointF &p)
{
    VPaintPoint *newPoint = new VPaintPoint(QRect(-4, -4, 8, 8),
                                            true, 
                                            Qt::black, 
                                            Qt::black,  
                                            pointsPaintArea->size());

    newPoint->setPos(p);

    vScene->addItem(newPoint);
    pointsPaintArea->append(newPoint);
    pointsArea->append(newPoint->pos());

    connect(newPoint, SIGNAL(setNewPos(QPointF)), this, SLOT(setPoly(QPointF)));

    emit createPointAreaSig(newPoint);

    if ( pointsArea->size() > 1 )
    {
        setPoly(p);
    }
}

QVector<QPointF> *VArea::getPoints()
{
    return this->pointsArea;
}

void VArea::removePoly()
{
    if ( f_createPolyArea )
    {
        for( int i = 0; i < pointsPaintArea->size(); i++ )
        {
            if (vScene->items().contains(pointsPaintArea->at(i)))
            {
                vScene->removeItem(pointsPaintArea->at(i));
            }
        }
        if (vScene->items().contains(polyArea))
        {
            vScene->removeItem(polyArea);
        }
        pointsPaintArea->clear();
        pointsArea->clear();
        f_createPolyArea = false;
    }
}

void VArea::removeThis()
{
    removePoly();
    listThisObjects->removeAll(this);
}

VPointPaintArray *VArea::getPointsPaint()
{
    return this->pointsPaintArea;
}

void VArea::setPolygon(QPolygonF *poly)
{
    this->poly = poly;
}

void VArea::setPoly(QPointF)
{
    pointsArea->clear();

    for( int i = 0; i < pointsPaintArea->size(); i++ )
    {
        pointsArea->append(pointsPaintArea->at(i)->pos());
    }

    if ( pointsArea->size() > 1 )
    {
        if ( pointsArea->size() == 2 )
        {
            if ( f_createPolyArea )
            {
                if (vScene->items().contains(polyArea))
                {
                    vScene->removeItem(polyArea);
                }
            }
            if ( f_linesArea )
            {
                if (vScene->items().contains(linesArea))
                {
                    vScene->removeItem(linesArea);
                }
            }
            linesArea = new VPaintMultiLine(pointsArea,
                                            new QBrush(Qt::black),
                                            3,
                                            Qt::DashLine);

            vScene->addItem(linesArea);
            f_linesArea = true;
        }

        if ( pointsArea->size() > 2 )
        {
            if ( f_linesArea )
            {
                if (vScene->items().contains(linesArea))
                {
                    vScene->removeItem(linesArea);
                }
            }

            if ( f_createPolyArea )
            {
                if (vScene->items().contains(polyArea))
                {
                    vScene->removeItem(polyArea);
                }
            }

            polyArea = new VPaintPolygon(pointsArea,
                                         new QColor(Qt::transparent), 
                                         3, 
                                         Qt::DashLine);
///TODO
            polyArea->setUID(uid);

            //double varea_square = fabs(countSquare());
            //double varea_max = uav->getMaxAreaSquare();
            //double varea_square_2_3 = uav->getMaxAreaSquare() * 2/3;

            
            /*
            if ( varea_square > varea_max )
            {
                //bad
                polyArea->setColorPen(new QColor(Qt::red));
            }
            else if ( varea_square >=  varea_square_2_3 && varea_square < varea_max )
            {
                //normal
                polyArea->setColorPen(new QColor(Qt::yellow));
            }
            else if ( varea_square < varea_square_2_3 )
            {
                //good
                polyArea->setColorPen(new QColor(Qt::green));
            }
            */
            vScene->addItem(polyArea);
            f_createPolyArea = true;
        }
    }
}

void VArea::createPoly()
{

    if (pointsArea->size() > 2)
    {
        if (f_createPolyArea)
        {
            polyArea->setColorPen(new QColor(Qt::black));
        }

        f_createPolyArea = true;
        emit savePoly();
    }
}

void VArea::create(QPointF &pos)
{
    if ( pointsPaintArea->isEmpty() )
    {
        pos.setX(pos.x()-5);
        pos.setY(pos.y()-5);
        createPoint(pos);
        pos.setX(pos.x()+10);
        createPoint(pos);
        pos.setY(pos.y()+10);
        createPoint(pos);
        pos.setX(pos.x()-10);
        createPoint(pos);

        setPoly(pos);

        return;
    }
}

void VArea::setPointsPaint(VPointPaintArray *pointsPaintArea)
{
    this->pointsPaintArea = pointsPaintArea;
}


void VArea::setDefaultColor()
{
    for ( int i = 0; i < pointsPaintArea->size(); i++ )
    {
        pointsPaintArea->at(i)->setDefaultColor();
    }
}

void VArea::setVScene(QGraphicsScene *vScene)
{
    this->vScene = vScene;
}

VPaintPolygon *VArea::getPolygonPaint()
{
    return polyArea;
}

QDataStream& operator>>( QDataStream& stream, VArea& area )
{
    stream  >> *area.pointsArea;
    return stream;
}

QDataStream& operator<<( QDataStream& stream, VArea& area )
{
    stream << *area.getPoints();
    return stream;
}

void VArea::createPolyFromNet(QVector<QPointF> *pointsArea, UIDType uid)
{  
     for(QPointF p: *pointsArea)
    {
        VPaintPoint *newPoint = new VPaintPoint(QRect(-4, -4, 8, 8),
                                                true,
                                                Qt::black, Qt::black,
                                                 pointsPaintArea->size());
        newPoint->setPos(p);
        vScene->addItem(newPoint);
    }


     for(QGraphicsItem *item: vScene->items())
     {
         if (VPaintPolygon* poly = dynamic_cast<VPaintPolygon*>(item ) )
         {
            if(poly->getUID() == uid)
            {
                 vScene->removeItem(item);
                 break;
            }
         }
     }

     polyArea = new VPaintPolygon(pointsArea,
                                  new QColor(Qt::transparent), 
                                  3, 
                                  Qt::DashLine);
     polyArea->setUID(uid);
     vScene->addItem(polyArea);

    f_createPolyArea = true;
}
