#ifndef FORCES_H
#define FORCES_H

#include <QTime>
#include <QFile>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include "mtwist.h"
#include "avector3d.h"
#include "constants.h"
#include "field.h"



// ---------------------------------- //
// ----Begin Header Declarations----- //
// ---------------------------------- //


// These functions are the forces used by solver.h to solve the equation of motion for the particles

inline aVector3D LJForce (const aVector3D& r1, const aVector3D& r2, qreal lj_type, qreal lj_const);

inline aVector3D exVolRepForce (const aVector3D& r1, const aVector3D& r2);

inline aVector3D wormForce (const aVector3D& v1, const aVector3D& v2);

inline aVector3D bondSpringForce (const aVector3D& r1, const aVector3D& r2);

inline aVector3D bendForce (const aVector3D& r1, const aVector3D& r2, const aVector3D& r3);

inline aVector3D ranForce (MTRand& mtrand1);

inline aVector3D dragForce (const aVector3D& v1);

inline aVector3D coulForce (const aVector3D& r1, const aVector3D& r2, qreal charge1, qreal charge2);

inline aVector3D eBiasForce (const aVector3D& r1, qreal charge1);

inline aVector3D electroForce (const aVector3D& e1, qreal charge1);




// ---------------------------------- //
// ------Begin Body Definitions------ //
// ---------------------------------- //


// ---------INLINE functions--------- //
// ---------------------------------- //


// This force is the Lennard-Jones excluded volume force
inline aVector3D LJForce (const aVector3D& r1, const aVector3D& r2, qreal lj_type, qreal lj_const)
{
        // find distance between the particles
        qreal particle_dist = (r1 - r2).length();

        // check to see if the distance is too big to matter
        if (particle_dist > (lj_const * 2.5))
        {
                aVector3D force;
                force = aVector3D();
                return force;
        }

        qreal bead_ratio = lj_const / particle_dist;
        qreal LJ_Base = pow(bead_ratio,3);
        LJ_Base = LJ_Base * LJ_Base;

        // equation is KbT * [(lconst/distance)^12 - 2(lconst/distance)^6]
        aVector3D force = lj_type * ((LJ_Base * bead_ratio) - (LJ_Base * LJ_Base * bead_ratio)) * (r2 - r1).normalized();

        return force;
}



// This force is the excluded volume force for the DNA polymer chain only
inline aVector3D exVolRepForce (const aVector3D& r1, const aVector3D& r2)
{
        // find distance between the particles
        qreal particle_dist = (r1 - r2).length();

        aVector3D force = exc_vol * exp((-3 / (4*((max_l * 2 * P_l)/6.0))) * particle_dist * particle_dist) * (r1 - r2).normalized();

        return force;
}



// This force is for the spring force for the DNA polymer chain only
inline aVector3D wormForce (const aVector3D& r1, const aVector3D& r2)
{
        // find distance between the particles and the unit vector
        qreal particle_dist = (r1 - r2).length();

        // equation for worm-like chain force is - KbT/4P * [(1 - ratio of distance to max length)^-2 + 4distance/maxlength - 1] * unit vector
        aVector3D force = -1.0 * (kT / 4 * P_l) * (((max_l  / (max_l - particle_dist)) * (max_l  / (max_l - particle_dist))) + (4 * (particle_dist / max_l)) - 1) * (r1 - r2).normalized();;

        return force;
}

// This force is for the spring force for the DNA polymer chain only
inline aVector3D bondSpringForce (const aVector3D& r1, const aVector3D& r2)
{
        // find distance between the particles and the unit vector
        qreal particle_dist = (r1 - r2).length();

        // equation for harmonic stretching force is - 2Kbend * (distance now - equalibrium distance) * unit vector
        aVector3D force = kbond * (particle_dist - bond_l) * (r2 - r1).normalized();

        return force;
}

