#ifndef COMMON
#define COMMON
#include <QString>
#include <QVector>
#include <QTime>
#include <math.h>

typedef enum
{
    Area,
    Ship,
    Dron,
    OilDerrick
} TypeObject;

typedef quint64 UIDType;

struct steps_struct
{
    QVector<double> x;
    QVector<double> y;
    QVector<double> angle;
};

struct vect2D
{
    double x;
    double y;
    double operator*(vect2D& v) { return x*v.x + y*v.y; }  // dot product of 2 vectors (scalar product)
    double length() { return sqrt(x*x + y*y); }  // length
    double det(vect2D& v){ return x*v.y - y*v.x; } // determinant
};


#endif // COMMON

