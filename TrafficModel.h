#ifndef _TrafficModel_H_
#define _TrafficModel_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include "Platoon.h"

#include "Car.h"

using namespace std;

class TrafficModel
{
	private:
		vector<Platoon> platoons;
		vector<string> commands; // stores a set of lane change commands

	public:
		TrafficModel();
		~TrafficModel();

		int get_lane_change_command(int id);
		void set_commands(vector<string> commands);
		void initialize(vector<string> info);
		void update();
		vector<string> get_system_state();
		bool CheckCarIDS(vector<int> MovedCarIDS, int carID);
};


#endif 
