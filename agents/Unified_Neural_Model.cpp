
#include"Unified_Neural_Model.h"

Unified_Neural_Model::Unified_Neural_Model(Random* random)
{
	this->random= random;

	Module::setRandom(random);
	
	testing_subpop= 0;
	testing_individual= 0;	
	generation=1;
	best_index=0;

#ifdef	SPECTRUM_DIVERSITY
	nmap= new Novelty_Map(NOVELTY_MAP_SIZE , SPECTRUM_SIZE);
	
		
	if(NOVELTY_MAP_SIZE >= SUBPOPULATION_SIZE)
	{
		printf("ERROR: Novelty map's size is bigger or equal to the subpopulation size\n");
		exit(1);
	}
	
	if(NUMBER_OF_SUBPOPULATIONS != 1)
	{
		printf("ERROR: Number of subpopulations is different than 1\n");
		exit(1);
	}
#endif

	step_counter=0;
}

Unified_Neural_Model::~Unified_Neural_Model()
{
	free(action);
	
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			delete subpopulation[i][j];
		}
		delete subpopulation[i];
		free(fitness[i]);
	}
	
	delete subpopulation;
	free(fitness);
}

/*
void Unified_Neural_Model::savePopulation()
{
	printf("Saving population\n");
	
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			subpopulation[i][j]->saveDNA();		
		}
	}
}
*/
		
void Unified_Neural_Model::saveAgent(const char* filename)
{
	//printf("Saving best agent model\n");
	
	//execute individual
	subpopulation[MAIN_SUBPOP][best_index]->saveDNA(filename);		

	subpopulation[MAIN_SUBPOP][best_index]->printGraph("best_individual.dot");		
	
	//char name[64];
	//sprintf(name,"%s_printed",filename);
	//subpopulation[MAIN_SUBPOP][best_index]->printDNA(name);		
		
}

void Unified_Neural_Model::loadAgent(const char* filename)
{
	//printf("Loading agent model\n");
	
	//printf("primer list\n");
	//printArray(subpopulation[MAIN_SUBPOP][best_index]->primer_list, subpopulation[MAIN_SUBPOP][best_index]->number_of_primers);
	
	//execute individual
	subpopulation[MAIN_SUBPOP][best_index]->loadDNA(filename);	

	//subpopulation[MAIN_SUBPOP][best_index]->printDNA();	

	
	subpopulation[MAIN_SUBPOP][best_index]->clearMemory();		
	
	//char name[64];
	//sprintf(name,"%s_loaded_printed",filename);

	//subpopulation[MAIN_SUBPOP][best_index]->printDNA(name);		

}

void Unified_Neural_Model::init(int number_of_observation_vars, int number_of_action_vars)
{
	this->number_of_observation_vars= number_of_observation_vars;
	this->number_of_action_vars= number_of_action_vars;

	action= (double*) calloc(number_of_action_vars, sizeof(double));

	//random subpopulation	
	subpopulation= new Module**[NUMBER_OF_SUBPOPULATIONS];		
	tmp_subpopulation= new Module**[NUMBER_OF_SUBPOPULATIONS];		
	fitness= (double**)malloc(sizeof(double*)*NUMBER_OF_SUBPOPULATIONS);
	tmp_fitness= (double**)malloc(sizeof(double*)*NUMBER_OF_SUBPOPULATIONS);
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		subpopulation[i]= new Module*[SUBPOPULATION_SIZE];
		tmp_subpopulation[i]= new Module*[SUBPOPULATION_SIZE];
		fitness[i]= (double*)malloc(sizeof(double)*SUBPOPULATION_SIZE);
		tmp_fitness[i]= (double*)malloc(sizeof(double)*SUBPOPULATION_SIZE);
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			subpopulation[i][j]= new Module(number_of_observation_vars, number_of_action_vars, INITIAL_ALLOCATION_LENGTH);
			tmp_subpopulation[i][j]= new Module(number_of_observation_vars, number_of_action_vars, INITIAL_ALLOCATION_LENGTH);

			fitness[i][j]= EXTREME_NEGATIVE_REWARD;

			//starting mutations
			for(int k=0; k < NUMBER_OF_INITIAL_MUTATIONS; ++k)
			{
				subpopulation[i][j]->structuralMutation();
			}
			subpopulation[i][j]->updatePrimerList();
		}
	}

	
}

