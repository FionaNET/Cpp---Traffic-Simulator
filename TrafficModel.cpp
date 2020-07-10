#include "TrafficModel.h"
#include <stdlib.h>
using namespace std;
#include <iostream>

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){}

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{

	//Vector that contains cars that want to move- important to ensure that we check all the cars before actually moving them
	std::vector<Car*> PotentialChange;

	//Initial pointer will point to the head of the platoon at lane 0 ( platoon[0])
	Car* currentpointer = NULL;
	int currentID = -1;
	int command = -1;

	//Loop through all the lanes, starting at platoon[0]
	for (unsigned int j = 0; j < platoons.size();j++){

		//updating current pointer to point at a specific lane , starting from the rightmost end (tail)
		currentpointer = platoons[j].get_tail();

		while (currentpointer != NULL){

			//Setting lane of each car
			currentpointer->lane = j;

			//By default the car will not be able to move until future direction is set to 0,1,2 for straight, left, right respectively
			currentpointer->FutureDirection= -1;

			//Car information
			currentID = currentpointer->get_id();
			command = get_lane_change_command(currentID);


			if (((j == (platoons.size())-1) && (command == 2)) || ((j == 0) && (command == 1)) || (command == 0)){
				//Wants to go straight or Edge commands (trying to turn into non existent lanes)
				PotentialChange.push_back(currentpointer);
				currentpointer->FutureDirection = 0;


			}else if (command == 1){
				//Wants to turn left //
				PotentialChange.push_back(currentpointer);
				currentpointer->FutureDirection = command;


			}else if (command == 2){
				//Wants to turn right
				PotentialChange.push_back(currentpointer);
				currentpointer->FutureDirection = command;
			}

			//Move to the next car in the platoon
			currentpointer = currentpointer->get_prev();

		}	

	}	


	//Lane changes for cars already in the vector list
	for (unsigned int k = 0; k<PotentialChange.size();k++){

		if (((PotentialChange[k]->FutureDirection == 0)) && (platoons[PotentialChange[k]->lane].carHereLANE((PotentialChange[k]->get_position()) + 1) == -1 )){

			//When going staight, checks for space after cars are being inserted in order
			PotentialChange[k]->set_position((PotentialChange[k]->get_position())+1);

		}else if (PotentialChange[k]->FutureDirection == 1){

			//Car wants to turn left, and we are checking again if a car has taken the spot after cars have been moved around

			if ((platoons[(PotentialChange[k]->lane) - 1].carHereLANE((PotentialChange[k]->get_position())) != 0)){

				//There is a space to the left so it moves to the left
				platoons[PotentialChange[k]->lane].remove(PotentialChange[k]);
				platoons[(PotentialChange[k]->lane)-1].insert(PotentialChange[k],PotentialChange[k]->get_position());

			}else {

				//There is no space to the left, so we set the direction to 0 and recheck this car to see if it can move forward
				PotentialChange[k]->FutureDirection = 0;
				k--;
			}
		}else if (PotentialChange[k]->FutureDirection == 2){

			if ((platoons[(PotentialChange[k]->lane) + 1].carHereLANE((PotentialChange[k]->get_position())) != 0)){
				//There is a space to the right so it moves to the right
				platoons[PotentialChange[k]->lane].remove(PotentialChange[k]);
				platoons[(PotentialChange[k]->lane)+1].insert(PotentialChange[k], PotentialChange[k]->get_position());

			} else {

				//There is no space to thr right, so we set the direction to 0 and recheck this car to see if it can move forward
				PotentialChange[k]->FutureDirection = 0;
				k--;
			}
		}

	}

}

//Returns true if the Car ID is already in the list - i.e the car has already moved this tick
bool TrafficModel::CheckCarIDS(vector<int> MovedCarIDS, int carID){
	//Automatically set as false - i.e not in the list of cars that have been already moved in this tick
	bool CarIsIn = false;
	for (unsigned int i=0;i<MovedCarIDS.size();i++){
		if (carID == MovedCarIDS[i]){
			CarIsIn = true;
		}
	}
	return CarIsIn;
}

/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					"," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}