inline aVector3D bendForce (const aVector3D& r1, const aVector3D& r2, const aVector3D& r3)
{
        qreal dxi=0.001;

        qreal xi_new = r2.x() - dxi;
        qreal yi_new = r2.y() - dxi;
        qreal zi_new = r2.z() - dxi;

        qreal top1 = (r2.x()-r3.x()) * (r1.x()-r2.x()) + (r2.y()-r3.y()) * (r1.y()-r2.y()) + (r2.z()-r3.z()) * (r1.z()-r2.z());
        qreal rip1 = sqrt(pow((r2.x()-r3.x()),2.0) + pow((r2.y()-r3.y()),2.0) + pow((r2.z()-r3.z()),2.0));
        qreal rim1 = sqrt(pow((r1.x()-r2.x()),2.0) + pow((r1.y()-r2.y()),2.0) + pow((r1.z()-r2.z()),2.0));
        qreal costh1 = top1/rip1/rim1;

        qreal top2x = (xi_new-r3.x()) * (r1.x()-xi_new) + (r2.y()-r3.y()) * (r1.y()-r2.y()) + (r2.z()-r3.z()) * (r1.z()-r2.z());
        qreal rip2x = sqrt(pow((xi_new-r3.x()),2.0) + pow((r2.y()-r3.y()),2.0) + pow((r2.z()-r3.z()),2.0));
        qreal rim2x = sqrt(pow((r1.x()-xi_new),2.0) + pow((r1.y()-r2.y()),2.0) + pow((r1.z()-r2.z()),2.0));
        qreal costh2x = top2x/rip2x/rim2x;

        qreal top2y = (r2.x()-r3.x()) * (r1.x()-r2.x()) + (yi_new-r3.y()) * (r1.y()-yi_new) + (r2.z()-r3.z()) * (r1.z()-r2.z());
        qreal rip2y = sqrt(pow((r2.x()-r3.x()),2.0) + pow((yi_new-r3.y()),2.0) + pow((r2.z()-r3.z()),2.0));
        qreal rim2y = sqrt(pow((r1.x()-r2.x()),2.0) + pow((r1.y()-yi_new),2.0) + pow((r1.z()-r2.z()),2.0));
        qreal costh2y = top2y/rip2y/rim2y;

        qreal top2z = (r2.x()-r3.x()) * (r1.x()-r2.x()) + (r2.y()-r3.y()) * (r1.y()-r2.y()) + (zi_new-r3.z()) * (r1.z()-zi_new);
        qreal rip2z = sqrt(pow((r2.x()-r3.x()),2.0) + pow((r2.y()-r3.y()),2.0) + pow((zi_new-r3.z()),2.0));
        qreal rim2z = sqrt(pow((r1.x()-r2.x()),2.0) + pow((r1.y()-r2.y()),2.0) + pow((r1.z()-zi_new),2.0));
        qreal costh2z = top2z/rip2z/rim2z;

        aVector3D force = aVector3D();
        force.setX(kbend*(costh1-costh2x)/dxi);
        force.setY(kbend*(costh1-costh2y)/dxi);
        force.setZ(kbend*(costh1-costh2z)/dxi);

        return force;
}


// This force is the brownian dynamics random force
inline aVector3D ranForce (MTRand& mtrand1)
{
        //if (step == 0)
        //    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()) * static_cast<unsigned int>(getpid()));

        aVector3D ran_vec = aVector3D();
        //qreal floor = -1.0, ceiling = 1.0, range = (ceiling - floor);

        do {


        /*qreal ranNum1 = floor + qreal((range * qrand()) / (RAND_MAX + 1.0));
        qreal ranNum2 = floor + qreal((range * qrand()) / (RAND_MAX + 1.0));
        qreal ranNum3 = floor + qreal((range * qrand()) / (RAND_MAX + 1.0));*/

        /*qreal ranMulti1 = (qrand()%1000)/1000.0;
        qreal ranExp1 = qrand()%1000;
        qreal ranNum1 = pow(-1.0, ranExp1)*ranMulti1;

        qreal ranMulti2 = (qrand()%1000)/1000.0;
        qreal ranExp2 = qrand()%1000;
        qreal ranNum2 = pow(-1.0, ranExp2)*ranMulti2;

        qreal ranMulti3 = (qrand()%1000)/1000.0;
        qreal ranExp3 = qrand()%1000;
        qreal ranNum3 = pow(-1.0, ranExp3)*ranMulti3;*/

        double ranNum1 = -1.0 + mtrand1.rand( 2.0 );
        double ranNum2 = -1.0 + mtrand1.rand( 2.0 );
        double ranNum3 = -1.0 + mtrand1.rand( 2.0 );

        ran_vec.setX(ranNum1);
        ran_vec.setY(ranNum2);
        ran_vec.setZ(ranNum3); }
        while(ran_vec.length() > 1.0);



        aVector3D force = rand1 * ran_vec;

        return force;
}



// Dynamics drag force based on velocity
inline aVector3D dragForce (const aVector3D& v1)
{
        aVector3D force;
        force = -1.0 * drag * v1;
        return force;
}



// This is the electrical force that acts between particles
inline aVector3D coulForce (const aVector3D& r1, const aVector3D& r2, qreal charge1, qreal charge2)
{
        if (charge1 == 0 || charge2 == 0)
        {
            aVector3D force = aVector3D();
            return force;
        }

        // find distance between the particles
        qreal particle_dist = (r1 - r2).length();

        if (particle_dist > (debye * 1.5))
        {
                aVector3D force;
                force = aVector3D();
                return force;
        }

        // equation for screened coulomb force is 1 / (4pi * permitivity) * (charge1 * charge2) * e^(-distance/debye length) / distance * unit vector
        aVector3D force = charge1 * charge2 * dh_coul * exp(-1.0 * particle_dist / debye) *  ((1 / debye) + (1 / particle_dist)) / particle_dist * (r1 - r2).normalized();

        return force;
}



// This is the electrical force that acts due to electrophoresis
inline aVector3D eBiasForce (const aVector3D& r1, qreal charge1)
{
        aVector3D force;

        qreal magnitude = charge1 * v_slope;

        if (r1.y() < memStart)
            force = aVector3D(0,magnitude * exp(-1.0*pow((r1.y() - memStart)/debye,2)),0);
        else if (r1.y() >= memStart && r1.y() <= memEnd)
            force = aVector3D(0,magnitude,0);
        else if (r1.y() > memEnd)
            force = aVector3D(0,magnitude * exp(-1.0*pow((r1.y() - memEnd)/debye,2)),0);

        return force;
}


// Apply the membrane electrostatics from Field class
inline aVector3D electroForce (const aVector3D& e1, qreal charge1)
{
    aVector3D force = charge1 * e1;

    return force;
}


#endif
