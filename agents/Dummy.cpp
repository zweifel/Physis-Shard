
#include"Dummy.h"

Dummy::Dummy(Reinforcement_Environment* env)
{
	//random= new State_of_Art_Random(time(NULL));
	this->env= env;
}

Dummy::~Dummy()
{
	free(action);
}
		
void Dummy::saveAgent(const char* filename)
{
	printf("saveAgent() not implemented\n");

}

void Dummy::loadAgent(const char* filename)
{
	printf("loadAgent() not implemented\n");
}

void Dummy::init(int number_of_observation_vars, int number_of_action_vars)
{
	this->number_of_observation_vars= number_of_observation_vars;
	this->number_of_action_vars= number_of_action_vars;

	action= (double*) malloc(number_of_action_vars*sizeof(double));
}

void Dummy::step(double* observation, double reward)
{
	int i;

	//print environment's information
	env->print();

	//print observation and reward
	printArray(observation, number_of_observation_vars);
	printf("Reward: %f\n",reward);

	//just ignore reward and observation 
	//and give a random action! :P
	for(i=0;i<number_of_action_vars;++i)
	{
		printf("Action %d: ",i);
		scanf("%lf",&(action[i]));
	}
	printArray(action,number_of_action_vars);
}
		
void Dummy::endEpisode()
{
	printf("Episode finished!!!!!!!!!!!\nTime for a beer\n");
}

void Dummy::print()
{

}

double Dummy::stepBestAction(double* observation)
{
	return 0;
}
