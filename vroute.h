#ifndef VROUTE_H
#define VROUTE_H

#include <QObject>
#include <QGraphicsScene>
#include <varea.h>
#include <vpaintline.h>
#include <vpaintpolygon.h>
#include <vpaintpoint.h>
#include <vpaintmultiline.h>

class VRoute: public QObject
{
    Q_OBJECT
public:
    VRoute();

    virtual void setArea(VArea *) = 0;
    virtual void setVScene(QGraphicsScene *) = 0;

    virtual VPointPaintArray *getPaintPoints() = 0;
    virtual QVector<QPointF> *getPoints() = 0;
    virtual void setPaintPoints(VPaintPoint *, double ) = 0;
    virtual void createFromNet(QVector<QPointF> *, UIDType ) = 0;
    virtual void remove() = 0;
    virtual VPaintMultiLine *getLine() = 0;
    virtual void addPointFromCursor(QPointF &pos) = 0;

    virtual void setSpeed(float speed) = 0;
    virtual void setUID(UIDType uid) = 0;
    virtual UIDType getUID() = 0;

    virtual bool getFlagBuild() = 0;
    virtual void setFlagBuild(bool) = 0;

public slots:
    virtual void create() = 0;
    virtual void setDefaultColor() = 0;
    virtual void setNumPointClick(int) = 0;
    virtual void set(QPointF) = 0;

signals:
    void createPoint(VPaintPoint *);
    void addedSecondPoint(bool);
    void created(bool);
    void addFirstPoint(QPointF &);
    void finish();
    void except();
};

#endif // VROUTE_H
