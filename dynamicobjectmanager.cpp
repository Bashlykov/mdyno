#include "dynamicobjectmanager.h"

DynamicObjectManager::DynamicObjectManager(Ui::ModelingDynamicObject *ui):


    listShip(new QList<DynamicShip*>),
    currentShip(nullptr),
    f_createdShip(false),
    f_startModeling(false),
    f_enableEditArea(false),
    f_enableEditRouteShip(false),
    index_area(0),
    countShip(0),
    listVArea(new QList<VArea*>),
    object_select(Area),
    ui(ui)
{
     connect(ui->addObjectButtonOk, SIGNAL(clicked()),
            this, SLOT(addObjectToMap()));
    connect(ui->objComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setCurrentIndexObject(int)));

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(startModeling()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopModeling()));
    connect(ui->fastButton, SIGNAL(clicked()), this, SLOT(fastSpeedModeling()));
    connect(ui->slowButton, SIGNAL(clicked()), this, SLOT(slowSpeedModeling()));
    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pauseModeling()));

    ///TODO
  //  connect(this, SIGNAL(notSelectPoint(int)), this, SLOT( (int)));

}

DynamicObjectManager::~DynamicObjectManager()
{
    listShip->clear();
    listVArea->clear();
    delete listShip;
    delete listVArea;
    delete currentShip;
    delete ui;
}

void DynamicObjectManager::setCurrentIndexArea(int i)
{
    this->index_area = i;
}

bool DynamicObjectManager::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        auto ev = static_cast<QGraphicsSceneMouseEvent*> (event);
        QPointF pos = ev->scenePos();

        if(ev->button() == Qt::LeftButton)
        {
            //Добавление новой точки

           if (f_enableEditArea)  //Добавить точку для района
           {
                if (listVArea->size() == 0)
                   return false;

               listVArea->at(index_area)->createPoint(pos);

           }
           else if ( f_enableEditRouteShip ) //Добавить точку для траектории
           {
               if (currentShip != nullptr)
                   if ( !currentShip->getRoute()->getFlagBuild() )
                       currentShip->getRoute()->addPointFromCursor(pos);
           }
           else
           {
               //если по клику мыши ни одна точка не выбрана,
               //то установить всем точкам изначальный цвет
              if ( listVArea->size() > 0 )
              {
                  for( int i=0; i<listVArea->size(); i++)
                  {
                      listVArea->at(i)->setDefaultColor();
                  }
              }
              if (listShip->size() >0)
              {
                  for( int i=0; i<listShip->size(); i++)
                  {
                      listShip->at(i)->getRoute()->setDefaultColor();
                  }
              }
           }
        }
        if(ev->button() == Qt::RightButton)
        {
            for( int i=0; i<listVArea->size(); i++)
            {
                if (listVArea->at(i)
                        ->getPolygonPaint()->getPoly()
                        .containsPoint(pos, Qt::OddEvenFill))
                {
                    qDebug() << "click in polygon" << listVArea->at(i)->getUID();
                    listVArea->at(i)->menu.exec(QCursor::pos());
                    index_area = i;
                }
            }

            return true;
        }
    }
    return false;
}

void DynamicObjectManager::setGraphicsView(QGraphicsView *graphView)
{
    this->graphView = graphView;
}

void DynamicObjectManager::setTimer(QTimer *t)
{
    this->timer = t;
}

void DynamicObjectManager::startModeling()
{
    if (f_startModeling)
        goto timer_start;

    for (DynamicShip *ship:
         *reinterpret_cast<QList<DynamicShip*>*>(listShip) )
    {
        ship->calcRoute();
        ship->f_end_route = false;

        for (DynamicDron *dron:
             *reinterpret_cast<QList<DynamicDron*>*>(ship->getListChild()))
        {

           dron->f_end_route = false;
        }
    }

    f_startModeling = true;
timer_start:
    timer->start(16);

}

void DynamicObjectManager::stopModeling()
{
    if ( dynamic_cast<QPushButton*>(sender()) == ui->stopButton )
    {
        for (DynamicShip *ship: *reinterpret_cast<QList<DynamicShip*>*>(listShip) )
        {
            ship->f_end_route = true;
            emit ship->endRoute();
            for (DynamicDron *dron:
                 *reinterpret_cast<QList<DynamicDron*>*>(ship->getListChild()))
            {
                dron->f_end_route = true;
                emit dron->endRoute();
            }
        }
        goto stop_timer;
    }

    for (DynamicShip *ship: *reinterpret_cast<QList<DynamicShip*>*>(listShip) )
    {
        if (!ship->f_end_route)
            return;
    }

stop_timer:
    timer->stop();
    f_startModeling = false;
}

