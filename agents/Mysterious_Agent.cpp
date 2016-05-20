
#include"Mysterious_Agent.h"

Mysterious_Agent::Mysterious_Agent()
{
	random= new State_of_Art_Random(time(NULL));
	
	a=true;
}

Mysterious_Agent::~Mysterious_Agent()
{
	free(action);
}

void Mysterious_Agent::init(int number_of_observation_vars, int number_of_action_vars)
{
	this->number_of_observation_vars= number_of_observation_vars;
	this->number_of_action_vars= number_of_action_vars;

	action= (double*) malloc(number_of_action_vars*sizeof(double));
}

void Mysterious_Agent::step(double* observation, double reward)
{
	int i;

	//just ignore reward and observation 
	//and give a random action! :P
	for(i=0;i<number_of_action_vars;++i)
	{
		action[i]= random->uniform(1.0,9.0);
	}
		
	if(a==true)
	{
	action[0]=0;
	action[1]=1;
	}
	else
	{
	action[0]=1;
	action[1]=0;
	}

	a= !a;
}

void Mysterious_Agent::print()
{

}
		
void Mysterious_Agent::endEpisode(double reward)
{

}

double Mysterious_Agent::stepBestAction(double* observation)
{
	//just ignore reward and observation 
	//and give a random action! :P
	for(int i=0;i<number_of_action_vars;++i)
	{
		action[i]= random->uniform(1.0,9.0);
	}

	return 0;
	
}

void Mysterious_Agent::saveAgent(const char* filename)
{
	printf("saveAgent() not implemented\n");

}

void Mysterious_Agent::loadAgent(const char* filename)
{
	printf("loadAgent() not implemented\n");
}

