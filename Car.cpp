#include "Car.h"
#include <stdlib.h>
using namespace std;
#include <iostream>
		
Car::Car(int id, int pos){
	this->id = id;
	this->position = pos;
}

int Car::get_id()
{
	return id;
}

int Car::get_position()
{
	return position;
}

void Car::set_position(int pos)
{
	this->position = pos;
}

void Car::set_next(Car* v)
{
	next = v;
}

void Car::set_prev(Car* v)
{
	prev = v;
}

Car* Car::get_next()
{
	return next;
}

Car* Car::get_prev()
{
	return prev;
}

