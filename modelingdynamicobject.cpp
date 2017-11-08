#include "modelingdynamicobject.h"
#include "ui_modelingdynamicobject.h"

ModelingDynamicObject::ModelingDynamicObject(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModelingDynamicObject),
    scene(new QGraphicsScene)
{
    ui->setupUi(this);
    scene->setSceneRect(ui->graphicsView->rect());

    ui->graphicsView->setScene(scene);

    mapObjectMngr = new DynamicObjMngr(ui);
    mapObjectMngr->setGraphicsView(ui->graphicsView);
    mapObjectMngr->setVScene(scene);
    mapObjectMngr->setTimer(&timer);

    connect(&timer, SIGNAL(timeout()), scene, SLOT(advance()));

}

ModelingDynamicObject::~ModelingDynamicObject()
{
    delete ui;
}
