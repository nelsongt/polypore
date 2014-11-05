#ifndef POLYMER_H
#define POLYMER_H

#include "algebra.h"
#include "constants.h"


class Polymer
{
	private:
		double charge;			// strength of field


	public:
		double get_charge();

		void set_charge(double newCharge);
	
		aVector find_potential(aVector v1);
};

#endif