void Unified_Neural_Model::step(double* observation, double reward)
{
	//execute individual
	subpopulation[testing_subpop][testing_individual]->process(observation, action);		

	//update reward
	tmp_fitness[testing_subpop][testing_individual]+= reward;
}
		
void Unified_Neural_Model::endEpisode(double reward)
{
	//update reward
	tmp_fitness[testing_subpop][testing_individual]+= reward;

	step_counter++;
	

	//printf("%f\n",tmp_fitness[testing_subpop][testing_individual]);

	//average 
	if(step_counter >= EPISODES_PER_INDIVIDUAL)
	{
		tmp_fitness[testing_subpop][testing_individual]/= (double) step_counter;
		step_counter=0;

	}
	else
	{
		return;
	}
	
	subpopulation[testing_subpop][testing_individual]->clearMemory();		
	
	//test a new individual from this subpopulation
	testing_individual++;

	//change subpopulation if all individuals from this subpop were already tested
	//evolve if all subpopulations and all individuals were tested (evaluated)
	if(testing_individual >= SUBPOPULATION_SIZE)
	{
		testing_subpop++;
		if(testing_subpop >= NUMBER_OF_SUBPOPULATIONS)
		{
			testing_subpop=0;

			//Evolve
#ifdef	SPECTRUM_DIVERSITY
			spectrumDiversityEvolve();
#else
			evolve();
#endif
			//supremacistEvolve();
		}

		testing_individual=0;
	}

}

void Unified_Neural_Model::print()
{
	subpopulation[MAIN_SUBPOP][best_index]->printGraph("best_individual.dot");		
}

double Unified_Neural_Model::stepBestAction(double* observation)
{

	//execute individual
	subpopulation[MAIN_SUBPOP][best_index]->process(observation, action);		

	return 0;	
}


void Unified_Neural_Model::endBestEpisode()
{
	subpopulation[MAIN_SUBPOP][best_index]->clearMemory();		
}


void Unified_Neural_Model::supremacistEvolve()
{
	//update fitness
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			fitness[i][j]= tmp_fitness[i][j];
		
			//mark this subpopulation as without any individuals inserted
			//selected_individuals[i][j][0]=-1;
			
			//reset the fitness
			tmp_fitness[i][j]= 0.0;;
		}
	}
	

	for(int s= 0; s < NUMBER_OF_SUBPOPULATIONS; ++s)
	{
		int best_index_ind=0;
		int best_index_sub=0;
		double best_objective= subpopulationObjective(subpopulation[0][0], fitness[0][0], s);
		//update fitness
		for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
		{
			for(int j=0;j<SUBPOPULATION_SIZE; ++j)
			{
				double objective= subpopulationObjective(subpopulation[i][j], fitness[i][j], s);
			
				if(best_objective < objective)
				{
					best_index_ind= j;
					best_index_sub= i;
					best_objective= objective;	
				}	
			}
		}
		selected_individuals[s][0][0]= best_index_sub;
		selected_individuals[s][0][1]= best_index_ind;
	
	}


	printSubpop();

	//Create the new population!!
	//
	//for the time being, move the fittest individuals to a temporary subpopulation
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		int subpopulation_index= selected_individuals[i][0][0];
		int individual_index= selected_individuals[i][0][1];

		tmp_subpopulation[i][0]->clone(subpopulation[subpopulation_index][individual_index]);
		
		//create new individuals
		for(int j=1;j < SUBPOPULATION_SIZE; ++j)
		{
			//int random_individual= random->uniform(0,SUBPOPULATION_SIZE/2);
			tmp_subpopulation[i][j]->clone(subpopulation[i][0]);

			int number_of_mutations= random->uniform(-NUMBER_OF_STEP_MUTATIONS, NUMBER_OF_STEP_MUTATIONS);

			//structural mutation
			for(int k=0; k < number_of_mutations; ++k)
			{
				tmp_subpopulation[i][j]->structuralMutation();
			}
			tmp_subpopulation[i][j]->updatePrimerList();

			//weight mutation
			tmp_subpopulation[i][j]->weightMutation();
		}
	}

	//swap temporary population with the original one	
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			Module* swap_individual= subpopulation[i][j];
			subpopulation[i][j]= tmp_subpopulation[i][j];
			tmp_subpopulation[i][j]= swap_individual;
		}
	}
		
	generation++;



	//find the index of the best individual
	best_index=0;
	//printBest();

	
}

