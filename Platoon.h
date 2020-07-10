#ifndef _Platoon_H_
#define _Platoon_H_

#include <vector>
#include <string>
#include <sstream>
#include "Car.h"

using namespace std;

class Platoon
{
    private:
        Car* head = NULL;
        Car* tail = NULL;

    public:
        Platoon(string init);
        Car* get_tail();
        Car* get_head();

        //Custom Functions
        
        void remove(Car* c); //Remove a car with specific id from the platoon 
        void addtoend(Car* c); //Add car object to end of the linked list
        void addtostart(Car* c); //Add car object to start of linked list -- new cars are added to the start/head of the linked list by default
        int getplatoonsize(); //Returns the current size of the platoon 
        int carHereLANE(int pos);   //Returns the car ID currently at specific position in the lane (for update)
        Car* carHerePLAT(int pos);   //Returns the car ID currently at specific position in the platoon (for insert)
        void insert(Car* c, int pos); //Insert a car into platoon
      
};
#endif
