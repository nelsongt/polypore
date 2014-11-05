#ifndef PARTICLE_H
#define PARTICLE_H

#include "avector3d.h"
#include "constants.h"

struct Particle
{
        public:
                quint16 ptype;		 	// type of particle
                qreal charge;			// charge of a particle

                aVector3D pos;          	// particle vectors
                aVector3D temp_pos;
                aVector3D vel;
                aVector3D temp_vel;
                aVector3D force;
                aVector3D temp_force;
};

#endif


