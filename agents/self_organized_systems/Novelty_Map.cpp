
#include"Novelty_Map.h"

Novelty_Map::Novelty_Map(int population_size, int number_of_inputs)
{
	this->population_size= population_size;
	this->number_of_inputs= number_of_inputs;
	
	map= (cell*) malloc(sizeof(cell)*population_size);
	int i;
	for(i=0;i<population_size;++i)
	{
		map[i].weight= (double*) calloc(number_of_inputs, sizeof(double));
	}

	current_population_size=0;

#ifdef	PRINT_NOVELTY_MAP_UPDATES
	individual_difference= 0.0;
	update_counter=0;
#endif
}

Novelty_Map::~Novelty_Map()
{
	
	//freeing memory
	int i;
	for(i=0;i<population_size;++i)
	{
		free(map[i].weight);
	}
	free(map);

}

	
void Novelty_Map::print()
{
	int i,j;

	for(i=0; i<current_population_size; ++i)
	{
		for(j=0;j<number_of_inputs;++j)
		{
			printf("%f ",map[i].weight[j]);
		}
		printf("\n");
	}
}

void Novelty_Map::printToFile(const char* filename)
{
	int i,j;

	FILE* fp = fopen(filename,"w");

	for(i=0; i<current_population_size; ++i)
	{
		for(j=0;j<number_of_inputs;++j)
		{
			fprintf(fp,"%f ",map[i].weight[j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

}

//return the index of the closest cell to the input_array
int Novelty_Map::input(double* input_array)
{
	int inserted_index;

	//check if the population is not full
	if(current_population_size < population_size)
	{
		inserted_index= current_population_size;
		insertIndividual(inserted_index, input_array);
		map[inserted_index].pointer=NULL;

		current_population_size++;
		if(current_population_size==population_size)
		{
			updateMinDistance();
		}

		return inserted_index;
	}
	//if the population is full, check for the uniqueness of the input
	//in the case the input is "unique enough", insert it
	else
	{
		int closest_individual;
		double current_input_min_distance= minDistance(input_array,closest_individual);
		if(min_distance < current_input_min_distance)
		{
			insertIndividual(worst_individual, input_array);
			updateMinDistance();
			//printf("removing worst %d \n",worst_individual);
#ifdef	PRINT_NOVELTY_MAP_UPDATES
			individual_difference+= diffIndividual(worst_individual, input_array);
			update_counter++;
#endif
			return worst_individual;
		}
		else
		{
			return closest_individual;
		}
	}
		
	printf("ERROR: should have returned previously\n");
	return -1;
}

//return the index of the closest cell to the input_array
//IMPORTANT: inputNeutral() does not modify anything inside the Novelty map, i.e., does not add or update it
int Novelty_Map::inputNeutral(double* input_array)
{
	int closest_individual;
	minDistance(input_array,closest_individual);
	
	return closest_individual;
}

void Novelty_Map::insertIndividual(int individual_index, double* weight)
{
	int i;
	for(i=0;i<number_of_inputs;++i)
	{
		map[individual_index].weight[i]= weight[i];
	}
	map[individual_index].frequency=0;
}

//measure the difference between the individual and an input_array called weight
double Novelty_Map::diffIndividual(int individual_index, double* weight)
{
	double difference=0.0;
	
	int i;
	for(i=0;i<number_of_inputs;++i)
	{
		difference+= fabs(map[individual_index].weight[i] - weight[i]);
	}
	difference/= (double)number_of_inputs;
	
	return difference;
}

//update the values of both:
//	-min_distance	- minimal distance of the worst individual to another individual in the population
//	-worst_individual - individual with smaller min_distance
void Novelty_Map::updateMinDistance()
{
	int i;

	worst_individual=0;
	min_distance=minDistance(worst_individual);
	for(i=1;i<current_population_size;++i)
	{
		if(min_distance > minDistance(i))	
		{
			min_distance= minDistance(i);
			worst_individual=i;
		}
	}
}

//calculate the minimal distance to the individual
double Novelty_Map::minDistance(int individual)
{
	int i;

	double dist;
	if(individual!=0)
	{
		dist= quadraticDist(individual,0);
	}
	else
	{
		dist= quadraticDist(individual,1);
	}

	for(i=1;i<current_population_size;++i)
	{
		if( i!=individual && dist > quadraticDist(individual, i) )
		{
			dist= quadraticDist(individual,i);
		}
	}

	return dist;
	
}

//calculate the minimal distance to the input
double Novelty_Map::minDistance(double* input_array, int& closest_individual)
{
	int i;

	double dist;
	dist= quadraticDist(0,input_array);
	closest_individual=0;

	for(i=1;i<current_population_size;++i)
	{
		if(dist > quadraticDist(i, input_array) )
		{
			dist= quadraticDist(i,input_array);
			closest_individual= i;
		}
	}

	return dist;
	
}

double Novelty_Map::quadraticDist(int individual_a, double* input_array)
{
	int i;

	double dist=0.0;
	for(i=0;i<number_of_inputs;++i)
	{
		dist+= (map[individual_a].weight[i] - input_array[i])*
			(map[individual_a].weight[i] - input_array[i]);
	}

	return dist;
}

double Novelty_Map::quadraticDist(int individual_a, int individual_b)
{
	int i;

	double dist=0.0;
	for(i=0;i<number_of_inputs;++i)
	{
		dist+= (map[individual_a].weight[i] - map[individual_b].weight[i])*
			(map[individual_a].weight[i] - map[individual_b].weight[i]);
	}

	return dist;
}

