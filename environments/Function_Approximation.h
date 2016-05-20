
#ifndef FUNCTION_APPROXIMATION
#define FUNCTION_APPROXIMATION

#include"../parameters.h"
#include"Reinforcement_Environment.h"
#include"../agents/Reinforcement_Agent.h"
#include"random/State_of_Art_Random.h"
#include"stdlib.h"
#include"stdio.h"


class Function_Approximation : public Reinforcement_Environment
{

	public:
		Function_Approximation(Random* random, int evaluations_per_episode, bool verbose);
		~Function_Approximation();
		
		//double* observation;		
		//int number_of_observation_vars;
		//int number_of_action_vars;
		//int trial;
		
		int problem_index;
		int solved_counter;


		// Global variables:
		Random* random;
		int evaluations_per_episode;
		int evaluation;
		bool verbose;
	
		void start(int &number_of_observation_vars, int& number_of_action_vars);
		double step(double* action);
		double restart();
		void print();
};

#endif
