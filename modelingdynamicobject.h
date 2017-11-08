#ifndef MODELINGDYNAMICOBJECT_H
#define MODELINGDYNAMICOBJECT_H

#include <QMainWindow>
#include <QGraphicsScene>

#include <dynamicobjmngr.h>
#include <vpaintpoint.h>
#include <vpaintline.h>
#include <vpaintpolygon.h>

namespace Ui {
class ModelingDynamicObject;
}

class ModelingDynamicObject : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModelingDynamicObject(QWidget *parent = 0);
    ~ModelingDynamicObject();

private:
    Ui::ModelingDynamicObject *ui;
    QGraphicsScene *scene;
    DynamicObjMngr *mapObjectMngr;
    QTimer timer;
};

#endif // MODELINGDYNAMICOBJECT_H
