
#include"Som_Map.h"

Som_Map::Som_Map(int width, int height, int size, int number_of_basis_vectors)
{
	int i,j;
	
	this->width= width;
	this->height= height;
	this->size= size;
	this->number_of_basis_vectors= number_of_basis_vectors;
	max_width= 5*width;
	max_height= 5*height;
	map_error=0.0;
	growing_counter=0;

	//map= new Cell**[max_width];
	map= (Cell***)malloc(sizeof(Cell**)*max_width);
	for(i=0;i<max_width;++i)
	{
		//map[i]= new Cell*[max_height];
		map[i]= (Cell**)malloc(sizeof(Cell*)*max_height);
	}

	//initial value, will be smaller than any distance. Therefore, it will be modified soon enough.
	r=-1;
	
	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
		
			// if it is an ASSOM
			if(number_of_basis_vectors>0)
			{
				map[i][j]= new ASSOM_Cell(size, number_of_basis_vectors);
			}
			else // or if it is an SOM
			{
				map[i][j]= new SOM_Cell(size);
			}
		}
	}
	
	
	///////////////	   parameters	///////
	
	//growing parameters
	growing_turn=width*height;
	//error_threshold= 0.05;
	error_threshold= 2.5;
	grow_enabled=false;
	
	//usual parameters
	threshold= 0.005;	//threshold above which cells will not be updated anymore	
	
	//SOM_TYPE == SOM 	(not parameter_less)
	learning_rate= 0.1;	
	mature_rate= 1.0001;	//the mature speed, how fast it will make the network stable as it gets input
	neighborhood_parameter= 1.0;	
	//mature_rate= 1.0;
	
	//SOM_TYPE == PARAMETER_LESS 	("parameter less" SOM)
	//neighborhood_max= (width+height)/4;
	//neighborhood_max= width*height;
	neighborhood_max= (width + height)/2;
	//neighborhood_max= width*width*height*height;
	neighborhood_min= 0.01;

	//SOM_TYPE == DSOM
	if(SOM_TYPE == DSOM)
	{
		learning_rate= 0.1;
		plasticity= 1.25;
	}
	
}

Som_Map::~Som_Map()
{

}

void Som_Map::input(double* input_array)
{
	if(grow_enabled == true)
	{
		growing_counter++;
		if(growing_counter > growing_turn)
		{
			grow();
			growing_counter=0;
			//map_error=0.0;
			growing_turn=width*height;
		}
	}


	//NEURAL COMPETITION
	//find the smallest neuron's error from the map
	
	//indexes and error of the fittest neuron
	int i_index=0;
	int j_index=0;	

	//return the position of the neuron which won the competition
	neuronCompetition(input_array, i_index, j_index);

	
	//UPDATE NEURONS IN THE NEIGHBORHOOD

	updateNeighborhood(input_array, i_index, j_index);

}
	
void Som_Map::updateNeighborhood(double* input_array, int i_index, int j_index)
{
	int i,j;
	
	double quadratic_min_error= map[i_index][j_index]->quadraticError(input_array);

	map_error+= quadratic_min_error;

//	printf("min error %f\n",min_error);
//	printf("i %d j %d \n",i_index, j_index);
//	map[i_index][j_index]->print();
//	printf("\n");

	if(SOM_TYPE == PARAMETER_LESS)
	{
		if(quadratic_min_error > r)
		{
			r = quadratic_min_error;
		}

		//printf("%f\n",min_error);
		double epsilon= quadratic_min_error/r;
		neighborhood_parameter= neighborhood_max*epsilon;

		if(neighborhood_parameter < neighborhood_min)
		{
			neighborhood_parameter= neighborhood_min;
		}

		learning_rate= epsilon;
	}

	//UPDATING NEURONS
	//close neurons are updated
	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
			if(SOM_TYPE==SOM||SOM_TYPE==PARAMETER_LESS)
			{
				double neuron_distance= abs(i-i_index) + abs(j - j_index);
				//double neuron_distance= sqrt((i-i_index)*(i-i_index) + (j - j_index)*(j-j_index));

				//neighborhood_coefficient= exp(-neuron_distance);
				double neighborhood_coefficient= exp(-(neuron_distance*neuron_distance)/(neighborhood_parameter*neighborhood_parameter));

				double combined_coefficient= neighborhood_coefficient*learning_rate;

				//printf("coef %f\n",combined_coefficient);

				//only update if the learning coefficient is above the threshold
				if(combined_coefficient > threshold)
				{
					map[i][j]->mapInput(input_array, combined_coefficient);	
				}
			}
			
			if(SOM_TYPE==DSOM)
			{
				//double neuron_distance= abs(i-i_index) + abs(j - j_index);
				double quadratic_neuron_to_neuron_distance= ((i-i_index)*(i-i_index) + (j - j_index)*(j-j_index));
				
				double neighborhood_coefficient= exp(-(quadratic_neuron_to_neuron_distance)/(quadratic_min_error*plasticity*plasticity));
				
				double neuron_to_input_error= sqrt(map[i][j]->quadraticError(input_array));

				double combined_coefficient= neighborhood_coefficient*learning_rate*neuron_to_input_error;

				map[i][j]->mapInput(input_array, combined_coefficient);	
			}
		}
	}

	if(SOM_TYPE == SOM)
	{
		//mature 
		learning_rate= learning_rate*(1 -(mature_rate -1));
		//neighborhood_parameter= neighborhood_parameter*mature_rate;
	}
	
}
	
