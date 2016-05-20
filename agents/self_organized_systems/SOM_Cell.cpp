
#include"SOM_Cell.h"

SOM_Cell::SOM_Cell(int size, void* pointer)
{
	int i;
	
	weight= (double*)malloc(sizeof(double)*size);

	for(i=0;i<size;++i)
	{
		weight[i]= (rand()/double(RAND_MAX));
	}

	//initial values
	accumulated_error=0.0;
	this->size= size;
	frequency=0;
	this->pointer=pointer;

}

SOM_Cell::~SOM_Cell()
{

}

//map the input into this cell
void SOM_Cell::mapInput(double* input_array, double learning_coefficient)
{
	//accumulate error of all mapped input in this cell
	accumulated_error+= quadraticError(input_array);

	//increase frequency
	frequency++;

	//adapt cell's value
	int i;
	for(i=0;i<size;++i)
	{
		//printf("%f\n",input_array[i]);
		//printf("%f -  %f %f\n",learning_coefficient, weight[i],input_array[i]);
		weight[i]+= learning_coefficient*(input_array[i] - weight[i]);
		//printf("%f %f\n",weight[i],input_array[i]);
	}
	
}

//sum the weights of the cell passed as parameter in this SOM_Cell
void SOM_Cell::sumCellWeights(Cell* c2)
{
	SOM_Cell* c1 =(SOM_Cell*)c2;

	int i;
	for(i=0;i<size;++i)
	{
		weight[i]+= c1->weight[i];
	}
}

//scale weights of this cell by the value passed as parameter
void SOM_Cell::scaleWeights(double factor)
{
	int i;
	for(i=0;i<size;++i)
	{
		weight[i]= weight[i]*factor;
	}
}

//output the error of the input array and the cell value
double SOM_Cell::quadraticError(double* input_array)
{
	double error= 0.0; 

	int i;
	for(i=0;i<size;++i)
	{
		error+= (weight[i] - input_array[i])*(weight[i] - input_array[i]);
	}

	return (error);
}

void SOM_Cell::print()
{
	int i;
	
	printf("( ");
	for(i=0;i<size;++i)
	{
		printf("%.3f ",weight[i]);
	}
	printf(")");

}

void SOM_Cell::printToFile(FILE* fp)
{
	int i;
	
	for(i=0;i<size;++i)
	{
		fprintf(fp,"%.3f ",weight[i]);
	}

}

void SOM_Cell::printError()
{
	printf("%.2f ",accumulated_error);
}

