#include "varea.h"

VArea::VArea():
    points( new QVector<QPointF> ),
    paintPoints( new VPointPaintArray ),
    paintPoly( new VPaintPolygon ),
    listThisObjects(nullptr),
    f_create_poly( false ),
    f_lines( false )
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
    points->clear();
    paintPoints->clear();
    delete points;
    delete paintPoints;
    delete paintPoly;
}

void VArea::setPointPaint(VPaintPoint *point)
{
    paintPoints->append(point);
    points->append( QPointF(point->x(), point->y()) );
}

void VArea::createPoint(QPointF &p)
{
    VPaintPoint *newPoint = new VPaintPoint(QRect(-4, -4, 8, 8),
                                            true, 
                                            Qt::black, 
                                            Qt::black,  
                                            paintPoints->size());

    newPoint->setPos(p);

    vScene->addItem(newPoint);
    paintPoints->append(newPoint);
    points->append(newPoint->pos());

    connect(newPoint, SIGNAL(setNewPos(QPointF)), this, SLOT(setPoly(QPointF)));
    connect(newPoint, SIGNAL(deletingThis(int)), this, SLOT(deletePoint(int)));

    emit createPointAreaSig(newPoint);

    if ( points->size() > 1 )
    {
        setPoly(p);
    }
}

void VArea::deletePoint(int num)
{
    for(int i=0; i<paintPoints->size(); i++)
    {
        if (paintPoints->at(i)->getNum() == num)
        {
            vScene->removeItem(paintPoints->at(i));
            paintPoints->removeAll(paintPoints->at(i));
            points->removeAt(i);
            setPoly(QPointF());
        }
    }

}

QVector<QPointF> *VArea::getPoints()
{
    return this->points;
}

void VArea::removePoly()
{
    if ( f_create_poly )
    {
        for( int i = 0; i < paintPoints->size(); i++ )
        {
            if (vScene->items().contains(paintPoints->at(i)))
            {
                vScene->removeItem(paintPoints->at(i));
            }
        }
        if (vScene->items().contains(paintPoly))
        {
            vScene->removeItem(paintPoly);
        }
        paintPoints->clear();
        points->clear();
        f_create_poly = false;
    }
}

void VArea::removeThis()
{
    removePoly();
    listThisObjects->removeAll(this);
}

VPointPaintArray *VArea::getPointsPaint()
{
    return this->paintPoints;
}

void VArea::setPolygon(QPolygonF *poly)
{
    this->poly = poly;
}

void VArea::setPoly(QPointF)
{
    points->clear();

    for( int i = 0; i < paintPoints->size(); i++ )
    {
        points->append(paintPoints->at(i)->pos());
    }

    if ( points->size() > 1 )
    {
        if ( points->size() == 2 )
        {
            if ( f_create_poly )
            {
                if (vScene->items().contains(paintPoly))
                {
                    vScene->removeItem(paintPoly);
                }
            }
            if ( f_lines )
            {
                if (vScene->items().contains(paintLines))
                {
                    vScene->removeItem(paintLines);
                }
            }
            paintLines = new VPaintMultiLine(points,
                                            new QBrush(Qt::black),
                                            3,
                                            Qt::DashLine);

            vScene->addItem(paintLines);
            f_lines = true;
        }

        if ( points->size() > 2 )
        {
            if ( f_lines )
            {
                if (vScene->items().contains(paintLines))
                {
                    vScene->removeItem(paintLines);
                }
            }

            if ( f_create_poly )
            {
                if (vScene->items().contains(paintPoly))
                {
                    vScene->removeItem(paintPoly);
                }
            }

            paintPoly = new VPaintPolygon(points,
                                         new QColor(Qt::transparent), 
                                         3, 
                                         Qt::DashLine);
///TODO
            paintPoly->setUID(uid);

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
            vScene->addItem(paintPoly);
            f_create_poly = true;
        }
    }
}

void VArea::createPoly()
{

    if (points->size() > 2)
    {
        if (f_create_poly)
        {
            paintPoly->setColorPen(new QColor(Qt::black));
        }

        f_create_poly = true;
        emit savePoly();
    }
}

void VArea::create(QPointF &pos)
{
    if ( paintPoints->isEmpty() )
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
    this->paintPoints = pointsPaintArea;
}


void VArea::setDefaultColor()
{
    for ( int i = 0; i < paintPoints->size(); i++ )
    {
        paintPoints->at(i)->setDefaultColor();
    }
}

void VArea::setVScene(QGraphicsScene *vScene)
{
    this->vScene = vScene;
}

VPaintPolygon *VArea::getPolygonPaint()
{
    return paintPoly;
}

QDataStream& operator>>( QDataStream& stream, VArea& area )
{
    stream  >> *area.points;
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
                                                 paintPoints->size());
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

     paintPoly = new VPaintPolygon(pointsArea,
                                  new QColor(Qt::transparent), 
                                  3, 
                                  Qt::DashLine);
     paintPoly->setUID(uid);
     vScene->addItem(paintPoly);

    f_create_poly = true;
}