void Unified_Neural_Model::spectrumDiversityEvolve()
{
	//update fitness
	double avg_fitness=0;
	double fcounter=0;
	double best_fitness= tmp_fitness[0][0];
	//int previous_best_index= best_index;
	best_index= 0;
	int best_number_of_neurons= subpopulation[0][0]->number_of_neurons;		
		
	//find best individual	
	//printf("\n\n\nprevious best index %d %f\n", previous_best_index, fitness[0][previous_best_index]);
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			fitness[i][j]= tmp_fitness[i][j];

			if(tmp_fitness[i][j] > best_fitness )
			{
				best_fitness= tmp_fitness[i][j];
				best_index=j;
				best_number_of_neurons= subpopulation[i][j]->number_of_neurons;		
			}
			else
			{
				//if they have the same fitness only updtate if the new individual have the same or less number of neurons
				if(tmp_fitness[i][j] == best_fitness )
				{
					if(best_number_of_neurons >= subpopulation[i][j]->number_of_neurons)
					{
						best_fitness= tmp_fitness[i][j];
						best_index=j;
						best_number_of_neurons= subpopulation[i][j]->number_of_neurons;		

					}
				}
			}
		
			//mark this subpopulation as without any individuals inserted
			//selected_individuals[i][j][0]=-1;
			
			//reset the fitness
			tmp_fitness[i][j]= 0.0;
	
			//printf("%d %f\n", j, fitness[i][j]);
			avg_fitness+= fitness[i][j];
			fcounter++;
		}
	}
	avg_fitness/= fcounter;

//	printf("best index %d best fitness %f\n",best_index, best_fitness);

//	char filename[64];
//	sprintf(filename,"dna_%d",generation);

	//saveAgent("test_agent");
//	saveAgent(filename);

	//fflush(NULL);

		
/*	int read_fd[2];
	int write_fd[2];
        pipe(read_fd);
        pipe(write_fd);
*/
/*        //spawn a child 
        pid_t pid=fork();
		double testing_result;

        //child process
        if(pid==0)
        {

*/
/*		dup2(write_fd[0], STDIN_FILENO);
		dup2(read_fd[1], STDOUT_FILENO);

		close((read_fd[0]));
		close((read_fd[1]));
		close((write_fd[0]));
		close((write_fd[1]));
*/		
/*		execl("/home/ask/code/rl_debugging/live","/home/ask/code/rl_debugging/live","test_agent",NULL);
	
		sleep(1);

		exit(1);
	}
	else
	{
		int status;
		waitpid(pid,&status,0);
		
		FILE*fp1= fopen("comp","r");
		
		fscanf(fp1,"%lf",&testing_result);
		printf("testing %f\n",testing_result);
		fclose(fp1);
	}
*/
	//decide the parents
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			//calculate spectrum
			double spectrum[SPECTRUM_SIZE];	
	

			calculateSpectrum(spectrum, i,j);

			//printArray(spectrum,SPECTRUM_SIZE);

			//insert in Novelty Map
			int index= nmap->input(spectrum);

			nmap_cell* cell= (nmap_cell*)(nmap->map[index]).pointer;
			double this_fitness= fitness[i][j];
			
			if(cell==NULL)
			{
				cell= (nmap_cell*)malloc(sizeof(nmap_cell));
				nmap->map[index].pointer= cell;
				cell->module=NULL;
			}	
			
			//check if nothing was inserted
			if(cell->module==NULL)
			{
				cell->module= subpopulation[i][j];		
				cell->fitness= this_fitness;
			}	
			else
			{
				if(cell->fitness < this_fitness)
				{
					cell->module= subpopulation[i][j];		
					cell->fitness= this_fitness;			
				}
				else
				{
					//if they have the same fitness only updtate if the new individual have the same or less number of neurons
					if(cell->fitness == this_fitness )
					{
						if((cell->module)->number_of_neurons >= subpopulation[i][j]->number_of_neurons)
						{
							cell->module= subpopulation[i][j];		
							cell->fitness= this_fitness;			
						}
					}
				}
			}
		
		}
	}
	
		
	nmap_cell* cell= (nmap_cell*)(nmap->map[0]).pointer;
	best_fitness= cell->fitness;
	best_index= 0;

	//copy parents (nmap's cells) to the population
	for(int i=0;i<NOVELTY_MAP_SIZE;++i)
	{	
		cell= (nmap_cell*)(nmap->map[i]).pointer;
		
		if(cell->module!=NULL)
		{
			if(cell->fitness> best_fitness)
			{
				best_fitness=cell->fitness;
				best_index=i;
			}
		}
		
		//printf("%d %p %p %p cell %d %d\n",i, tmp_subpopulation[0][i], cell->module, subpopulation[cell->i][cell->j], cell->i, cell->j);
		//prevent the algorithm from choosing empty cells (those cells appear when there is many cells with the same weight array)
		while(cell->module==NULL)
		{	
			//printf("%d %p is null\n",i, cell->module);
			int roullete= random->uniform(0,NOVELTY_MAP_SIZE-1);
			cell= (nmap_cell*)(nmap->map[roullete]).pointer;
			//printf("%d %p is null\n",i, cell->module);
		}	

		tmp_subpopulation[0][i]->clone(cell->module);

		
		//tmp_subpopulation[0][i]->clone(subpopulation[cell->i][cell->j]);

	}

