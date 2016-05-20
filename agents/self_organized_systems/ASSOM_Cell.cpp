
#include"ASSOM_Cell.h"

ASSOM_Cell::ASSOM_Cell(int size, int number_of_basis_vectors, void* pointer)
{
	int i,j;
	
	//initial values
	accumulated_error=0.0;
	this->size= size;
	this->number_of_basis_vectors= number_of_basis_vectors;
	frequency=0;
	this->pointer=pointer;
		
	//generate random vectors
	weight= (double**)malloc(sizeof(double*)*number_of_basis_vectors);
	for(i=0;i<number_of_basis_vectors;++i)
	{
		weight[i]= (double*)malloc(sizeof(double)*size);
		for(j=0;j<size;++j)
		{
			weight[i][j]= rand()/double(RAND_MAX);
		}
	}

	//orthonormalize weight vectors
	gramSchmidt();


}

ASSOM_Cell::~ASSOM_Cell()
{

}

//Modified Gram Schmidt Orthonormalization
void ASSOM_Cell::gramSchmidt()
{
	int i,j,k;

	for(i=0;i<number_of_basis_vectors;++i)
	{
	
	
		//normalization
		double module=0;
		for(j=0;j<size;++j)
		{
			module+= weight[i][j]*weight[i][j];
		}
		module= sqrt(module);

		for(j=0;j<size;++j)
		{
			weight[i][j]= weight[i][j]/module;
		}
		
	
		//projection and orthogonalization
		//the first vector is always fixed as it is
		for(k=i+1;k<number_of_basis_vectors;++k)
		{
			// <v_k,v_i>
			// ---------  * v_i
			// <v_i,v_i>

			// <v_k,v_i> -> projection of v_k in direction v_i 
			double vk_vi=0.0;
			for(j=0;j<size;++j)
			{
				vk_vi+= (weight[i][j]*weight[k][j]);
			}
			
			// <v_i,v_i>
			double vi_vi=0.0;
			for(j=0;j<size;++j)
			{
				vi_vi+= (weight[i][j]*weight[i][j]);
			}
			
			double scalar_result= vk_vi/vi_vi;
			
			//remove the component of v_k in the direction v_i
			for(j=0;j<size;++j)
			{
				weight[k][j]= weight[k][j] - scalar_result*weight[i][j];
			}
			
			
		}
	}
	
	
}

//map the input into this cell
void ASSOM_Cell::mapInput(double* input_array, double learning_coefficient)
{
	//accumulate error of all mapped input in this cell
	accumulated_error+= quadraticError(input_array);

	//increase frequency
	frequency++;

	//adapt cell's value
	int i,j;
	double xl= 0.0;

	//computing the module of the input's projection xl
	for(i=0;i<number_of_basis_vectors;++i)
	{
		double projection = 0.0; 
		for(j=0;j<size;++j)
		{
			projection+= weight[i][j]*input_array[j];
		}
		xl+= projection*projection;
	}
	xl= sqrt(xl);

	//calculating module of the input
	double input_module=0.0;
	for(j=0;j<size;++j)
	{
		input_module+= input_array[j]*input_array[j];
	}
	input_module= sqrt(input_module);
		
	double vector_invariant_coefficient= learning_coefficient/(xl*input_module); 
	
	//final computation
	for(i=0;i<number_of_basis_vectors;++i)
	{
		//input's projection on the basis vector
		double projection=0.0;
		for(j=0;j<size;++j)
		{
			projection+= (weight[i][j]*input_array[j]);
		}

		//calculate alpha from the article:
		//Fast-Learning Adaptive-Subspace Self-Organizing
		//Map: An Application to Saliency-Based Invariant
		//Image Feature Construction
		double alpha= vector_invariant_coefficient*projection;


		for(j=0;j<size;++j)
		{
			weight[i][j]+= alpha*(input_array[j]);

			if(fabs(weight[i][j]) < 0.001)
			{
				weight[i][j]= 0.0;
			}
		}
	}
	
	//orthonormalize weight vectors
	gramSchmidt();
	
}

//sum the weights of the cell passed as parameter in this ASSOM_Cell
void ASSOM_Cell::sumCellWeights(Cell* c2)
{
	ASSOM_Cell* c1= (ASSOM_Cell*)c2;

	int i,j;
	for(i=0;i<number_of_basis_vectors;++i)
	{
		for(j=0;j<size;++j)
		{
			weight[i][j]= c1->weight[i][j];
		}
	}
}

//scale weights of this cell by the value passed as parameter
//FOR ANY VALUE OTHER THAN 0.0 THE WEIGHTS WILL ONLY BE ORTHONORMALIZED 
//(cause there is no meaning in scaling vectors that should be later orthonormalized)
void ASSOM_Cell::scaleWeights(double factor)
{
	if(factor==0.0)
	{
		int i,j;
		for(i=0;i<number_of_basis_vectors;++i)
		{
			for(j=0;j<size;++j)
			{
				weight[i][j]= 0.0;
			}
		}
	}
	else
	{
		gramSchmidt();
	}
}

//output the error of the input array and the cell value
double ASSOM_Cell::quadraticError(double* input_array)
{
	//module of the orthogonal projection of the input on this neuron subspace
	double xl=0;

	int i,j;
	for(i=0;i<number_of_basis_vectors;++i)
	{
		double projection = 0.0; 
		for(j=0;j<size;++j)
		{
			projection+= weight[i][j]*input_array[j];
		}
		xl+= projection*projection;
	}

	//last_xl= sqrt(xl);

	return (1.0/(xl*(double)size));

}

void ASSOM_Cell::print()
{
	int i,j;
	
	printf("( ");
	for(i=0;i<number_of_basis_vectors;++i)
	{
	
		for(j=0;j<size;++j)
		{
			printf("%.3f ",weight[i][j]);
		}
		printf(", ");
	}
	printf(")");
}

void ASSOM_Cell::printToFile(FILE* fp)
{
	int i,j;
	
	for(i=0;i<number_of_basis_vectors;++i)
	{
	
		for(j=0;j<size;++j)
		{
			fprintf(fp,"%.3f ",weight[i][j]);
		}
		fprintf(fp,", ");
	}
}

void ASSOM_Cell::printError()
{
	printf("%.2f ",accumulated_error);
}
