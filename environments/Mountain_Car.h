
#ifndef MOUNTAIN_CAR
#define MOUNTAIN_CAR

#include"../parameters.h"
#include"Reinforcement_Environment.h"
#include"../agents/Reinforcement_Agent.h"
#include"random/State_of_Art_Random.h"
#include"stdlib.h"
#include"stdio.h"


class Mountain_Car : public Reinforcement_Environment
{

	public:
		Mountain_Car(Random* random);
		~Mountain_Car();
		
		//double* observation;		
		//int number_of_observation_vars;
		//int number_of_action_vars;
		//int trial;

		// Global variables:
		float mcar_position, mcar_velocity;
		int last_change;
		Random* random;
		bool normalized_observation;
		bool normalized_action;
	
		float mcar_min_position;
		float mcar_max_position;
		float mcar_max_velocity;            // the negative of this in the minimum velocity
		float mcar_goal_position;
			
		bool original_value;

		// Profiles
		//void MCarInit();                              // initialize car state
		//void MCarStep(int a);                         // update car state for given action
		//bool MCarAtGoal ();                           // is car at goal?
			
		void start(int &number_of_observation_vars, int& number_of_action_vars);
		double step(double* action);
		double restart();
		void print();
		bool set(int feature);
};

#endif
