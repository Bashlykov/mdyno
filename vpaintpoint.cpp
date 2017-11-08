#include "vpaintpoint.h"

VPaintPoint::VPaintPoint(QRect size,
                          bool f_move,
                          Qt::GlobalColor color,
                          Qt::GlobalColor penColor,
                          int num):
    size(size),
    color(color),
    penColor(penColor),
    num (num),
    tmpColor(color),
    tmpPenColor (penColor),
    f_move (f_move)
{
    setFlag( QGraphicsItem::ItemIgnoresTransformations );

    if (f_move)
    {
        setCursor(Qt::OpenHandCursor);
        setAcceptedMouseButtons(Qt::LeftButton);
    }
    prevVisibleState = true;
}

VPaintPoint::~VPaintPoint()
{
}

QRectF VPaintPoint::boundingRect() const
{
    return QRectF( size );
}

QPainterPath VPaintPoint::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void VPaintPoint::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pPen(penColor, 1, Qt::SolidLine);
    pPen.setCosmetic(true);
    p->setPen(pPen);
    QBrush mPointBrush(color);
    p->setBrush( mPointBrush );
    p->drawEllipse( size );
    scene()->update(scene()->sceneRect());
}

void VPaintPoint::advance(int step)
{
    if (!step)
        return;
}

void VPaintPoint::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit numPoint(num);
    color = Qt::green;
    penColor = Qt::yellow;
    scene()->update();
}

void VPaintPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (f_move)
    {
        setPos(event->scenePos());
        emit setNewPos(event->scenePos());
        emit setNumNewPos(num, event->scenePos());
    }
}

void VPaintPoint::selectFromRow(int num)
{
    if (this->num == num)
    {
        color = Qt::yellow;
        penColor = Qt::yellow;
    }
    else
    {
        color = tmpColor;
        penColor = tmpPenColor;
    }
}

void VPaintPoint::setDefaultColor()
{
    color = tmpColor;
    penColor = tmpPenColor;
}
