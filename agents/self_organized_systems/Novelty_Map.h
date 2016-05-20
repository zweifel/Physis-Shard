
#ifndef NOVELTY_MAP
#define NOVELTY_MAP

#include"stdlib.h"
#include"stdio.h"
#include"math.h"
#include"../../parameters.h"

typedef struct _cell
{
	double* weight; 

	int frequency;
	void* pointer;	//used to store something
}cell;

class Novelty_Map
{
	public:
		Novelty_Map(int population_size, int number_of_inputs);
		~Novelty_Map();
	
		cell* map;
		int population_size;
		int number_of_inputs;

		int current_population_size;
		double min_distance;
		int worst_individual;

#ifdef	PRINT_NOVELTY_MAP_UPDATES
		double individual_difference;
		int update_counter;
#endif

		//auxiliary functions
		void insertIndividual(int individual_index, double* weight);
		double diffIndividual(int individual_index, double* weight);
		void updateMinDistance();
		double minDistance(int individual);
		double minDistance(double* input_array);
		double minDistance(double* input_array, int& closest_individual);
		double quadraticDist(int individual_a, double* input_array);
		double quadraticDist(int individual_a, int individual_b);
		
		//not implemented yet
		//bool grow_enabled;
		
		void print();
		void printToFile(const char* filename);
		//void printError();

		//API
		int input(double* input_array);
		int inputNeutral(double* input_array);
		//void neuronCompetition(double* input_array, int& i_index, int& j_index);
		//void updateNeighborhood(double* input_array, int i_index, int j_index);

};

#endif