// 	verbose
	//printf("%d new index %d fitness %f avg_fitness %f\n", generation, best_index, best_fitness, avg_fitness);

	//reproduce 
	for(int i=NOVELTY_MAP_SIZE;i<SUBPOPULATION_SIZE;++i)
	{
		int random_individual= random->uniform(0,NOVELTY_MAP_SIZE-1);
		cell= (nmap_cell*)(nmap->map[random_individual]).pointer;
		
		while(cell->module==NULL)
		{	
			int roullete= random->uniform(0,NOVELTY_MAP_SIZE-1);
			cell= (nmap_cell*)(nmap->map[roullete]).pointer;
		}	
			
		//tmp_subpopulation[0][i]->clone(subpopulation[0][random_individual]);
		tmp_subpopulation[0][i]->clone(cell->module);

		//int number_of_mutations= random->uniform(1, NUMBER_OF_STEP_MUTATIONS);
		int number_of_mutations= NUMBER_OF_STEP_MUTATIONS;

		//structural mutation
		for(int k=0; k < number_of_mutations; ++k)
		{
			tmp_subpopulation[0][i]->structuralMutation();
		}
		tmp_subpopulation[0][i]->updatePrimerList();
		

		//weight mutation
		tmp_subpopulation[0][i]->weightMutation();
	}
	
	//remove all modules inserted in the novelty map
	for(int i=0;i<NOVELTY_MAP_SIZE;++i)
	{	
		cell= (nmap_cell*)(nmap->map[i]).pointer;
		cell->module=NULL;
		cell->fitness=EXTREME_NEGATIVE_REWARD;
	}

/*	
	for(int i=0;i<SUBPOPULATION_SIZE;++i)
	{
		tmp_subpopulation[0][i]->clone(subpopulation[0][i]);
	}
*/	
	//swap temporary population with the original one	
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			Module* swap_individual= subpopulation[i][j];
			subpopulation[i][j]= tmp_subpopulation[i][j];
			tmp_subpopulation[i][j]= swap_individual;
		}
	}

	generation++;	

	//double testing_result;	
	//scanf("%f",&testing_result);

	//if(generation>1000)
	//{
	//	exit(1);
	//}
}


// Create the Spectrum of the DNA
//
// | Identity | Sigmoid | Threshold | Random | Control | Slow |
void Unified_Neural_Model::calculateSpectrum(double* spectrum, int subpopulation_index, int individual_index)
{
	Module* mod= subpopulation[subpopulation_index][individual_index];		

	//clear spectrum
	for(int i=0;i<SPECTRUM_SIZE;++i)
	{
		spectrum[i]=0;
	}
	
	//execute all Control Neurons that are excited and not activated
	int counter=0;
	for(int i=0; mod->n[i].id >= 0;++i)
	{
		switch(mod->n[i].type)
		{
			case IDENTITY:
			{
				spectrum[0]++;
			}
			break;

			case SIGMOID:
			{
				spectrum[1]++;
			}
			break;
			
			case THRESHOLD:
			{
				spectrum[2]++;
			}
			break;
			
			case RANDOM:
			{
				spectrum[3]++;
			}
			break;
			
			case CONTROL:
			{
				spectrum[4]++;
			}
			break;
		}
		
		if(mod->n[i].firing_rate != 1)
		{
			spectrum[5]++;
		}
				
		counter++;
	}

#ifdef NORMALIZED_SPECTRUM_DIVERSITY
	for(int i=0;i<SPECTRUM_SIZE;++i)
	{
		spectrum[i]/=(double)counter;
	}
	//printArray(spectrum,SPECTRUM_SIZE);
#endif
	
}

