#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGlobal>
#include <cmath>

// This file contains global variables and constants,
// each global variable MUST be DEFINED in the "constants.cpp" and
// declared in the "constants.h" with the "extern" keyword!


        extern qreal k_B ; // 8.617343(15)e-5 eV/K  =  1.3806504e-23 J/K
        extern qreal Temperature; // K
        extern qreal kT; // eV
        extern qreal calorie;

        extern qreal pi ; // 3.14159
        extern qreal drag ; // 11.9 N*s/m
        extern qreal umass;

        extern qreal q_elem; // 1e = 1.602e-19 C;
        extern qreal eps_0; //5.52635e-2 e^2/(eV nm) = 5.52635e7 e^2/(eV m) =  8.85e-12 C^2/(J m)
        extern qreal eps_Si; // units
        extern qreal eps_Sol; // solvent
        extern qreal const v_slope; // linear slope of the

        //extern qreal r_c; // cutoff radius
        extern qreal debye;  // Debye length
        extern qreal bond_l; // length of bond
        extern qreal bead_d; // diameter of bead
        extern qreal bead_w; // bead-wall LJ equil.

        extern qreal const kbond;
        extern qreal const kbend;
        extern qreal const lj_bead;
        extern qreal const lj_wall;
        extern qreal const dh_coul;
        extern qreal const rand1;


        extern qreal lambda; // Velocity-Verlet parameter, used for prediction-correction

        extern qreal max_l; // worm-like chain spring length
        extern qreal P_l; // Persistence length

        extern qreal const exc_vol;


	// !!!
	// Not constants
	// (Mode Specification)

        extern quint16 THREADS;

        extern qreal timeStep;    // set the time interval for the simulation and # of steps

        extern const qreal scale; // pixels per micrometer

        extern unsigned int beadNumber;

        extern qreal chHeight;      // geometry
        extern qreal chLength;
        extern qreal chWidth;
        extern qreal memThick;
        extern qreal bigPoreRadius;
        extern qreal lilPoreRadius;
        extern qreal poreCenterX;
        extern qreal poreCenterY;
        extern qreal poreCenterZ;
        extern qreal transCut;

        extern qreal const memStart;  // for convenience
        extern qreal const memEnd;
        extern qreal const bigPoreDepth;
        extern qreal const bigPoreHeightHi;
        extern qreal const bigPoreHeightLo;
        extern qreal const lilPoreDepth;
        extern qreal const lilPoreHeightHi;
        extern qreal const lilPoreHeightLo;
        extern qreal const radSlope;

        extern qreal const timeDrag;


#endif
