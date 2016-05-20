
#ifndef STRUCTURAL_DNA
#define STRUCTURAL_DNA

#include"stdio.h"
#include"random/State_of_Art_Random.h"

//all neurons not activated by a control neuron are inactive by default
#define DEFAULT_INACTIVE

//structure and weight cycles are inexistent, all the evolution happens on only a structure cycle and imprecise weights
#define NO_WEIGHT_STRUCTURE_CYCLE

#define REMAINING_NEURON_THRESHOLD 0.001

//neuron types
//
//Important:
//Input and Output neurons are special neurons, which can not be randomly generated and therefore
//are after the NUMBER_OF_NEURON_TYPES definition
//enum{IDENTITY, SIGMOID, THRESHOLD, RANDOM, CONTROL, SIN, NUMBER_OF_NEURON_TYPES, INPUT_IDENTITY, INPUT_SIGMOID, OUTPUT_IDENTITY, OUTPUT_SIGMOID};
enum{IDENTITY, SIGMOID, THRESHOLD, RANDOM, CONTROL, NUMBER_OF_NEURON_TYPES, INPUT_IDENTITY, INPUT_SIGMOID, OUTPUT_IDENTITY, OUTPUT_SIGMOID, SIN};


//starting levels of firing rate 
enum{LEVEL1, LEVEL7, LEVEL49, NUMBER_OF_FIRING_RATE_LEVELS};

typedef struct _neuron
{
	int id;
	int firing_rate;
	int type;
	int interface_index;	//only used by input and output neurons to find the respective variable inside the input/output vector
}neuron;

typedef struct _connection
{
	int from_neuron_id;
	int to_neuron_id;
	float weight;
	int neuro_modulation;	//-1 for inactive, for >0 it is active and represents the id of the neuron whose response is used as weight 
}connection;

inline void writeDNA(const char* filename, neuron** n, int n_size, connection** c, int c_size)
{
	FILE* fp= fopen(filename, "w");

	if(fp==NULL)
	{
		printf("Could no open file to write\n");
		exit(1);
	}

	fwrite((const void *)&n_size, sizeof(n_size), 1, fp);
	fwrite((const void *)&c_size, sizeof(c_size), 1, fp);
	fwrite((const void *)*n, sizeof(neuron), n_size+1, fp);
	fwrite((const void *)*c, sizeof(connection), c_size+1, fp);
	//fwrite((unsigned char *)n, sizeof(neuron), n_size, fp);
	//fwrite((unsigned char *)c, sizeof(connection), c_size, fp);
	
	//printf("saving n_size %d %d\n",n_size,sizeof(neuron));
	//printf("saving c_size %d %d\n",c_size,sizeof(connection));
	
	fclose(fp);

}

inline void readDNA(const char* filename, neuron** n, int& n_size, connection** c, int& c_size)
{
	FILE* fp= fopen(filename, "r");
	
	if(fp==NULL)
	{
		printf("Could no open file to read\n");
		exit(1);
	}

	int size;

	fread((void *)&size, sizeof(size), 1, fp);
	n_size= size;
	fread((void *)&size, sizeof(size), 1, fp);
	c_size= size;
	fread((void *)*n, sizeof(neuron), n_size+1, fp);
	fread((void *)*c, sizeof(connection), c_size+1, fp);

	//printf("n_size %d\n",n_size);
	//printf("c_size %d\n",c_size);

	fclose(fp);
}

inline int randomFiringRateLevel(Random* random)
{
	int level= random->uniform(0,NUMBER_OF_FIRING_RATE_LEVELS-1);

	switch(level)
	{
		case LEVEL1:
		{
			return 1;
		}
		break;
		case LEVEL7:
		{
			return 7;
		}
		break;
		case LEVEL49:
		{
			return 49;
		}
		break;
	}

	printf("ERROR: random firing rate is generated with incorrect value of %d\n",level);
	exit(1);
	return 0;
}

inline void fprintNeuronType(FILE* fp, int neuron_type)
{
	switch(neuron_type)
	{
		case IDENTITY:
		{
			fprintf(fp,"Identity");
			return;
		}
		break;

		case SIGMOID:
		{
			fprintf(fp,"Sigmoid");
			return;
		}
		break;
		
		case THRESHOLD:
		{
			fprintf(fp,"Threshold");
			return;
		}
		break;
		
		case RANDOM:
		{
			fprintf(fp,"Random");
			return;
		}
		break;
		
		case SIN:
		{
			fprintf(fp,"Sin");
			return;
		}
		break;
		
		case CONTROL:
		{
			fprintf(fp,"Control");
			return;
		}
		break;
		
		case INPUT_IDENTITY:
		{
			fprintf(fp,"Input Identity");
			return;
		}
		break;
		
		case INPUT_SIGMOID:
		{
			fprintf(fp,"Input Sigmoid");
			return;
		}
		break;
		
		case OUTPUT_IDENTITY:
		{
			fprintf(fp,"Output Identity");
			return;
		}
		break;
		
		case OUTPUT_SIGMOID:
		{
			fprintf(fp,"Output Sigmoid");
			return;
		}
		break;
	}
	
	printf("ERROR: fprintfNeuronType() function with an incorrect value of neuron type %d\n",neuron_type);
	exit(1);
	return;
}

inline void printNeuronType(int neuron_type)
{
	switch(neuron_type)
	{
		case IDENTITY:
		{
			printf("Identity");
			return;
		}
		break;

		case SIGMOID:
		{
			printf("Sigmoid");
			return;
		}
		break;
		
		case THRESHOLD:
		{
			printf("Threshold");
			return;
		}
		break;
		
		case RANDOM:
		{
			printf("Random");
			return;
		}
		break;
		
		case CONTROL:
		{
			printf("Control");
			return;
		}
		break;
		
		case SIN:
		{
			printf("Sin");
			return;
		}
		break;
		
		case INPUT_IDENTITY:
		{
			printf("Input Identity");
			return;
		}
		break;
		
		case INPUT_SIGMOID:
		{
			printf("Input Sigmoid");
			return;
		}
		break;
		
		case OUTPUT_IDENTITY:
		{
			printf("Output Identity");
			return;
		}
		break;
		
		case OUTPUT_SIGMOID:
		{
			printf("Output Sigmoid");
			return;
		}
		break;
	}
	
	printf("ERROR: printNeuronType() function with an incorrect value of neuron type %d\n",neuron_type);
	exit(1);
	return;
}

inline float activationFunction(int neuron_type, float weighted_input, Random* random)
{
	switch(neuron_type)
	{
		
		case RANDOM:
		{
			double result= random->uniform(-1.0,1.0);

			return (float)result;
		}
		break;
		
		case THRESHOLD:
		{
		}
		case CONTROL:
		{
			if(weighted_input >= 0)
			{
				return 1.0;
			}
			else
			{
				return -1.0;
			}
		}
		break;
		
		case SIN:
		{
			return sin(weighted_input);
		}
		break;
		
		case IDENTITY:
		{
		}
		case INPUT_IDENTITY:
		{
		}
		case OUTPUT_IDENTITY:
		{
			return weighted_input;
		}
		break;
	
		case SIGMOID:
		{
		}
		case INPUT_SIGMOID:
		{
		}
		case OUTPUT_SIGMOID:
		{
			return tanhf(weighted_input);
		}
		break;
	}
	
	printf("ERROR: activationFunction() with an incorrect value of neuron type %d\n",neuron_type);
	exit(1);
	return 0;
}

#endif
