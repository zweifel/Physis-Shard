
#ifndef UNIFIED_NEURAL_MODEL_H
#define UNIFIED_NEURAL_MODEL_H

#include"Reinforcement_Agent.h"
#include"../environments/Reinforcement_Environment.h"
#include"stdlib.h"
#include"stdio.h"
#include"time.h"
#include"useful/useful_utils.h"
#include"random/State_of_Art_Random.h"
#include"modules/Module.h"
#include"../parameters.h"
#include"self_organized_systems/Novelty_Map.h"

//#include"unistd.h"
//#include"sys/wait.h"
//subpopulations' size
//#define NEURON_EFFICIENT_SUBPOPULATION_SIZE 10
//#define CONNECTION_EFFICIENT_SUBPOPULATION_SIZE 10
//#define FITNESS_SUBPOPULATION_SIZE 10
//#define NEURON_RICH_SUBPOPULATION_SIZE 10
//#define CONNECTION_RICH_SUBPOPULATION_SIZE 10

typedef struct _nmap_cell
{
	Module* module;
	double fitness;
}nmap_cell;


class Unified_Neural_Model : public Reinforcement_Agent
{
	public:

		Unified_Neural_Model(Random* random);
		~Unified_Neural_Model();

		//All Reinforcement Agents have the following commented variables, although it is not declared here!
		//double* action;
		//int number_of_observation_vars;
		//int number_of_action_vars;
		
		Random* random;

		Module*** subpopulation;
		Module*** tmp_subpopulation;
		double** fitness;
		double** tmp_fitness;
		int testing_individual;
		int testing_subpop;
		int best_index;
		int selected_individuals[NUMBER_OF_SUBPOPULATIONS][SUBPOPULATION_SIZE][2];	
		int generation;
		double step_counter;
#ifdef	SPECTRUM_DIVERSITY
		Novelty_Map* nmap;
#endif
		//auxiliary
		void printBest();
		void evolve();
		void supremacistEvolve();
		void spectrumDiversityEvolve();
		double subpopulationObjective(Module* module, double fitness, int subpopulation_index);
		void findBestIndividual();
		void tryToInsertInSubpopulation(int subpopulation_index, int individual_index, int inserting_subpop);
		void calculateSpectrum(double* spectrum, int subpopulation_index, int individual_index);
		void endBestEpisode();

		//Implementing the Reinforcement Agent Interface
		void init(int number_of_observation_vars, int number_of_action_vars);
		void step(double* observation, double reward);
		void print();
		double stepBestAction(double* observation);
		void endEpisode(double reward);
		void saveAgent(const char* filename);
		void loadAgent(const char* filename);
		
		//debug
		void printSubpop();

};

#endif
