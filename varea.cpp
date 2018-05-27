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
//
    r = new VPaintPolygon;
    paint_lines = new QVector<VPaintLine*>;
    dron_points = new QVector<QPointF>;
//
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

            //*
            // All the lines of a polygon are recorded in a array
            QVector<QLineF> *poly_lines = new QVector<QLineF>;

            for(int i=0; i < paintPoly->getPoly().length(); i++)
            {
                QLineF line(paintPoly->getPoly().at(i),
                                paintPoly->getPoly().
                                       at(i+1 == paintPoly->getPoly().length() ? 0 : i+1));
                poly_lines->append(line);
            }

            // Search max length of line from polygon for min square bounding rectangle
            QLineF max_poly_line = poly_lines->at(0);
            float max_len = max_poly_line.length();

            for(int i=1; i < poly_lines->length(); i++)
            {
                if(max_len < poly_lines->at(i).length())
                {
                    max_poly_line = poly_lines->at(i);
                    max_len = max_poly_line.length();
                }
            }

            // Rotate the polygon so that the line with the maximum length had an angle of 0 degrees
            float angle_base = 360.0;
            float angle_from = max_poly_line.angle();
            float angle_to = angle_base - angle_from;

            QMatrix matrix;
            matrix.rotate(angle_from);
            paintPoly->getPoly() = matrix.map(paintPoly->getPoly());


            if(vScene->items().contains(r))
                vScene->removeItem(r);

            // Rectangle bounding of polygon
            QPointF topLeft(getPolygonPaint()->boundingRect().topLeft());
            QPointF topRight(getPolygonPaint()->boundingRect().topRight());
            QPointF bottomRight(getPolygonPaint()->boundingRect().bottomRight());
            QPointF bottomLeft(getPolygonPaint()->boundingRect().bottomLeft());
            v = new QVector<QPointF>;
            *v          << topLeft
                        << topRight
                        << bottomRight
                        << bottomLeft;
            // for debug
            r = new VPaintPolygon(v, new QColor(125, 55, 99, 15));
            vScene->addItem(r);
            //

            QLineF top_line(topLeft, topRight);// top line
            QLineF bottom_line(bottomLeft, bottomRight);// bottom line

            int width = 15; // width between lines
            float num = top_line.length() / width; // num of lines

            // Create vertical lines from the top line to the bottom line
            QVector<QLineF*> *lines_not_clip = new QVector<QLineF*>;
            QPointF point1(top_line.p1().x() + width, top_line.p1().y());
            QPointF point2(bottom_line.p1().x() + width, bottom_line.p1().y());
            QLineF *line_prev = new QLineF(point1, point2);
            lines_not_clip->append(line_prev);

            for(int i=0; i < num; i++)
            {
                QPointF point1(line_prev->p2().x() + width, line_prev->p2().y());
                QPointF point2(line_prev->p1().x() + width, line_prev->p1().y());
                QLineF *line = new QLineF(point1, point2);
                lines_not_clip->append(line);
                line_prev = line;
            }

            // Clipping lines
            QVector<QLineF*> *lines_clip = clipping_lines(lines_not_clip,
                                                          getPolygonPaint()->getPoly());

            for(int i=0; i<lines_clip->size()-1; i++)
            {
                qDebug() << "p1 y" << lines_clip->at(i)->p1().y()
                << "p2 y" << lines_clip->at(i)->p2().y();
            }

           // Remove lines from scene
           if ( paint_lines->length() > 0 )
           {
               for( int i=0; i < paint_lines->length(); i++)
                    vScene->removeItem(paint_lines->at(i));
               paint_lines->clear();
           }

           if ( dron_points->length() > 0 )
               dron_points->clear();

            // Connection of lines with Bezier curves
            connect_points_bezier(dron_points, lines_clip);

            // Rotate the polygon and all lines to the original angle
            matrix.reset();
            matrix.rotate(angle_to);
            paintPoly->getPoly() = matrix.map(paintPoly->getPoly());
            paintPoly->setPoly(paintPoly->getPoly());

            path_dron = new QPolygonF(*dron_points);
            *path_dron = matrix.map(*path_dron);


            // Add lines to the scene
            for(int i=0; i<path_dron->size()-1; i++)
            {
                VPaintLine *line = new VPaintLine(path_dron->at(i),
                                                    path_dron->at(i+1));
                paint_lines->append(line);
                vScene->addItem(line);
            }

            vScene->update();
 //*/
    }
}

