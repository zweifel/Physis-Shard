
#ifndef REINFORCEMENT_ENVIRONMENT_H
#define REINFORCEMENT_ENVIRONMENT_H

//features (in fact, they are restrictions to the reinforcement problem)
//	NORMALIZED_OBSERVATION - Observation real values must be in the [-1,1] range
//	NORMALIZED_ACTION - Action real values are supposed to be in the [0,1] range
enum{NORMALIZED_OBSERVATION, NORMALIZED_ACTION};

class Reinforcement_Environment
{
	public:

		//Reinforcement_Problem()=0;
		//~Reinforcement_Problem()=0;
		
		double* observation;		
		int number_of_observation_vars;
		int number_of_action_vars;
		int trial;
		int MAX_STEPS;

		virtual void start(int &number_of_observation_vars, int& number_of_action_vars)=0;
		virtual double step(double* action)=0;
		virtual double restart()=0;
		virtual void print(){};
		virtual bool set(int feature){return false;}; //see enum above for the acceptable features
};

#endif
