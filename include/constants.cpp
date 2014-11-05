
#include "constants.h"


// This file contains global variables and constants,
// each global variable MUST be DEFINED in the "constants.cpp" and
// declared in the "constants.h" with the "extern" keyword!




        qreal k_B = 1.3806504e-23; //8.617343(15)e-5 eV/K  =  1.3806504e-23 J/K
        qreal Temperature = 298.0; // K
        qreal kT = k_B * Temperature ;  // k_B * Temperature;
        qreal calorie = 6.9477e-21;

        qreal pi = 3.14159265; // no units
        qreal drag = 7.5e-9; // N*s/m
        qreal umass = 1.0; // mass of a bead
  
        // electric constants
        qreal q_elem = 1.602e-19; // 1e = 1.602e-19 C;  4.11x10e−21
        qreal eps_0 = 8.85e-12; //5.52635e-2 e^2/(eV nm) = 5.52635e7 e^2/(eV m) =  8.85e-12 C^2/(J m)  !change this!
        qreal eps_Si = 11.70; // same units
        qreal eps_Sol = 78.0; // Solution
        qreal const v_slope = (-0.410 / memThick) * 1.0e20; // N/C

        // lengths (in angstroms)
        //qreal r_c = 2.0; // cutoff radius
        qreal debye = 10.0; // Debye length
        qreal bond_l = 3.5; // length of bond
        qreal bead_d = 6.5; // width of atom
        qreal bead_w = 3.5; // atom wall lj equil.

        // force constant
        qreal const kbond = 171.0 * 2 * calorie * 1.0e20; // bond constant
        qreal const kbend = 8.0 * 0.5 * calorie * 1.0e20;
        qreal const lj_bead = 0.1 * 1.0 * calorie * 1.0e20 * 12 / bead_d; // Bead-Bead Lennard Jones constant
        qreal const lj_wall = 0.1 * 1.0 * calorie * 1.0e20 * 12 / bead_w; // Bead-Wall
        qreal const dh_coul = 1.0 / (4.0 * pi * eps_0 * eps_Sol) * 1.0e30;
        qreal const rand1 = sqrt(6.0 * kT * drag / timeStep) * 1.0e10;


        // Velocity-Verlet parameters
        qreal lambda = 0.5;    // prediction-correction


        // old constants
        qreal max_l = 2.778;   // worm-like chain max spring length
        qreal P_l = 2.778;     // Persistence length

        qreal const exc_vol = ((3.0 * .0012 * (max_l / 2.0 * P_l) * (max_l / 2.0 * P_l)) / (4.0*((max_l * 2.0 * P_l)/6.0))) * pow((3.0 / ( 4.0 * pi * ((max_l * 2.0 * P_l)/6.0))),(3.0 / 2.0));




	// !!!
	// Not constants
	// (Mode Specification)

        quint16 THREADS = 1;

        qreal timeStep = 5.0e-12;     //set the time interval for the simulation

        const qreal scale = 2.0;      //change the graphics size

        unsigned int beadNumber = 45;

        // Membrane Dimensions
        qreal chHeight = 100.0;
        qreal chLength = 640.0;
        qreal chWidth = 240.0;
        qreal memThick = 260.0;
        qreal bigPoreRadius = 10.0;
        qreal lilPoreRadius = 10.0;
        qreal poreCenterX = 110.0;
        qreal poreCenterY = chLength / 2.0;
        qreal poreCenterZ = 50.0;
        qreal transCut = 470;

        // Convenient Numbers
        qreal const memStart = (chLength - memThick) / 2;
        qreal const memEnd = (chLength + memThick) / 2;
        qreal const bigPoreDepth = poreCenterX - bigPoreRadius;
        qreal const bigPoreHeightHi = poreCenterZ + bigPoreRadius;
        qreal const bigPoreHeightLo = poreCenterZ - bigPoreRadius;
        qreal const lilPoreDepth = poreCenterX - lilPoreRadius;
        qreal const lilPoreHeightHi = poreCenterZ + lilPoreRadius;
        qreal const lilPoreHeightLo = poreCenterZ - lilPoreRadius;
        qreal const radSlope = (lilPoreRadius - bigPoreRadius) / (poreCenterY - memStart);

        qreal const timeDrag = timeStep / drag;

