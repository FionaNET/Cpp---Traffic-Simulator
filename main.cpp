#include "SimulationControl.h"

int main(int argc, const char * argv[])
{
	SimulationControl* controller = new SimulationControl();
	
	//run the simulation
	controller->run();
	
	delete controller;
	
	return 0;
}

