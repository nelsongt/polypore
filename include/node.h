#ifndef NODE_H
#define NODE_H

#include <QVector3D>

struct NODE
{
    QVector3D pos;

    //calculatable
    QVector3D tempPos;
    double fx;
    double fy;
    double fz;
};

#endif // NODE_H
