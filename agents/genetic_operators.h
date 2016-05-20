
#ifndef GENETIC_OPERATORS
#define GENETIC_OPERATORS

#include"random/Random.h"

//genetic operators
inline void incrementalMutation(double* chromossome, int chromossome_size, double mu, double increment, Random* random);
inline void incrementalGaussianMutation(double* chromossome, int chromossome_size, double mu, Random* random);
inline void differentialEvolutionOperator(double* trial_vector, double CR, double F, int individual, int dna_size, double** population, int population_size, Random* random);
inline void differentialEvolutionOperator(double* trial_vector, double CR, double F, double* parent_dna, double* r1_dna, double* r2_dna, double* r3_dna, int dna_size, Random* random);
inline void indirectDifferentialEvolutionOperator(double* trial_vector, double CR, double F, int individual, int dna_size, double** population, int* set, int set_size, Random* random);

//roulettes and tournaments
inline int tournament(double* fitness, int population_size, int tournament_size, Random* random);
inline int indirectTournament(int* index_set, int set_size, double* fitness, int tournament_size, Random* random);
inline int inverseTournament(double* fitness, int population_size, int tournament_size, Random* random);
inline int rouletteSelection(double* fitness, int size, Random* random);
inline int indirectRouletteSelection(int* index_set, int set_size, double* fitness, int size, Random* random);
inline int inverseRouletteSelection(double* fitness, int size, Random* random);

//updating rules
inline void Widrow_Hoff(double& value, double coefficient, double new_value);
inline void MAM(double& value, int& times_adjusted, double coefficient, double new_value);

//mutation with probability mu per allele
inline void incrementalMutation(double* chromossome, int chromossome_size, double mu, double increment, Random* random)
{
	int i;
	for(i=0;i<chromossome_size;++i)
	{
		if(random->uniform(0.0,1.0) < mu)
		{
			chromossome[i]+= random->uniform(-increment, increment);
		}
	}
}

//mutation with probability mu per allele and modification equal to the standard gaussian (standard deviation = 1, mean= 0)
inline void incrementalGaussianMutation(double* chromossome, int chromossome_size, double mu, Random* random)
{
	int i;
	for(i=0;i<chromossome_size;++i)
	{
		if(random->uniform(0.0,1.0) < mu)
		{
			chromossome[i]+= random->gaussian(0.0,1.0);
		}
	}
}

//return a index selected at random with the chances weighted proportionally to their fitness.
//the roulette will be done at the index_set, using the respective indexed fitness
//(i.e., individual i from index_set has fitness[index_set[i]])
inline int indirectRouletteSelection(int* index_set, int set_size, double* fitness, int population_size, Random* random)
{
	int i;

	double sum=0;

	for(i=0;i<set_size;++i)
	{
		sum+= fitness[index_set[i]];
	}

	double roulette= random->uniform(0.0, sum);

	if(sum > 0.0)
	{
		i=0;
		sum= fitness[index_set[i]];
		for(;roulette > sum;)
		{
			i++;
			sum+= fitness[index_set[i]];
		}
	}
	else
	{
		i=0;
		sum= fitness[index_set[i]];
		for(;roulette < sum;)
		{
			i++;
			sum+= fitness[index_set[i]];
		}
	}

	return i;
}

//return a index selected at random with the chances weighted proportionally to their fitness.
inline int rouletteSelection(double* fitness, int population_size, Random* random)
{
	int i;

	double sum=0;

	for(i=0;i<population_size;++i)
	{
		sum+= fitness[i];
	}

	double roulette= random->uniform(0.0, sum);

	i=0;
	sum= fitness[i];
	for(;roulette > sum;)
	{
		i++;
		sum+= fitness[i];
	}

	return i;
}

//use the index_set as the population for subsampling instead of the population
inline int indirectTournament(int* index_set, int set_size, double* fitness, int tournament_size, Random* random)
{
	int i;

	int champion= random->uniform(0,set_size-1);

	//choosing tournament members (repeated players are allowed, though not reasonable
	for(i=1;i<tournament_size;++i)
	{
		int member= random->uniform(0,set_size-1);

		if(fitness[index_set[member]] > fitness[index_set[champion]])
		{
			champion= member;	
		}
	}

	return champion;
}

//in the Inverse tournament the WEAKER wins :)
inline int inverseTournament(double* fitness, int population_size, int tournament_size, Random* random)
{
	int i;

	int champion= random->uniform(0,population_size-1);

	//choosing tournament members (repeated players are allowed, though not reasonable
	for(i=1;i<tournament_size;++i)
	{
		int member= random->uniform(0,population_size-1);

		if(fitness[member] < fitness[champion])
		{
			champion= member;	
		}
	}

	return champion;
}

inline int tournament(double* fitness, int population_size, int tournament_size, Random* random)
{
	int i;

	int champion= random->uniform(0,population_size-1);

	//choosing tournament members (repeated players are allowed, though not reasonable
	for(i=1;i<tournament_size;++i)
	{
		int member= random->uniform(0,population_size-1);

		if(fitness[member] > fitness[champion])
		{
			champion= member;	
		}
	}

	return champion;
}

