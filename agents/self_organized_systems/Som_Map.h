
#ifndef SOM_MAP
#define SOM_MAP

#include"Cell.h"
#include"SOM_Cell.h"
#include"ASSOM_Cell.h"
#include"math.h"
#include"../../parameters.h"

class Som_Map
{
	public:
		Som_Map(int width, int height, int size, int number_of_basis_vectors=0);
		~Som_Map();
	
		Cell*** map;
		int width;
		int height;
		int size;
		int number_of_basis_vectors;  			//zero for SOM and nonzero for ASSOM
		//the real allocated height and width
		int max_width;
		int max_height;
		bool grow_enabled;
		double mature_rate;
		
		double learning_rate;
		double neighborhood_parameter;
		double threshold;
		double map_error;
		double error_threshold;
		int growing_turn;
		int growing_counter;

		//PARAMETER_LESS
		double neighborhood_max;
		double neighborhood_min;
		double r;

		//DSOM
		double plasticity;

		void print();
		void printToFile(const char* filename);
		void printError();

		//API
		void input(double* input_array);
		void neuronCompetition(double* input_array, int& i_index, int& j_index);
		void updateNeighborhood(double* input_array, int i_index, int j_index);
		
		//growing
		void insertRow(int i_index,int j_index,int df_i);
		void insertColumn(int i_index,int j_index,int df_j);
		void reallocateWidth();
		void reallocateHeight();
		void grow();

};

#endif
