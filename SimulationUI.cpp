#include "SimulationUI.h"

SimulationUI::SimulationUI(string filename)
{
	this->filename = filename;

	ifstream infile (filename.c_str());
	if (!infile.good()){
		cout << "The input.txt file cannot be found" << endl;
		exit(0);
	}
	
	cout << "Simulation has started" << endl;
}

SimulationUI::~SimulationUI(){
}

/*
 * Extracts the maximum time information from the input file
 * Returns the max time value (int)
 */
int SimulationUI::getMaxSimulationTime()
{
	string line;
	ifstream infile (filename.c_str());
	getline(infile, line);
	infile.close();

	return stoi(line);
}

/* 
 * Extracts the initial position information from the input file
 * Returns a vector of strings, where each element is a line in the text file
 */
vector<string> SimulationUI::getInitInfo()
{
	vector<string> output;
	string line;
	ifstream infile (filename.c_str());
	getline(infile, line); // ignore the first line

	while (getline(infile, line))
	{
		std::size_t found = line.find("!");
		if (found!=std::string::npos) break;
		output.push_back(line);
	}

	infile.close();

	return output;
}

/*
 * Extracts the lane change commands from the input file
 * Returns a vector of strings, where each string is a command.
 */ 
vector<string> SimulationUI::getCommands(int tick)
{
	string line;
	ifstream infile (filename.c_str());
	// skip these lines, until "!" is found
	while (getline(infile, line))
	{
		std::size_t found = line.find("!");
		if (found!=std::string::npos) break;
	}

	// now, we read the command lines
	vector<string> commands;

	while(getline(infile, line)){
		string command_time = split(line, ',')[0];
		stringstream ss(command_time);
		int cmd_time;
		ss >> cmd_time;
		if (cmd_time == tick) {
			size_t found = line.find(',');
			string sub = line.substr(found+1, line.size()-1);
			commands.push_back(sub);
		}
 	}
	
	return commands;
}

/*
 * Write the reslut to the result.txt file.
 */ 
void SimulationUI::writeResult(int tick, vector<string> state)
{
	// create the final string
	int count = state.size();
	ostringstream out;
	out << tick;
	for (int i = 0; i < count; i++){
		out << state[i];
	}
	
	if (tick == 0){
		ofstream result;
		result.open("result.txt", ios::out | ios::trunc );
		result << out.str() << endl;
		result.close();
	}
	else{
		ofstream result;
		result.open("result.txt", ios::out | ios::app );
		result << out.str() << endl;
		result.close();
	}
	cout << out.str() << endl;
}

/* A helper function that split a string with a specific delimeter
 * Returns a vector of strings.
 */
vector<string> SimulationUI::split(string line, char delimeter){
	stringstream ss(line);
	string segment;
	vector<string> list;
	while(getline(ss, segment, delimeter))
	{
		list.push_back(segment);
	}
	return list;
}