void DynamicObjectManager::pauseModeling()
{
    for (DynamicShip *ship: *reinterpret_cast<QList<DynamicShip*>*>(listShip) )
    {
        ship->setPause();
        for (DynamicDron *dron:
             *reinterpret_cast<QList<DynamicDron*>*>(ship->getListChild()))
        {
            dron->setPause();
        }
    }
    timer->stop();
}

void DynamicObjectManager::fastSpeedModeling()
{
    timer->setInterval(timer->interval()/2);
}

void DynamicObjectManager::slowSpeedModeling()
{
    timer->setInterval(timer->interval()*2);
}

void DynamicObjectManager::setVScene(QGraphicsScene* scene)
{
    this->scene = scene;
    this->scene->installEventFilter(this);
}

void DynamicObjectManager::createShip()
{
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
    UIDType uid = (qrand() % 90000 + 10000);
    qDebug() << "ship created with id " << uid;
    DynamicShip *ship = new DynamicShip(uid);
    scene->addItem(ship);
    scene->update();
    ship->getRoute()->setVScene(scene);
    ship->setTypeObj(TypeObject::Ship);
    ship->setPos(0.0, 0.0);
    listShip->append(ship);
    ship->setListThisObjects(reinterpret_cast<QList<IDynamicObject*>*>(listShip));
    currentShip = ship;

    connect(ship, SIGNAL(endRoute()),
            this, SLOT(stopModeling()));
    connect(ship, SIGNAL(runRouteAction()),
            this, SLOT(enableEditRoute()));
    connect(ship, SIGNAL(clickedShape(IDynamicObject*)),
            this, SLOT(setCurrentShip(IDynamicObject*)));

    countShip++;
}

void DynamicObjectManager::setCurrentShip(IDynamicObject* ship)
{
    currentShip = dynamic_cast<DynamicShip*>(ship);
}

void DynamicObjectManager::createOilDerrick()
{
    QVector<QPointF> *points = new QVector<QPointF>;
    *points  << QPointF(-15, -15)
                    << QPointF(-15, 15)
                    << QPointF(15, 15)
                    << QPointF(15, -15);
    VPaintPolygon *oilDerrick = new VPaintPolygon(points, new QColor(Qt::red));
    oilDerrick->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    oilDerrick->setFlag( QGraphicsItem::ItemIgnoresTransformations );
    scene->addItem(oilDerrick);
    oilDerrick->setPos(0.0, 0.0);
}

void DynamicObjectManager::setCurrentIndexObject(int index)
{
    ui->objComboBox->setCurrentIndex(index);
    switch ( static_cast<TypeObject>(index) )
    {
        case Area:
                object_select = Area;
        break;
        case Ship:
                object_select = Ship;
        break;
        case Dron:
                object_select = Dron;
        break;
        case OilDerrick:
                object_select = OilDerrick;
        break;
    }
}

void DynamicObjectManager::addObjectToMap()
{
    switch ( object_select )
    {
        case Area:
                createVArea();
        break;
        case Ship:
                createShip();
        break;
        case Dron:
                if ( listShip->size() < 1 )
                {
                    QMessageBox msg;
                    msg.warning(dynamic_cast<QWidget*>(this), QString("Предупреждение"),
                                QString("Не создано ни одного судна!"));
                    return;
                }
                if(listVArea->size() == 0)
                {
                    QMessageBox msg;
                    msg.warning(dynamic_cast<QWidget*>(this), QString("Предупреждение"),
                                QString("Не создано ни  одного района!"));
                    return;
                }
                currentShip->createChild(listVArea->at(index_area));
        break;
        case OilDerrick:
                createOilDerrick();
        break;
    }
}

void DynamicObjectManager::enableEditRoute()
{
    f_enableEditRouteShip = true;
    f_enableEditArea = false;
    currentShip->getRoute()->setFlagBuild(false);
}

//Работа с районами

VArea *DynamicObjectManager::createVArea()
{
    VArea *varea = new VArea;
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
    UIDType uid = (qrand() % 90000 + 10000);
    varea->setUID(uid);
    qDebug() << "area id" << varea->getUID();
    varea->setVScene(scene);
    QPointF point(0.0, 0.0);
    varea->create(point);
    listVArea->append(varea);
    varea->setListThisObjects(listVArea);
    connect(varea, SIGNAL(addingPoint()), this, SLOT(enableEditArea()));
    connect(varea, SIGNAL(savePoly()), this, SLOT(disableEditArea()));
    return varea;
}

void DynamicObjectManager::enableEditArea()
{
    f_enableEditArea = true;
    f_enableEditRouteShip = false;
}

void DynamicObjectManager::disableEditArea()
{
    f_enableEditArea = false;
}

QList<VArea *> *DynamicObjectManager::getListVArea()
{
    return listVArea;
}