void Unified_Neural_Model::evolve()
{
	//printf("generation %d\n",generation);

	//update fitness
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			fitness[i][j]= tmp_fitness[i][j];
		
			//mark this subpopulation as without any individuals inserted
			selected_individuals[i][j][0]=-1;
		
			//reset the fitness
			tmp_fitness[i][j]= 0.0;;
		}
	}
	
	//worst fitness in the subpop
	//double fitness_cut[NUMBER_OF_SUBPOPULATIONS];	
	//individual with worst fitness in the subpop
	//double individual_fitness_cut[NUMBER_OF_SUBPOPULATIONS];	


	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			for(int k= 0; k < NUMBER_OF_SUBPOPULATIONS; ++k )
			{
				tryToInsertInSubpopulation(i,j, k);
			}
		}
	}

	//find the index of the best individual
	findBestIndividual();

	printSubpop();
	printBest();
	
	//Create the new population!!
	//
	//for the time being, move the fittest individuals to a temporary subpopulation
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		int j;
		for(j=0;j<SUBPOPULATION_SIZE/2; ++j)
		{
			int subpopulation_index= selected_individuals[i][j][0];
			int individual_index= selected_individuals[i][j][1];

			tmp_subpopulation[i][j]->clone(subpopulation[subpopulation_index][individual_index]);
		}
		
		//create new individuals
		for(;j < SUBPOPULATION_SIZE; ++j)
		{
			//int random_individual= random->uniform(0,SUBPOPULATION_SIZE/2);
			tmp_subpopulation[i][j]->clone(subpopulation[i][j - (SUBPOPULATION_SIZE/2)]);

			int number_of_mutations= random->uniform(-NUMBER_OF_STEP_MUTATIONS, NUMBER_OF_STEP_MUTATIONS);

			//structural mutation
			for(int k=0; k < number_of_mutations; ++k)
			{
				tmp_subpopulation[i][j]->structuralMutation();
			}
			tmp_subpopulation[i][j]->updatePrimerList();

			//weight mutation
			tmp_subpopulation[i][j]->weightMutation();
		}
	}

	//swap temporary population with the original one	
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		for(int j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			Module* swap_individual= subpopulation[i][j];
			subpopulation[i][j]= tmp_subpopulation[i][j];
			tmp_subpopulation[i][j]= swap_individual;
		}
	}
		
	generation++;
		
}

void Unified_Neural_Model::findBestIndividual()
{
	int i=0;
	int subpopulation_index= selected_individuals[MAIN_SUBPOP][i][0];
	int individual_index= selected_individuals[MAIN_SUBPOP][i][1];
	int best_complexity= subpopulation[subpopulation_index][individual_index]->number_of_connections;
	double best_objective=  subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], MAIN_SUBPOP);
	best_index=0;

	for(i=1;i<SUBPOPULATION_SIZE/2; ++i)
	{
		subpopulation_index= selected_individuals[MAIN_SUBPOP][i][0];
		individual_index= selected_individuals[MAIN_SUBPOP][i][1];
		int complexity= subpopulation[subpopulation_index][individual_index]->number_of_connections;
		double objective=  subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], MAIN_SUBPOP);
	
		if(objective > best_objective)
		{
			best_index=i;
			best_objective= objective;
			best_complexity= complexity;
		}
		else
		{
			if(objective == best_objective)
			{
				//occam's razor
				if(complexity < best_complexity)
				{
					best_index=i;
					best_objective= objective;
					best_complexity= complexity;

				}
			}
		}
	}
	
}

