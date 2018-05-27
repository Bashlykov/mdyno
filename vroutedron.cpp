#include "vroutedron.h"

VRouteDron::VRouteDron():
    paintPointsRoute( new VPointPaintArray ),
    pointsRoute( new QVector<QPointF> ),
    f_createRoute( false )
{

}

VRouteDron::~VRouteDron()
{
    paintPointsRoute->clear();
    pointsRoute->clear();
    delete paintPointsRoute;
    delete pointsRoute;
}

VPointPaintArray *VRouteDron::getPaintPoints()
{
    return paintPointsRoute;
}

QVector<QPointF> *VRouteDron::getPoints()
{
    return this->pointsRoute;
}

void VRouteDron::_create()
{
    bool buildTrajectory;

    try
    {
        buildTrajectory = true;
    }
    catch (std::exception &e)
    {
        qDebug() << e.what();
        msgExept = QString(e.what());
        msgWarning();
        emit except();
        return;
    }

    finish();
    emit finish();
    emit except(); //for testing
}

QString &VRouteDron::getMsgExcept()
{
    return msgExept;
}

void VRouteDron::finish()
{
     if ( f_createRoute )
        return;

///TODO
    for ( double i = 0; i < 1; i++ )
    {
        VPaintPoint *newPoint = new VPaintPoint(QRect(-1, -1, 1, 1),
                                                false,
                                                Qt::yellow,
                                                Qt::yellow,
                                                i);

        paintPointsRoute->append(newPoint);
        /// TODO newPoint->setPos(sx, sy);
        pointsRoute->append(newPoint->pos());

        vScene->addItem(newPoint);
    }

    lineRoute = new VPaintMultiLine(pointsRoute,
                                    new QBrush(Qt::yellow),
                                    2, Qt::DotLine);
    vScene->addItem(lineRoute);

    emit created(true);

    f_createRoute = true;

}

void VRouteDron::msgWarning()
{
    QString strMsg = "Невозможно построить маршрут!\n" + getMsgExcept();
    QMessageBox msg;
    msg.warning(0, "Внимание!", strMsg);
}

void VRouteDron::setArea(VArea *varea)
{
    this->varea = varea;
}

void VRouteDron::create()
{
    remove();
    _create();
}

void VRouteDron::createFromNet(QVector<QPointF> *pointsRouteDron, UIDType)
{
    this->pointsRoute = pointsRouteDron;
    if(vScene->items().contains(lineRoute))
        vScene->removeItem(lineRoute);
    lineRoute = new VPaintMultiLine(pointsRouteDron,                                    
                                    new QBrush(Qt::yellow),
                                    2, Qt::DotLine);
    vScene->addItem(lineRoute);
}


void VRouteDron::remove()
{
    try
    {
        if ( f_createRoute )
        {
            for( int i = 0; i < paintPointsRoute->size(); i++ )
            {
                vScene->removeItem(paintPointsRoute->at(i));
            }
            vScene->removeItem(lineRoute);
            paintPointsRoute->clear();
            pointsRoute->clear();
            f_createRoute = false;
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

void VRouteDron::setPaintPoints(VPaintPoint *point)
{
    this->paintPointsRoute->append(point);
    this->pointsRoute->append( QPointF(point->x(), point->y()) );
}

void VRouteDron::setVScene(QGraphicsScene *vScene)
{
    this->vScene = vScene;
}