//return a index selected at random with the chances weighted INVERSELLY proportional to their fitness.
inline int inverseRouletteSelection(double* fitness, int population_size, Random* random)
{
	int i;

	double sum=0;

	for(i=0;i<population_size;++i)
	{
		if(fitness[i]>0.01)
		{
			sum+= 1.0/fitness[i];
		}
		else
		{
			sum+= 100.0;
		}
	}

	double roulette= random->uniform(0.0, sum);

	i=0;
	sum= 1.0/fitness[i];
	for(;roulette > sum;)
	{
		i++;
		if(fitness[i]>0.01)
		{
			sum+= 1.0/fitness[i];
		}
		else
		{
			sum+= 100.0;
		}
	}

	return i;
}


//Moyenne Adaptive Modifiee (French for "average adaptive modifier")
//for coefficient in [0,1]
inline void MAM(double& value, int& times_adjusted, double coefficient, double new_value)
{

	//widrow-hoff rule
	if(times_adjusted > 1.0/coefficient)
	{	
		value= value + coefficient*(new_value - value);
	}
	//simple average
	else
	{
		value= (value+new_value)/2.0;
	}

	times_adjusted++;
}

//Widrow-Hoff rule
inline void Widrow_Hoff(double& value, double coefficient, double new_value)
{
		value= value + coefficient*(new_value - value);
}

inline void differentialEvolutionOperator(double* trial_vector, double CR, double F, int individual, int dna_size, double** population, int population_size, Random* random)
{
	int i;

	int r1, r2, r3; //index of other parents

	r1= random->uniform(0,population_size-1);
	r2= random->uniform(0,population_size-1);
	r3= random->uniform(0,population_size-1);

	for(i=0;i < dna_size;++i)
	{
		//F=random->uniform(0.0,1.0);
		trial_vector[i]= population[r1][i] + F*(population[r2][i] - population[r3][i]);
	
	}
	
	//choose a random index, to ensure that at least this will be inserted in the trial_vector
	int random_index= random->uniform(0, dna_size-1);
	
	for(i=0;i< dna_size;++i)
	{

		if(random_index != i && random->uniform() > CR)
		{
			trial_vector[i]= population[individual][i];
		}
		else
		{
			//if constraining a variable between a max_limit and min_limit 
			//is interesting, uncomment the following
			//if(trial_vector[i]>max_limit || trial_vector[i]<min_limit)
			//{
			//	trial_vector[i]= generateRandomVariable(i);
			//}
		}
	}
}

//The trial_vector IS the Offspring dna!!!!!!
inline void differentialEvolutionOperator(double* trial_vector, double CR, double F, double* parent_dna, double* r1_dna, double* r2_dna, double* r3_dna, int dna_size, Random* random)
{
	int i;

	for(i=0;i < dna_size;++i)
	{
		//F=random->uniform(0.0,1.0);
		trial_vector[i]= r1_dna[i] + F*(r2_dna[i] - r3_dna[i]);
	
	}
	
	//choose a random index, to ensure that at least this will be inserted in the trial_vector
	int random_index= random->uniform(0, dna_size-1);
	
	for(i=0;i< dna_size;++i)
	{

		if(random_index != i && random->uniform() > CR)
		{
			trial_vector[i]= parent_dna[i];
		}
		else
		{
			//if constraining a variable between a max_limit and min_limit 
			//is interesting, uncomment the following
			//if(trial_vector[i]>max_limit || trial_vector[i]<min_limit)
			//{
			//	trial_vector[i]= generateRandomVariable(i);
			//}
		}
	}
}

inline void indirectDifferentialEvolutionOperator(double* trial_vector, double CR, double F, int individual, int dna_size, double** population, int* set, int set_size, Random* random)
{
	int i;

	int r1, r2, r3; //index of other parents

	r1= random->uniform(0,set_size -1);
	r2= random->uniform(0,set_size -1);
	r3= random->uniform(0,set_size -1);

	r1= set[r1];
	r2= set[r2];
	r3= set[r3];

	for(i=0;i < dna_size;++i)
	{
		//F=random->uniform(0.0,1.0);
		trial_vector[i]= population[r1][i] + F*(population[r2][i] - population[r3][i]);
	
	}
	
	//choose a random index, to ensure that at least this will be inserted in the trial_vector
	int random_index= random->uniform(0, dna_size-1);
	
	for(i=0;i< dna_size;++i)
	{

		if(random_index != i && random->uniform() > CR)
		{
			trial_vector[i]= population[individual][i];
		}
		else
		{
			//if constraining a variable between a max_limit and min_limit 
			//is interesting, uncomment the following
			//if(trial_vector[i]>max_limit || trial_vector[i]<min_limit)
			//{
			//	trial_vector[i]= generateRandomVariable(i);
			//}
		}
	}
}

#endif
