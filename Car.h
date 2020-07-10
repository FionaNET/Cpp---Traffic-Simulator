#ifndef _CAR_H_
#define _CAR_H_

#include <string>

using namespace std;

class Car {

	private:
		int id;
        int position;

        Car* next=NULL;
        Car* prev=NULL;

	public:
        
		Car(int id, int pos);
		int get_id();
		
		int get_position();
		void set_position(int pos);

		void set_next(Car* v);
		void set_prev(Car* v);
		Car* get_next();
		Car* get_prev();
		
		int lane;
		int FutureDirection;

};
#endif
