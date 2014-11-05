#ifndef SOLVER_H
#define SOLVER_H

#include "avector3d.h"
#include "mtwist.h"
#include "constants.h"
#include "forces.h"
#include "particle.h"



// ---------------------------------- //
// ----Begin Header Declarations----- //
// ---------------------------------- //



// These functions solve the equation of motion for each particle using the force-function defined in the forces.h file

inline aVector3D posit_step_langevin (const QList<Particle>& dnachain, const quint16 i);

inline aVector3D posit_step_brownian (const QList<Particle>& dnachain, const quint16 i);

inline aVector3D vel_predict (const QList<Particle>& dnachain, const quint16 i);

inline aVector3D speed_step (const QList<Particle>& dnachain, const quint16 i);

inline aVector3D force_step_all (const QList<Particle>& dnachain, const quint16 i, const quint16 j);

inline aVector3D force_step_chain (const QList<Particle>& dnachain, const quint16 i, const quint16 j);

inline aVector3D force_step_bend (const QList<Particle>& dnachain, const quint16 i);

inline aVector3D force_step_sngl (const QList<Particle>& dnachain, const quint16 i, const aVector3D& eField, MTRand& mtrand1);



// ---------------------------------- //
// ------Begin Body Definitions------ //
// ---------------------------------- //


// ---------INLINE functions--------- //
// ---------------------------------- //


inline aVector3D posit_step_langevin (const QList<Particle>& dnachain, const quint16 i)
{
        aVector3D new_posit = dnachain[i].temp_pos;
        new_posit += (timeStep * dnachain[i].temp_vel);
        new_posit += (0.5 * timeStep * timeStep * dnachain[i].temp_force / umass);

        return new_posit;
}

inline aVector3D posit_step_brownian (const QList<Particle>& dnachain, const quint16 i)
{
        aVector3D new_posit = dnachain[i].temp_pos;
        new_posit += (timeDrag * dnachain[i].temp_force);

        return new_posit;
}


inline aVector3D vel_predict (const QList<Particle>& dnachain, const quint16 i)
{
        aVector3D new_velpred = dnachain[i].temp_vel + (lambda * timeStep * dnachain[i].temp_force / umass);

        return new_velpred;
}


inline aVector3D speed_step (const QList<Particle>& dnachain, const quint16 i)
{
        aVector3D new_vel = dnachain[i].temp_vel + (0.5 * timeStep * (dnachain[i].force + dnachain[i].temp_force) / umass);

        return new_vel;
}


inline aVector3D force_step_all (const QList<Particle>& dnachain, const quint16 i, const quint16 j)
{

        // first calculate the repulsive force(s)
        //aVector3D new_force1 = LJForce(dnachain[i].pos, dnachain[(i + j) % beadNumber].pos);

        aVector3D new_force1 = LJForce(dnachain[i].pos, dnachain[j].pos, lj_bead, bead_d);

        // next do the coulomb force
        //aVector3D new_force = coulForce(dnachain[i].pos, dnachain[(i + j) % beadNumber].pos, dnachain[i].charge, dnachain[(i + j) % beadNumber].charge);
        //new_force1 += coulForce(dnachain[i].pos, dnachain[(i + j) % beadNumber].pos, dnachain[i].charge, dnachain[(i + j) % beadNumber].charge);


        //aVector3D new_force1 = aVector3D();
        return new_force1;
}


// this step is for polymers only
inline aVector3D force_step_chain (const QList<Particle>& dnachain, const quint16 i, const quint16 j)
{

        // additional bond force is calculated for polymer beads
        //aVector3D new_force2 = wormForce(dnachain[i].pos, dnachain[j].pos);
        aVector3D new_force2 = bondSpringForce(dnachain[i].pos, dnachain[j].pos);
        //aVector3D new_force2 = aVector3D();
        return new_force2;
}


inline aVector3D force_step_bend (const QList<Particle>& dnachain, const quint16 i)
{
        //aVector3D new_force3 = bendForce(dnachain[i-1].pos, dnachain[i].pos, dnachain[i+1].pos);
        aVector3D new_force3 = aVector3D();
        return new_force3;
}


inline aVector3D force_step_sngl (const QList<Particle>& dnachain, const quint16 i, const aVector3D& eField, MTRand& mtrand1)
{
        // Do the drag force
        //aVector3D new_force4 = dragForce(dnachain[i].vel);
        //new_force4 += dragForce(dnachain[i].vel);


        // next do the random force
        aVector3D new_force4 = ranForce(mtrand1);


        // next do the electrophoretic force
        //aVector3D new_force4 = eBiasForce(dnachain[i].pos, dnachain[i].charge);
        new_force4 += eBiasForce(dnachain[i].pos, dnachain[i].charge);


        // next do the electrostatic force
        //aVector3D new_force = electroForce(eField, dnachain[i].charge);
        new_force4 += electroForce(eField, dnachain[i].charge);


        //aVector3D new_force4 = aVector3D();
        return new_force4;
}


#endif