//return the position of the neuron which won the competition (closest neuron) for the input_array
void Som_Map::neuronCompetition(double* input_array, int& i_index, int& j_index)
{

	int i,j;
	//indexes and error of the fittest neuron
	i_index=0;
	j_index=0;	
	double min_error= map[0][0]->quadraticError(input_array);

	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
			double error= map[i][j]->quadraticError(input_array);
			
			//if we found a better neuron to fit
			if(error < min_error)
			{
				i_index=i;
				j_index=j;
				min_error=error;
			}
		}
	}

		
}

void Som_Map::grow()
{
	int i,j;

	printf("Map Error %f\n",map_error);
	
	//indexes and error of the neuron with highest acummulated error
	int i_index=0;
	int j_index=0;	
	double max_error= (map[0][0])->accumulated_error;
	double sum=0;
	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
			double error= (map[i][j])->accumulated_error;

			sum+= error;
			
			//if we found a higher accumulated error
			if(error > max_error)
			{
				i_index=i;
				j_index=j;
				max_error=error;
			}
		}
	}

	sum= sum/(width*height);

	//printf("max_error %f map error %f\n",max_error, map_error);
	printf("max_error %f sum %f\n",max_error, sum);


	//do not grow if the error inside the map is not enough
	if(map_error/(width*height) < error_threshold)
	//if(max_error/sum < error_threshold)
	{
		return;
	}
	else
	{
		//map_error= 0;
		//map[i_index][j_index]->accumulated_error= 0;
	}



//	printf("highest error: %d %d\n", i_index, j_index);
//	printf("width: %d %d\n", width, height);

	//find the neighbor with highest error
	double neighbor_error= -1;
	int df_i;
	int df_j;
	bool row=false;
	bool column=false;
	
	if(((j_index+1)<height) && (neighbor_error< map[i_index][j_index+1]->accumulated_error))
	{
		neighbor_error= map[i_index][j_index+1]->accumulated_error;
		df_i=0;
		df_j=1;
		row=false;
		column=true;
	}

	if(((i_index+1)< width) && (neighbor_error< map[i_index+1][j_index]->accumulated_error))
	{
		neighbor_error= map[i_index+1][j_index]->accumulated_error;
		df_i=1;
		df_j=0;
		row=true;
		column=false;

	}
	if(((i_index -1) >=0) && (neighbor_error< map[i_index-1][j_index]->accumulated_error))
	{
		neighbor_error= map[i_index-1][j_index]->accumulated_error;
		df_i=0;//-1;
		df_j=0;
		row=true;
		column=false;
	}
	if(((j_index -1 >=0 ))&& (neighbor_error< map[i_index][j_index-1]->accumulated_error))
	{
		neighbor_error= map[i_index][j_index-1]->accumulated_error;
		df_i=0;
		df_j=0;//-1;
		row=false;
		column=true;
	}
	
//	printf("neighbor's highest error: %d %d\n", df_i, df_j);
		
	//insert row
	//if(df_i!=0)
	if(row)
	{
		insertRow(i_index, j_index,df_i);
	}

	//insert column
	//if(df_j!=0)
	if(column)
	{
		insertColumn(i_index, j_index, df_j);
	}
	
}


