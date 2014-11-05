#include "polymer.h"

//  private:

	double charge = 3 * q_elem; 		// charge of the particle

//  public:

	double Polymer::get_charge() {return charge;}

	void Polymer::set_charge(double newCharge) {charge = newCharge;}

	aVector find_potential(aVector 1)
