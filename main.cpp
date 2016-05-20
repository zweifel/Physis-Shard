
#include"stdio.h"
#include"stdlib.h"

//agents
#include"agents/Unified_Neural_Model.h"

//environments
#include"environments/Function_Approximation.h"
#include"environments/Single_Cart_Pole.h"
#include"environments/Double_Cart_Pole.h"
#include"environments/Mountain_Car.h"
#include"environments/Multiplexer.h"

#include"parameters.h"

FILE* main_log_file;

double computeAverage(double* last_rewards, int counter)
{
	int k;

	if(counter>100)
	{
		counter=100;
	}

	double avg_rewards=0.0;
	//printf("AAA\n");
	for(k=0;k<counter;++k)
	{
		//printf("%f\n",last_rewards[k]);
		avg_rewards+= last_rewards[k];
	}
	avg_rewards= avg_rewards/(double)counter;

	return avg_rewards;
}

void setFeatures(Reinforcement_Environment* env)
{
	
#ifdef SET_NORMALIZED_INPUT
	bool feature_available;
	
	feature_available= env->set(NORMALIZED_OBSERVATION);

	if(feature_available == false)
	{
		printf("NORMALIZED_OBSERVATION feature not available\n");
		exit(1);
	}
	else
	{
		fprintf(main_log_file,"Normalized Observation enabled\n");
	}
#endif

#ifdef SET_NORMALIZED_OUTPUT
	bool feature_available;
	
	feature_available= env->set(NORMALIZED_ACTION);

	if(feature_available == false)
	{
		printf("NORMALIZED_ACTION feature not available\n");
		exit(1);
	}
	else
	{
		fprintf(main_log_file,"Normalized Action enabled\n");
	}
#endif

}

int main()
{
	//int trials_to_change_maze_states= 10000;
	int i;
	main_log_file= fopen("log.txt","w");

	Random* random= new State_of_Art_Random(time(NULL));
	
	//Reinforcement_Environment* env= new Mountain_Car(random);
	Reinforcement_Environment* env= new Function_Approximation(random,1000,false);
	//Reinforcement_Environment* env= new Single_Cart_Pole(random);
//Reinforcement_Environment* env= new Double_Cart_Pole(random);
	//Reinforcement_Environment* env= new Multiplexer(3,8,random);

	//Reinforcement_Agent* agent= new Dummy(env);
	Reinforcement_Agent* agent= new Unified_Neural_Model(random);
	
	setFeatures(env);

	//Self_Organizing_Neurons* b= (Self_Organizing_Neurons*)agent;

	//print max accumulated reward seen in N trials, the N trials is given by trial_frequency_to_print
	bool print_max_accum_reward_in_n_trials= true;
	int trial_frequency_to_print= 100;
	double max_accum_reward=0;
	bool was_initialized=false;	//tells if the max_accum_reward was initialized

	bool print_reward=false;
	bool print_step=false;
	bool print_average=false;
	//bool print_accumulated_reward=true;
	bool print_agent_information=false;

	//int trials=100000
	int trials=200000;		
	//int trials=200;		
	//int trials=500;		
	//int trials=100000;		
	
	int number_of_observation_vars;
	int number_of_action_vars;
	
	env->start(number_of_observation_vars, number_of_action_vars);
	agent->init(number_of_observation_vars, number_of_action_vars);
	
	//starting reward 
	double reward= env->step(NULL);		
	double step_counter=1;
		
	//agent->print();
		
	double last_rewards[100];
	int counter=0;
	double avg_rewards;

	
	for(i=env->trial;i<trials;)
	{
		double accum_reward=reward;
		//do one trial (multiple steps until the environment finish the trial or the trial reaches its MAX_STEPS)
		while(env->trial==i && step_counter <= env->MAX_STEPS)
		{

			agent->step(env->observation, reward);

			reward= env->step(agent->action);		
		
			accum_reward+= reward;
		
			if(print_reward)
			{
				last_rewards[counter%100]=reward;
				counter++;
				
				if(print_average)
				{
					avg_rewards= computeAverage(last_rewards, counter);
					printf("%d %f\n",i, avg_rewards);
				}
				else
				{
					printf("%d %f\n",i, reward);
				}

			}
		
			step_counter++;

		}

#ifdef TERMINATE_IF_MAX_STEPS_REACHED
		//end evolution when the MAX_STEPS is reached
		if(step_counter > env->MAX_STEPS)
		{
			i=trials;
		}
#endif
	
		//update the max_accum_reward and print		
		if(print_max_accum_reward_in_n_trials)
		{	
			if(was_initialized==false)
			{
				was_initialized=true;
				max_accum_reward= accum_reward;
			}
			else
			{
				if(max_accum_reward < accum_reward)
				{
					max_accum_reward= accum_reward;
				}
			}		
		
			if(i%trial_frequency_to_print==0)
			{	
				printf("%d %f\n",i, max_accum_reward);
		
				max_accum_reward=0;
				was_initialized=false;

			}
		}
	
		agent->endEpisode(reward);

			
		//if env->trial is the same as i, it means that the internal state of the environment has not changed
		//then it needs a restart to begin a new trial
		if(env->trial==i)
		{
			reward= env->restart();
		}
		else
		{
			reward= env->step(NULL);
		}
	
		//print the number of steps used in the last trial
		if(print_step)
		{
			last_rewards[counter%100]=step_counter;
			counter++;

			if(print_average)
			{
				avg_rewards= computeAverage(last_rewards, counter);
				printf("%d %f\n",i, avg_rewards);
			}
			else
			{
				printf("%d %f\n",i, step_counter);
			}
		}
		
		if(print_agent_information==true)
		{
			agent->print();
		}

		step_counter=1;

		i++;

	
	}

	agent->saveAgent("dna_best_individual");
	


	//printf("reward average %f\n",reward_sum/(double)trials);
	//printf("step average %f\n",step_sum/(double)trials);
	
	fclose(main_log_file);	

	return 0;
}

