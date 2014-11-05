#ifndef FIELD_H
#define FIELD_H

#include <QThread>
#include <QFile>
#include <QTextStream>

#include "avector3d.h"
#include "constants.h"



// ---------------------------------- //
// ----Begin Header Declarations----- //
// ---------------------------------- //


class Field : public QThread
{
    Q_OBJECT

    friend class Cycle;

    QList<aVector3D> grid;
    QList<float> field;

    quint32 N;
    quint32 xlim;
    quint32 ylim;
    quint32 zlim;
    quint32 xsav;
    quint32 ysav;
    quint32 zsav;


    inline qreal potenAtIndex(quint32 i, quint32 j, quint32 k);

    inline qreal xAtIndex(quint32 i);
    inline qreal yAtIndex(quint32 j);
    inline qreal zAtIndex(quint32 k);
    inline void locate(const aVector3D& r1, aVector3D& rl);

public:

    Field(QObject *parent = 0);
    ~Field();

    inline aVector3D fieldNearPoint(const aVector3D& r1);

public slots:

    void buildField();

signals:

    void inputError(QString error);
};




// ---------------------------------- //
// ------Begin Body Definitions------ //
// ---------------------------------- //



// ---------INLINE functions--------- //
// ---------------------------------- //


inline qreal Field::potenAtIndex(quint32 i, quint32 j, quint32 k)
{
    return field[j+k*ylim+i*ylim*zlim];
}

inline qreal Field::xAtIndex(quint32 i)
{
    return grid[i*ylim*zlim].x();
}

inline qreal Field::yAtIndex(quint32 j)
{
    return grid[j].y();
}

inline qreal Field::zAtIndex(quint32 k)
{
    return grid[k*ylim].z();
}


inline void Field::locate(const aVector3D& r1, aVector3D& rl)
{
    // This will cycle through the data until it finds the closest points

    // xlim,ylim,zlim are the dimensions of the table
    // xu and xl are the current guesses at the closest data point table indices
    qint32 xl,xu,xm,yl,yu,ym,zl,zu,zm, inc=1;
    xl = xsav;
    yl = ysav;
    zl = zsav;


    if (xl < 0 || xl > xlim-1) {
        xl = 0;
        xu = xlim-1;
    } else {
        if (xAtIndex(xl) < r1.x()) {   // Hunt up
            for(;;) {
                xu = xl + inc;
                if (xu >= xlim-1) { xu = xlim - 1; break; }     //Went past the table
                else if (xAtIndex(xu) >= r1.x())    break;       //Value lies within
                else {                                          //Not found, increase search space
                    xl = xu;
                    inc += inc;
                }
            }
        } else {                       // Hunt down
            xu = xl;
            for(;;) {
                xl = xl - inc;
                if (xl <= 0)          { xl = 0; break; }     //Went past the table
                else if (xAtIndex(xl) < r1.x()) break;       //Found it
                else {                                       //Not done
                    xu = xl;
                    inc += inc;
                }
            }
        }
    }

    while (xu-xl > 1)  // Bisection phase
    {
        xm = (xu+xl) >> 1;  // Uses bitwise to find an approx midpoint integer

        if (xAtIndex(xm) < r1.x())        xl = xm;
        else                              xu = xm;
    }



    inc = 1;

    if (yl < 0 || yl > ylim-1) {
        yl = 0;
        yu = ylim-1;
    } else {
        if (yAtIndex(yl) < r1.y()) {   // Hunt up
            for(;;) {
                yu = yl + inc;
                if (yu >= ylim-1) { yu = ylim - 1; break; }     //Went past the table
                else if (yAtIndex(yu) >= r1.y())    break;       //Value lies within
                else {                                          //Not found, increase search space
                    yl = yu;
                    inc += inc;
                }
            }
        } else {                       // Hunt down
            yu = yl;
            for(;;) {
                yl = yl - inc;
                if (yl <= 0)          { yl = 0; break; }     //Went past the table
                else if (yAtIndex(yl) < r1.y()) break;       //Found it
                else {                                       //Not done
                    yu = yl;
                    inc += inc;
                }
            }
        }
    }

    while (yu-yl > 1)
    {
        ym = (yu+yl) >> 1;  // Uses bitwise to find an approx midpoint integer

        if (yAtIndex(ym) < r1.y())        yl = ym;
        else                              yu = ym;
    }

    inc = 1;

    if (zl < 0 || zl > zlim-1) {
        zl = 0;
        zu = zlim-1;
    } else {
        if (zAtIndex(zl) < r1.z()) {   // Hunt up
            for(;;) {
                zu = zl + inc;
                if (zu >= zlim-1) { zu = zlim - 1; break; }     //Went past the table
                else if (zAtIndex(zu) >= r1.z())    break;       //Value lies within
                else {                                          //Not found, increase search space
                    zl = zu;
                    inc += inc;
                }
            }
        } else {                       // Hunt down
            zu = zl;
            for(;;) {
                zl = zl - inc;
                if (zl <= 0)          { zl = 0; break; }     //Went past the table
                else if (zAtIndex(zl) < r1.z()) break;       //Found it
                else {                                       //Not done
                    zu = zl;
                    inc += inc;
                }
            }
        }
    }

    while (zu-zl > 1)
    {
        zm = (zu+zl) >> 1;  // Uses bitwise to find an approx midpoint integer

        if (zAtIndex(zm) < r1.z())        zl = zm;
        else                              zu = zm;
    }


    xsav = xl;
    ysav = yl;
    zsav = zl;

    rl.setX(xl);
    rl.setY(yl);
    rl.setZ(zl);
}


