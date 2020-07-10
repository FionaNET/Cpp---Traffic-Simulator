#include "Platoon.h"
#include <stdlib.h>
using namespace std;
#include <iostream>

Platoon::Platoon(string init)
{
	// the string 'init' looks like:
	// a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
	stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

			//Sorting starting the platoon in terms of position using my own insert function
			insert(car, pos_value);

		}
	}
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

//To remove a car from platoon
void Platoon::remove(Car* c){


	if (((c->get_id()) == (get_head()->get_id())) && ((c->get_id()) == (get_tail()->get_id()))){
		//If it is the only car in the platoon	
		this->head = NULL;
		this->tail = NULL;

	}else if ((c->get_id()) == (get_head()->get_id())){
		////If it is the head
		Car* nextcar = c->get_next();
		nextcar->set_prev(NULL);
		c->set_next(NULL);
		this->head = nextcar;

	}else if ((c->get_id()) == (get_tail()->get_id())) {
		//If it is the tail
		Car* prevcar = c->get_prev();
		prevcar->set_next(NULL);
		c->set_prev(NULL);
		this->tail = prevcar;

	}else{
		//If it is in the center - between two cars
		//get the next and prev car of car c
		Car* nextcar = c->get_next();
		Car* prevcar = c->get_prev();

		//set left car 'next' = right car id :: however, the position in the platoon does not change
		prevcar->set_next(nextcar);

		//set right car 'prev' = left car id
		nextcar->set_prev(prevcar);

		//remove links from the car itself 
		c->set_next(NULL);
		c->set_prev(NULL);
	}

}

//Add car object to start of linked list (head end)
void Platoon::addtostart(Car* c){

	//Get current head and save as prev for the car
	c->set_next(get_head());

	//Set current head to have a next which is our car 'c'
	get_head()->set_prev(c);

	//Set platoon head to car* c
	this->head = c;

}


//Takes in the position in platoon where you want to insert (lane wise) and the car 'c' and inserts it into position wrt other cars position in lane
void Platoon::insert(Car* c, int pos){	


	if ((get_head() == NULL) && (get_tail() == NULL)){

		//if empty platoon
		this->head = c;
		this->tail = c;

	} else if (pos < (get_head()->get_position()) && (get_head() != NULL)){

		//the position we want to add it is at the head-so position is less than the current head
		addtostart(c);

	}else if (pos > (get_tail()->get_position()) && (get_tail() != NULL)){

		//the position we want to add it into is at the tail - so position is greater than the current tail
		addtoend(c);

	}else {
		//The car is between 2 other cars
		
		//Starting from the head
		Car* ConvertPos = get_head();
		int posinplat = -1;
		int i = 0;
		
		//Convert lane position to platoon position
		while (ConvertPos != NULL && i< getplatoonsize()){

			//Look for the position in platoon when 'pos' is greater than currentcar->get_position(), then next, untilset final pos = currentcar->getposition() -1
			if ((ConvertPos->get_position()) < pos) {

				//If the position where we want to insert is greater than the head position, then we move onto the next item in the platoon
				ConvertPos = ConvertPos->get_next();
				i++;

			}else {

				//The position is less than the position we are comparing, therefore we insert at this position
				posinplat = i;
				ConvertPos = NULL;
				break;
			}

		}

		//Setting links for all cars
		c->set_next(carHerePLAT(posinplat));
		c->set_prev(carHerePLAT(posinplat - 1));
		carHerePLAT(posinplat)->set_prev(c);
		(carHerePLAT(posinplat - 1))->set_next(c);
	}


}



//Add car object to end of the linked list (tail end)
void Platoon::addtoend(Car* c){

	//Get current tail and save as next for the car
	c->set_prev(get_tail());

	//set the current tail to have a previous which is our car 'c'
	get_tail()->set_next(c);

	//Set new platoon tail to car* c
	this->tail = c;

}



//returns size of platoon
int Platoon::getplatoonsize(){

	//Current counter
	int count = 0 ;
	//Pointer that starts at head
	Car* currentcarcount = get_head();

	while (currentcarcount != NULL){
		//Increase count
		count ++ ;
		//Update the new position of the pointer to move towrads the tail end
		currentcarcount = currentcarcount->get_next();
	}

	return count;
}

//returns car id that is currently in the specific position in the lane [NOT PLATOON POSITION] - for update 
int Platoon::carHereLANE(int pos){

	//Pointer that starts at head
	Car* currentcar = get_head();	

	//Keep going through the loop while the position doesn't match and there are cars left in the platoon to check
	while ((currentcar != NULL) && (currentcar->get_next() != NULL) && (pos != currentcar->get_position())){
		//Update the new position of the pointer to move towards the tail
		currentcar = currentcar->get_next();
	}

	//Once it exits the loop, it will either have empty platoon[null], or car at specific position was not found[current sits at tail], or car at position was found
	//Two possibilities when current car sits at the tail: the car at the specific position was not found, or the specific position is the tail pointer

	if (currentcar == NULL){
		//If the lane was empty (or does not exist), return -2
		return -2;

	}else if (pos != (currentcar->get_position())) {
		//If a position was found but doesn't match the position required - therefore means specific position is empty 
		return -1;

	}else {
		//If there was a car found then return 0
		return 0;
	}

}


//returns car* that is currently in the specific position in the platoon [NOT LANE POSITION] - for insert
Car* Platoon::carHerePLAT(int pos){

	//Current counter
	int count = 0;
	//Pointer that starts at head
	Car* currentcar = get_head();


	while ((count != pos) && (count < getplatoonsize()) && (currentcar != NULL)){
		//Update the new position of the pointer
		currentcar = currentcar->get_next();
		//Increase count
		count ++ ;
	}


	//If the head is empty then it should return null
	return currentcar;
}


