

#include"stdio.h"
#include"stdlib.h"
#include"agents/Unified_Neural_Model.h"
#include"agents/Dummy.h"
#include"environments/Function_Approximation.h"
#include"environments/Single_Cart_Pole.h"
#include"environments/Double_Cart_Pole.h"
#include"environments/Mountain_Car.h"
#include"parameters.h"


int main(int argc, char** argv)
{
	int i;

	if(argc <= 1)
	{
		printf("Usage: %s dna_file\n",argv[0]);
		exit(1);
	}

	Random* random= new State_of_Art_Random(time(NULL));
	
	Reinforcement_Environment* env= new Function_Approximation(random,1000,true);
	//Reinforcement_Environment* env= new Mountain_Car(random);
	//Reinforcement_Environment* env= new Single_Cart_Pole(random);
	//Reinforcement_Environment* env= new Double_Cart_Pole(random);
	//Reinforcement_Environment* env= new Multiplexer(2,4,random);

//	Reinforcement_Agent* agent= new Dummy(env);
	Reinforcement_Agent* agent= new Unified_Neural_Model(random);

	int number_of_observation_vars;
	int number_of_action_vars;
	
	env->start(number_of_observation_vars, number_of_action_vars);
	agent->init(number_of_observation_vars, number_of_action_vars);
	
	//starting reward 
	double reward= env->step(NULL);		
	int step_counter=1;
	double reward_sum= reward;
		
	

	agent->loadAgent(argv[1]);

	agent->print();
	

	i=env->trial;
	//do one trial (multiple steps until the environment finish the trial or the trial reaches its MAX_STEPS)
	while(env->trial==i && step_counter <= env->MAX_STEPS)
	{
		
		//printArray(env->observation, number_of_observation_vars);

		agent->stepBestAction(env->observation);

		//printArray(agent->action,number_of_action_vars);
	
		reward= env->step(agent->action);		


		step_counter++;
		reward_sum+= reward;

	}
	
	i=env->trial;
	
	//Unified_Neural_Model* unm= (Unified_Neural_Model*) agent;
	//unm->endBestEpisode();

	FILE* fp= fopen("comp","w");

	printf("step counter %d\n",step_counter);
	printf("accumulated reward %f\n",reward_sum);
	
	fprintf(fp,"%f\n",reward_sum);

	fclose(fp);	
	
//	General_Labyrinth *maze= (General_Labyrinth*)env;
//	maze->printAgentBehavior(agent);

			
	return 0;
}

