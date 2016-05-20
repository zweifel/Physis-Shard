
#ifndef CELL
#define CELL

#include"stdio.h"

class Cell
{
	public:

		Cell(){};
		virtual ~Cell(){};
		
		double accumulated_error;	//will be used to expand the map around the 	
		int frequency;
		void* pointer;	//used to store something

		virtual void mapInput(double* input_array, double learning_coefficient)=0;
		virtual double quadraticError(double* array)=0;
		virtual void print()=0;
		virtual void printToFile(FILE* fp)=0;
		virtual void printError()=0;

		virtual void scaleWeights(double factor)=0;
		virtual void sumCellWeights(Cell* c2)=0;
};

#endif