void Som_Map::insertRow(int i_index, int j_index, int df_i)
{
	int i,j;

	//check if reallocation is necessary
	if(width + 1 >= max_width)
	{
		reallocateWidth();
		printf("reallocate width\n\n");
	}

	//increase the width
	width++;

	for(i=width;i>i_index+df_i;--i)
	{
		for(j=0;j<height;++j)
		{
			map[i][j]= map[i-1][j];
			map[i-1][j]=NULL;
		}
	}
	
	for(j=0;j<height;++j)
	{
		//if it is an ASSOM
		if(number_of_basis_vectors>0)
		{
			map[i_index+df_i][j]= new ASSOM_Cell(size, number_of_basis_vectors);
		}
		else // or if it is an SOM
		{
			map[i_index+df_i][j]= new SOM_Cell(size);
		}
		
		//set the weights of the new Cell to the average of its neighbors
		
		//set all weights to zero
		map[i_index+df_i][j]->scaleWeights(0.0);
		int counter=0;

		if(i_index+df_i+1<width)
		{
			map[i_index+df_i][j]->sumCellWeights(map[i_index+df_i+1][j]);
			counter++;
		}
		
/*		if(j+1<height)
		{
			map[i_index+df_i][j]->sumCellWeights(map[i_index+df_i][j+1]);
			counter++;
		}
*/		
		if(i_index+df_i -1 >= 0)
		{
			map[i_index+df_i][j]->sumCellWeights(map[i_index+df_i-1][j]);
			counter++;
		}

/*		if(j-1 > 0)
		{
			map[i_index+df_i][j]->sumCellWeights(map[i_index+df_i][j-1]);
			counter++;
		}
*/
		map[i_index+df_i][j]->scaleWeights(1.0/((double)counter));

	}
	
	
}

void Som_Map::insertColumn(int i_index, int j_index, int df_j)
{
	int i,j;

	//check if reallocation is necessary
	if(height + 1 >= max_height)
	{
		reallocateHeight();
	}

	//increase the width
	height++;

	for(j=height;j>j_index+df_j;--j)
	{
		for(i=0;i<width;++i)
		{
			map[i][j]= map[i][j-1];
			map[i][j-1]=NULL;
		}
	}
	
	for(i=0;i<width;++i)
	{
		//if it is an ASSOM
		if(number_of_basis_vectors>0)
		{
			map[i][j_index+df_j]= new ASSOM_Cell(size, number_of_basis_vectors);
		}
		else // or if it is an SOM
		{
			map[i][j_index+df_j]= new SOM_Cell(size);
		}
		
		//set the weights of the new Cell to the average of its neighbors
		
		//set all weights to zero
		map[i][j_index+df_j]->scaleWeights(0.0);
		int counter=0;

		if(j_index+df_j+1<height)
		{
			map[i][j_index+df_j]->sumCellWeights(map[i][j_index+df_j+1]);
			counter++;
		}
		
/*		if(j+1<height)
		{
			map[i_index+df_i][j]->sumCellWeights(map[i_index+df_i][j+1]);
			counter++;
		}
*/		
		if(j_index+df_j -1 >= 0)
		{
			map[i][j_index+df_j]->sumCellWeights(map[i][j_index+df_j-1]);
			counter++;
		}

/*		if(j-1 > 0)
		{
			map[i_index+df_i][j]->sumCellWeights(map[i_index+df_i][j-1]);
			counter++;
		}
*/
		map[i][j_index+df_j]->scaleWeights(1.0/((double)counter));
	}
	
}

void Som_Map::reallocateHeight()
{
	int i;
	max_height= 5*max_height;
	
	for(i=0;i<max_width;++i)
	{
		map[i]= (Cell**)realloc(map[i], sizeof(Cell*)*max_height);
	}
}


void Som_Map::reallocateWidth()
{
	//printf("reallocate width...\n");
	int i;
	int past_width=max_width;
	max_width= 5*max_width;
	
	map= (Cell***)realloc(map,sizeof(Cell**)*max_width);
	for(i=past_width;i<max_width;++i)
	{
		map[i]= (Cell**)malloc(sizeof(Cell*)*max_height);
	}
}

void Som_Map::print()
{
	int i,j;

	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
			map[i][j]->print();
		}
		printf("\n");
	}
}

void Som_Map::printToFile(const char* filename)
{
	int i,j;

	FILE* fp = fopen(filename,"w");

	//fprintf(fp,"width height %d %d\n",width,height);
	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
			map[i][j]->printToFile(fp);
			fprintf(fp,"\n");
		}
	}
	fclose(fp);
}

void Som_Map::printError()
{
	int i,j;

	for(i=0;i<width;++i)
	{
		for(j=0;j<height;++j)
		{
			map[i][j]->printError();
		}
		printf("\n");
	}
}
