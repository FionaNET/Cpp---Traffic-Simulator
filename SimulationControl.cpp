#include "SimulationControl.h"

SimulationControl::SimulationControl()
{
	model = new TrafficModel();
	view = new SimulationUI("input.txt");
}

SimulationControl::~SimulationControl()
{
	delete view;
	delete model;
}

bool SimulationControl::run()
{
	// initialization
	int time = 0;
	int max_time = view->getMaxSimulationTime();
	initialization();

	while(time <= max_time-1)
	{
		time++;
		simulationStep(time);
	}
	return true;
}

/*
 * A single step (tick) execution
 */
void SimulationControl::simulationStep(int tick)
{
	// update the system state for this tick
	model->update();
	// read the lane change commands
	vector<string> commands = view->getCommands(tick);
	model->set_commands(commands);
	// obtain the result from the simulation, and write to the result.txt file
	vector<string> output = model->get_system_state();
	view->writeResult(tick, output);
}

/*
 * Initialization
 */
void SimulationControl::initialization()
{
	// get the initial positions
	vector<string> init = view->getInitInfo();
	model->initialize(init);
	// read the lane change commands
	vector<string> commands = view->getCommands(0); // zero tick
	model->set_commands(commands);
	// obtain the result from the simulation, and write to the result.txt file
	vector<string> output = model->get_system_state();
	view->writeResult(0, output); // zero tick
}

