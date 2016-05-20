
#ifndef SOM_CELL
#define SOM_CELL

#include"stdlib.h"
#include"stdio.h"
#include"random/State_of_Art_Random.h"
#include"Cell.h"

class SOM_Cell : public Cell
{
	public:
		SOM_Cell(int size, void* pointer=NULL);
		~SOM_Cell();

		double* weight; 
		int size;
		//double accumulated_error;	//will be used to expand the map around the 	
		//int frequency;

		void mapInput(double* input_array, double learning_coefficient);
		double quadraticError(double* array);
		void print();
		void printToFile(FILE* fp);
		void printError();

		void scaleWeights(double factor);
		void sumCellWeights(Cell* c2);
};

#endif