void connect_points_bezier(QVector<QPointF> *dron_points,
                            QVector<QLineF*> *lines_clip)
{
    int len = lines_clip->length();
    int curve_height = 10;

    dron_points->append(lines_clip->at(0)->p1());
    dron_points->append(lines_clip->at(0)->p2());

    for(int i=0; i < len-1; i+=1)
    {
        QPointF *p1_current;
        QPointF *p2_current;
        QPointF *p1_next;

        if( i % 2 == 0 )
        {
            p1_current = &lines_clip->at(i)->p1();
            p1_next = &lines_clip->at(i+1)->p1();
            p2_current = &lines_clip->at(i)->p2();
        }
        else
        {
            p1_current = &lines_clip->at(i)->p2();
            p1_next = &lines_clip->at(i+1)->p2();
            p2_current = &lines_clip->at(i)->p1();
        }

        QVector2D p0(*p1_current);

        QVector2D p1(p1_current->x(),
                     (p1_current->y() < p2_current->y() ?
                        p1_current->y() - curve_height :
                        p1_current->y() + curve_height  ));

        QVector2D p2(p1_next->x(),
                     (p1_next->y() < p2_current->y() ?
                        p1_next->y() - curve_height :
                        p1_next->y() + curve_height  ));

        QVector2D p3(*p1_next);


        QVector<QLineF> *lines_bezier = drawBezier(p0, p1, p2, p3);
        for(int i2=0; i2 < lines_bezier->size(); i2++)
        {
            dron_points->append(lines_bezier->at(i2).p1());
            dron_points->append(lines_bezier->at(i2).p2());
        }
    }

    dron_points->append(lines_clip->at(len-1)->p1());
    dron_points->append(lines_clip->at(len-1)->p2());
}

// Clipping lines over polygon
QVector<QLineF*> *clipping_lines(QVector<QLineF*> *lines_not_clip, QPolygonF &polygon)
{
    QVector<QLineF*> *lines_clip = new QVector<QLineF*>;
    for( int j=0; j < lines_not_clip->length(); j++)
    {
         int count_points = 0;
         int len = polygon.length();

         for(int i=0; i < len; i++)
         {
             QLineF line_pol(polygon.at(i),
                             polygon.at(i+1 == len ? 0 : i+1));

             QPointF *point1;
             QPointF *point2;
             QPointF *point_intersect = new QPointF;

             if (QLineF::BoundedIntersection
                     == lines_not_clip->at(j)->intersect(line_pol, point_intersect))
             {
                 ++count_points;

                 if (count_points == 1)
                 {
                     point1 = new QPointF(*point_intersect);
                 }
                 else
                 if (count_points == 2)
                 {
                     QLineF *line_clip = new QLineF;
                     point2 = new QPointF(*point_intersect);

                     if ( point1->y() < point_intersect->y())
                     {
                         line_clip->setP1(*point1);
                         line_clip->setP2(*point2);
                     }
                     else
                     {
                         line_clip->setP1(*point2);
                         line_clip->setP2(*point1);
                     }

                     lines_clip->append(line_clip);
                     count_points = 0;
                     break;
                 }
             }
         }
     }

    return lines_clip;
}

QVector<QLineF> *drawBezier(QVector2D &p0, QVector2D &p1, QVector2D &p2, QVector2D &p3)
{
    QVector<QLineF> *lines = new QVector<QLineF>;
    const char SEGMENT_COUNT = 9;
    QVector2D q0;
    QVector2D q1;

    q0 = calcBezierPoint(0, p0, p1, p2, p3);

    for( float i = SEGMENT_COUNT; i >= 1; i -= 0.1f )
    {
        float t = 1 / i;
        q1 = calcBezierPoint(t, p0, p1, p2, p3);

        if ( (q1-q0).length() >= 1 )
        {
            QLineF line(QPointF(q0.x(), q0.y()),
                        QPointF(q1.x(), q1.y()));

            lines->append(line);

            q0 = q1;
        }
    }

    return lines;
}

QVector2D &calcBezierPoint(float t, QVector2D &p0, QVector2D &p1, QVector2D &p2, QVector2D &p3)
{
    float    u = 1 - t,
             tt = t*t,
             uu = u*u,
             uuu = uu * u,
             ttt = tt * t;

    QVector2D p = uuu * p0;
    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;
    p += ttt* p3;

    return p;
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