inline aVector3D Field::fieldNearPoint(const aVector3D& r1)
{
    QTextStream out(stdout);

    aVector3D rl;

    locate(r1,rl);

    quint32 i = rl.x();
    quint32 j = rl.y();
    quint32 k = rl.z();

    /*quint32 i = 0;
    quint32 j = 0;
    quint32 k = 0;

    // This will cycle through the data until it finds the closest points
    for (unsigned int n=0; n < 2106081; n=n+(ylim*zlim))
    {
        if (grid[n].x() < r1.x() && grid[n].x() > xAtIndex(i))       i++;
    }

    for (unsigned int n=0; n < ylim; n++)
    {
        if (grid[n].y() < r1.y() && grid[n].y() > yAtIndex(j))       j++;
    }


    for (unsigned int n=0; n < (ylim*zlim); n=n+ylim)
    {
        if (grid[n].z() < r1.z() && grid[n].z() > zAtIndex(k))       k++;
    }*/


    qreal x1 = xAtIndex(i);
    qreal y1 = yAtIndex(j);
    qreal z1 = zAtIndex(k);
    qreal x2 = xAtIndex(i+1);
    qreal y2 = yAtIndex(j+1);
    qreal z2 = zAtIndex(k+1);

    qreal space_x = x2 - x1;
    qreal space_y = y2 - y1;
    qreal space_z = z2 - z1;

    qreal pot111 = potenAtIndex(i,j,k);
    qreal pot211 = potenAtIndex(i+1,j,k);
    qreal pot121 = potenAtIndex(i,j+1,k);
    qreal pot112 = potenAtIndex(i,j,k+1);
    qreal pot221 = potenAtIndex(i+1,j+1,k);
    qreal pot212 = potenAtIndex(i+1,j,k+1);
    qreal pot122 = potenAtIndex(i,j+1,k+1);
    qreal pot222 = potenAtIndex(i+1,j+1,k+1);

    qreal inv_x = 1 / space_x;
    qreal inv_y = 1 / space_y;
    qreal inv_z = 1 / space_z;

    qreal slope_x1 = (pot211 - pot111) * inv_x;
    qreal slope_x2 = (pot212 - pot112) * inv_x;
    qreal slope_x3 = (pot221 - pot121) * inv_x;
    qreal slope_x4 = (pot222 - pot122) * inv_x;

    qreal slope_y1 = (pot121 - pot111) * inv_y;
    qreal slope_y2 = (pot221 - pot211) * inv_y;
    qreal slope_y3 = (pot122 - pot112) * inv_y;
    qreal slope_y4 = (pot222 - pot212) * inv_y;

    qreal slope_z1 = (pot112 - pot111) * inv_z;
    qreal slope_z2 = (pot122 - pot121) * inv_z;
    qreal slope_z3 = (pot212 - pot211) * inv_z;
    qreal slope_z4 = (pot222 - pot221) * inv_z;


    qreal Exz1 = slope_x1 + (r1.z() - z1) * ((slope_x2 - slope_x1)*inv_z);
    qreal Exz2 = slope_x3 + (r1.z() - z1) * ((slope_x3 - slope_x4)*inv_z);
    qreal Ex = Exz1 + (r1.y() - y1) * ((Exz2 - Exz1)*inv_y);

    qreal Eyx1 = slope_y1 + (r1.x() - x1) * ((slope_y2 - slope_y1)*inv_x);
    qreal Eyx2 = slope_y3 + (r1.x() - x1) * ((slope_y3 - slope_y4)*inv_x);
    qreal Ey = Eyx1 + (r1.z() - z1) * ((Eyx2 - Eyx1)*inv_z);

    qreal Ezy1 = slope_z1 + (r1.y() - y1) * ((slope_z2 - slope_z1)*inv_y);
    qreal Ezy2 = slope_z3 + (r1.y() - y1) * ((slope_z3 - slope_z4)*inv_y);
    qreal Ez = Ezy1 + (r1.x() - x1) * ((Ezy2 - Ezy1)*inv_x);

    //out << yAtIndex(160) << "    \t" << ylim << endl;

    //QTextStream out2(stdout);
    //out2 << x1 << "   \t" << r1.x() << "   \t" << x2 << "   \t" << Ex << endl;


    return (-1.0 * aVector3D(Ex,Ey,Ez));
}

#endif // FIELD_H