void Unified_Neural_Model::tryToInsertInSubpopulation(int subpopulation_index, int individual_index, int inserting_subpop)
{
	double objective= subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], inserting_subpop);

	//printf("try to insert %d:%d %f\n",subpopulation_index, individual_index, objective);	

	if(selected_individuals[inserting_subpop][0][0]==-1)
	{
		selected_individuals[inserting_subpop][0][0]= subpopulation_index;
		selected_individuals[inserting_subpop][0][1]= individual_index;
		
		//printf("inserted\n");

		return;
	}

	int sub= selected_individuals[inserting_subpop][0][0];
	int ind= selected_individuals[inserting_subpop][0][1];
	double worst_objective= subpopulationObjective(subpopulation[sub][ind], fitness[sub][ind], inserting_subpop);
	int worst_index=0;

	for(int i=1;i<(SUBPOPULATION_SIZE/2); ++i)
	{
		//if there is no individuals inserted
		if(selected_individuals[inserting_subpop][i][0]==-1)
		{
			selected_individuals[inserting_subpop][i][0]= subpopulation_index;
			selected_individuals[inserting_subpop][i][1]= individual_index;

			//printf("inserted\n");
			return;
		}
		else
		{
			int sub= selected_individuals[inserting_subpop][i][0];
			int ind= selected_individuals[inserting_subpop][i][1];
			
			double tmp_objective= subpopulationObjective(subpopulation[sub][ind], fitness[sub][ind], inserting_subpop);

			if(worst_objective > tmp_objective)
			{
				worst_objective= tmp_objective;
				worst_index= i;
			}	
		}
	}
	

	if(objective > worst_objective)
	{
		selected_individuals[inserting_subpop][worst_index][0]=subpopulation_index;
		selected_individuals[inserting_subpop][worst_index][1]=individual_index;
	
		double objective= subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], inserting_subpop);
		

		//telling the compiler that we are not going to use the objective variable for now
		(void) objective;
		
		//printf("inserted in place of %d:%f\n",worst_index, worst_objective);
		//printf("now inserted in place of %d:%f\n",worst_index, objective);
	}
}

double Unified_Neural_Model::subpopulationObjective(Module* module, double fitness, int subpopulation_index)
{
	switch(subpopulation_index)
	{
		case FITNESS_BASED:
		{
			return fitness;
		}
		break;	
		case NEURON_EFFICIENT:
		{
			double objective= fitness - (double)module->number_of_neurons;		
			return objective;	
		}	
		break;	
		case CONNECTION_EFFICIENT:
		{
			double objective= fitness - (double)module->number_of_connections;			
			return objective;
		}
		break;	
		case NEURON_RICH:
		{
			double objective= fitness + (double)module->number_of_neurons;			
			return objective;

		}
		break;	
		case CONNECTION_RICH:
		{
			double objective= fitness + (double)module->number_of_connections;			
			return objective;

		}
		break;	
	}

	printf("ERROR: subpopulation objectie with unknown subpopulation index %d\n",subpopulation_index);
	exit(1);
	return 0;	
}
	
void Unified_Neural_Model::printSubpop()
{
	printf("Fitness of the subpops\n");
	//for the time being, move the fittest individuals to a temporary subpopulation
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		int j;
		for(j=0;j<SUBPOPULATION_SIZE; ++j)
		{
			double objective= subpopulationObjective(subpopulation[i][j], fitness[i][j], i);
			double main_objective= subpopulationObjective(subpopulation[i][j], fitness[i][j], MAIN_SUBPOP);

			printf("%d:%d current objective %f main objective: %f\n",i,j, objective, main_objective);	
		}
	}
	
	
	printf("\nFitness of best individuals in each subpop\n");
	//for the time being, move the fittest individuals to a temporary subpopulation
	for(int i= 0; i < NUMBER_OF_SUBPOPULATIONS; ++i )
	{
		int j;
		//for(j=0;j<SUBPOPULATION_SIZE/2; ++j)
		//for(j=0;j<selected_individuals[i][j][0]>=0; ++j)
		j=0;
		{
			int subpopulation_index= selected_individuals[i][j][0];
			int individual_index= selected_individuals[i][j][1];
	
			double objective= subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], i);

			printf("%d:%d %f   %f\n",i,j, fitness[subpopulation_index][individual_index], objective);	
		}
	}
	
	printf("\nBest individual:\n");
	int subpopulation_index= selected_individuals[MAIN_SUBPOP][best_index][0];
	int individual_index= selected_individuals[MAIN_SUBPOP][best_index][1];
	double objective= subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], MAIN_SUBPOP);
	printf("%d  %d:%d %f\n",generation, MAIN_SUBPOP, best_index, objective);

	
	subpopulation[subpopulation_index][individual_index]->printGraph("best_graph.dot");


}

void Unified_Neural_Model::printBest()
{
	int subpopulation_index= selected_individuals[MAIN_SUBPOP][best_index][0];
	int individual_index= selected_individuals[MAIN_SUBPOP][best_index][1];
	double objective= subpopulationObjective(subpopulation[subpopulation_index][individual_index], fitness[subpopulation_index][individual_index], MAIN_SUBPOP);
	printf("%d %f\n", generation, objective);

}
