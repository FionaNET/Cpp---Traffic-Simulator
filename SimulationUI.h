#ifndef _SIMULATION_UI_H
#define _SIMULATION_UI_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

class SimulationUI
{
	private: 
		string filename;

	public:
		SimulationUI(string filename);
		~SimulationUI();

		int getMaxSimulationTime();  
		vector<string> getInitInfo(); 
		vector<string> getCommands(int tick); 
		void writeResult(int tick, vector<string> state);
		vector<string> split(string line, char c);
};

#endif 
