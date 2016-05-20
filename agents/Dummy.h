
// Dummy = controlled by humans

#ifndef DUMMY_H
#define DUMMY_H

#include"Reinforcement_Agent.h"
#include"../environments/Reinforcement_Environment.h"
#include"stdlib.h"
#include"stdio.h"
#include"time.h"
#include"useful/useful_utils.h"

class Dummy : public Reinforcement_Agent
{
	public:

		Dummy(Reinforcement_Environment* env);
		~Dummy();

		//All Reinforcement Agents have the following commented variables, although it is not declared here!
		//double* action;
		//int number_of_observation_vars;
		//int number_of_action_vars;
		
		//Random* random;
		Reinforcement_Environment* env;

		//Implementing the Reinforcement Agent Interface
		void init(int number_of_observation_vars, int number_of_action_vars);
		void step(double* observation, double reward);
		void print();
		double stepBestAction(double* observation);
		void endEpisode();
		void saveAgent(const char* filename);
		void loadAgent(const char* filename);

};

#endif
