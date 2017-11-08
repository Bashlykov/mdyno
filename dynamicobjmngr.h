#ifndef DYNAMICOBJMNGR_H
#define DYNAMICOBJMNGR_H

#include <QGraphicsView>

#include <ui_modelingdynamicobject.h>

#include <idynamicobject.h>
#include <dynamicship.h>
#include <dynamicdron.h>
#include <varea.h>

class DynamicObjMngr: public QObject
{
    Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsView *graphView;

    QList<DynamicShip*> *listShip;
    DynamicShip *currentShip;

    QTimer *timer;
    bool f_createdShip;    
    bool f_startModeling;
    int indexArea;

    uint countShip;

    QList<VArea*> *listVArea;
    VArea* currentVarea;
    int type_point;

    bool f_enableEditArea;
    bool f_enableEditRouteShip;

    TypeObject object_select;
    Ui::ModelingDynamicObject *ui;

protected:
      //! Обработчик событий из QGraphicsScene
       bool eventFilter(QObject *, QEvent *event) override;

public:
    DynamicObjMngr(Ui::ModelingDynamicObject *ui);
    ~DynamicObjMngr();
    void setGraphicsView(QGraphicsView *graphView);
    void setVScene(QGraphicsScene* scene);
    void createShip();
    void createOilDerrick();
    void setTimer(QTimer *t);

    //Работа с районами
    VArea *createVArea();
    QList<VArea *> * getListVArea();
    void setCurrentIndexArea(int i);

public slots:
    void startModeling();
    void stopModeling();
    void setCurrentIndexObject(int index);

    void fastSpeedModeling();
    void slowSpeedModeling();
    void pauseModeling();

    void addObjectToMap();
    void setCurrentShip(IDynamicObject *ship);
    void enableEditArea();
    void disableEditArea();

    void enableEditRoute();

};

#endif // DYNAMICOBJMNGR_H
