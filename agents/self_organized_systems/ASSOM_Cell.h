
#ifndef ASSOM_CELL
#define ASSOM_CELL

#include"stdlib.h"
#include"stdio.h"
#include"random/State_of_Art_Random.h"
#include"Cell.h"
#include"math.h"

class ASSOM_Cell : public Cell
{
		void gramSchmidt();

	public:
		ASSOM_Cell(int size, int number_of_basis_vectors, void* pointer=NULL);
		~ASSOM_Cell();

		double** weight; 
		int number_of_basis_vectors;
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
