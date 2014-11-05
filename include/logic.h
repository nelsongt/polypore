#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/time.h>

#include "algebra.h"
#include "polymer.h"
#include "solver.h"


void timeMove()
{
	// make the bead polymer
	int beadNumber = 10;
	Polymer dnachain[beadNumber];


	// initial conditions of the bead
	aVector pos_i(2);
	aVector vel_i(2);
	aVector force_i(2);
	pos_i(0) = -6.0;
	pos_i(1) = -0.0;
	vel_i = 0.0;
	force_i = 0.0;

	// remake this into a loop later
	for ( int p = 0; p < beadNumber; p++ )
	{
		dnachain[p].set_bead_vel(vel_i);
		dnachain[p].set_bead_force(force_i);
		if ( p == 0 ) {
			dnachain[p].set_ptype(0);  }
		else if ( p == (beadNumber - 1) ) {
			dnachain[p].set_ptype(2);  }
		else {
			dnachain[p].set_ptype(1);  }
	}

	dnachain[0].set_bead_pos(pos_i);
	pos_i(0) = -4.5;
	pos_i(1) = -1.5;
	dnachain[1].set_bead_pos(pos_i);
	pos_i(0) = -3.0;
	pos_i(1) = 0.0;
	dnachain[2].set_bead_pos(pos_i);
	pos_i(0) = -1.5;
	pos_i(1) = 1.5;
	dnachain[3].set_bead_pos(pos_i);
	pos_i = 0.0;
	dnachain[4].set_bead_pos(pos_i);
	pos_i(0) = 1.5;
	pos_i(1) = -1.5;
	dnachain[5].set_bead_pos(pos_i);
	pos_i(0) = 3.0;
	pos_i(1) = 0.0;
	dnachain[6].set_bead_pos(pos_i);
	pos_i(0) = 4.5;
	pos_i(1) = 1.5;
	dnachain[7].set_bead_pos(pos_i);
	pos_i(0) = 6.0;
	pos_i(1) = 0.0;
	dnachain[8].set_bead_pos(pos_i);
	pos_i(0) = 7.5;
	pos_i(1) = -1.5;
	dnachain[9].set_bead_pos(pos_i);



	/*	Particle Types
		0 = chain first piece
		1 = chain middle piece
		2 = chain last piece
		3 = counterion		*/


	// create temp copies of position and velocity data so that timesteps aren't mixed up
	aVector temp_pos[beadNumber];
	aVector temp_vel[beadNumber];
	for ( int p = 0; p < beadNumber; p++ )
	{
		temp_pos[p] = dnachain[p].get_bead_pos();
		temp_vel[p] = dnachain[p].get_bead_vel();
	}
	

	// create the bead movement variables
	aVector posit_move = dnachain[0].get_bead_pos();
	aVector veloc_move = dnachain[0].get_bead_vel();
	aVector force_move = dnachain[0].get_bead_force();	
	aVector veloc_pred(2);


	// create the results variable (so the results will scale correctly)
	aVector results2 = dnachain[0].get_bead_force();
	aVector results = posit_move;


	// this loop begins and ends each timestep
	for ( int n = 0; n < steps; n++ ) 
	{


		// reset the temp copies of data for each time step so that timesteps aren't mixed up
		for ( int p = 0; p < beadNumber; p++ )
		{
			temp_pos[p] = dnachain[p].get_bead_pos();
			temp_vel[p] = dnachain[p].get_bead_vel();
		}


		// this loop will calculate the movement of each particle one at a time for this timestep
		for  ( int i = 0; i < beadNumber; i++ )
		{
			// Find the new position, velocity and force vectors
			posit_move = posit_step(temp_pos[i], temp_vel[i], dnachain[i].get_bead_force(), timeStep);
		
			veloc_pred = vel_predict(temp_pos[i], temp_vel[i], dnachain[i].get_bead_force(), timeStep);


			// the force vector is depended upon all other particles in the system
			force_move = 0.0;
			for ( int j = 1; j < beadNumber; j++ )
			{		
				force_move += force_step(posit_move, veloc_pred, temp_pos[(i + j) % beadNumber], temp_vel[(i + j) % beadNumber], 1);
			}


			// conditional statement decides if and how the worm force should be added
			if ( dnachain[i].get_ptype() == 0 ) {
				force_move += force_step_2(posit_move, temp_pos[i + 1]);
			}
			else if ( dnachain[i].get_ptype() == 1 ) {
				force_move += force_step_2(posit_move, temp_pos[i - 1]);
				force_move += force_step_2(posit_move, temp_pos[i + 1]);
			}
			else if ( dnachain[i].get_ptype() == 2 ) {
				force_move += force_step_2(posit_move, temp_pos[i - 1]);
			}


			veloc_move = speed_step(temp_pos[i], temp_vel[i], dnachain[i].get_bead_force(), force_move, timeStep);


			if ( norm(results2) < norm(dnachain[i].get_bead_force()) ) {
				results2 = dnachain[i].get_bead_force();
			}

			// reset the data vectors of the beads for this timestep		
			dnachain[i].set_bead_pos(posit_move);  
			dnachain[i].set_bead_vel(veloc_move);
			dnachain[i].set_bead_force(force_move);    
		}
	}


	return 0;
}
