
#include"Multiplexer.h"

Multiplexer::Multiplexer(int address_bits, int data_bits, Random* random)
{
	trial=-1;
	this->random= random;
	//this->verbose=verbose;

	this->address_bits= address_bits;
	this->data_bits= data_bits;

	//check for out of bound addresses
	int addressable_bits= (int) pow(2,address_bits);
	if(addressable_bits != data_bits)
	{
		//printf("ERROR: address bits can address more bits than available on the data_bits\n");
		printf("ERROR: incompatible address bits and data bits\n");
		exit(1);
	}

	//allocate all and set all combinations of inputs
	unsigned int iterator= 0;
	unsigned int one= 1;
	all_combinations= (int) pow(2,data_bits + address_bits);
	input_data= (char**)malloc(all_combinations*sizeof(char*));
	for(int i=0; i<all_combinations ;++i)
	{
		input_data[i]= (char*)calloc(data_bits+address_bits,sizeof(char));
		for(int j=0; j<(data_bits+address_bits) ;++j)
		{	
			unsigned int select_bit= one << j;
			char value= (iterator & select_bit) >> j;
			input_data[i][j]= value;
		}
			
		iterator++;		
	}

	//allocate a temporary address for use in the evaluation step
	//tmp_address=(char*)calloc(address_bits,sizeof(char));

	//max steps is just an arbitrary very big number
	//just to make sure the episode will not stop before it should
	MAX_STEPS= all_combinations*100;

	//printf("all combinations %d\n",all_combinations);
	
}

Multiplexer::~Multiplexer()
{
	//free(tmp_address);
}

void Multiplexer::start(int &number_of_observation_vars, int& number_of_action_vars)
{
	number_of_observation_vars= address_bits + data_bits;
	this->number_of_observation_vars= number_of_observation_vars;
	observation= (double*) malloc(number_of_observation_vars*sizeof(double));

	number_of_action_vars=1;
	this->number_of_action_vars=number_of_action_vars;

	// Initialize
	restart();
}

double Multiplexer::step(double* action)
{
	// initial reward
	if(action==NULL)
	{
		return 0;
	}

	//--------------------- Evaluate ------------------------

	double reward= 0.0;

	double correct_output= getCorrectOutput();

	//debug
	//printf("correct %f\n\n",correct_result);
	
	//discretize action (output)
	double discrete_output=0;
	if(action[0] > 0.5)
	{
		discrete_output=1;
	}

	//printf("%f (%f)",action[0],discrete_output);	
	
	//calculate squared error 
	//notice that mean squared error MSE= 1/n (\sum (y - correct_y)^2)
	double error= (discrete_output - correct_output);
	//since the output is binary the squared_error must be either 0 or 1
       	double squared_error= error*error; 	

	reward= -squared_error;

	//--------------------- Update Values and Check for End of Trial ------------------------
	
	//next step will be a different combination of inputs
	current_combination++;
	
	//check if all combinations were already tested
	if(current_combination >= all_combinations)
	{
		restart();
		return reward;
	}
	
	//set observation to current combination of inputs
	for(int j=0; j<(data_bits+address_bits) ;++j)
	{	
		observation[j]= (double) input_data[current_combination][j]; 
	}

	return reward;
}

bool Multiplexer::set(int feature)
{
	switch(feature)
	{
		case NORMALIZED_OBSERVATION:
		{
			//it is a binary problem {0,1} so the observation range is always in the normalized range [-1,1]
			normalized_observation=true;
			return true;
		}
		break;
		case NORMALIZED_ACTION:
		{
			//it is a binary problem {0,1} so the action range is always in the normalized range [0,1]
			//in fact, to avoid problems with the simmetry of the actions for sensitive agents, the threshold is set at 0.5, so 0.51 is 1 and 0.49 is 0.
			normalized_action=true;
			return true;
		}
		break;
		default:
		{
			return false;
		}
	}
}

//print all combinations of input data
void Multiplexer::print()
{
	for(int i=0; i<all_combinations ;++i)
	{
		for(int j=0; j<(data_bits+address_bits) ;++j)
		{	
			printf("%d ",input_data[i][j]);	
		}
		printf("\n");
	}
}

//for all arrays, swap its position with a random selected array
void Multiplexer::shuffle()
{
	for(int i=0; i< all_combinations ;++i)
	{
		int random_index= random->uniform(0,all_combinations-1);
		
		//swap values between random_index and this one
		for(int j=0; j<(data_bits+address_bits) ;++j)
		{	
			char tmp= input_data[i][j];
			input_data[i][j]= input_data[random_index][j];
			input_data[random_index][j]= tmp;
			
		}
	}

}

//correct output for current combination
double Multiplexer::getCorrectOutput()
{
	unsigned int address=0;
	for(int j=data_bits; j<data_bits + address_bits ;++j)
	{	
		//tmp_address[j-data_bits]= observation[j];
		char value= (char) observation[j];
		address|= value << (j-data_bits);
	}

	/*
	 * debug
	for(int j=0; j<data_bits;++j)
	{	
		printf("%f ",observation[j]);	
	}
	printf(" | ");
	for(int j=data_bits; j<data_bits + address_bits ;++j)
	{
		printf("%f ",observation[j]);	
	}
	printf("\n");

	printf("address %d\n",address);
	*/

	return observation[address];
}

double Multiplexer::restart()
{
	trial++;
	
	shuffle();

	//reset current combination counter
	current_combination=0;

	//set observation to current combination of inputs
	for(int j=0; j<(data_bits+address_bits) ;++j)
	{	
		observation[j]= (double) input_data[current_combination][j]; 
	}

	return 0;
}




