
#ifndef PARAMETERS
#define PARAMETERS

enum{TRIVIAL_MODEL, NEURAL_MODEL};

//General settings
#define FILENAME_SIZE 128

//  ----------  Self Organizing Classifiers  ---------- //

//Model
#define INDIVIDUAL_MODEL NEURAL_MODEL
//#define INDIVIDUAL_MODEL TRIVIAL_MODEL
#define ENSEMBLE_MODE false
//#define RANDOM_INPUT 

//Team of Classifiers
//#define	HIERARCHICAL_HOF

//Reinforcement Learning
#define GAMMA_DISCOUNT_FACTOR 0.99
#define WIDROW_COEFFICIENT 0.1
#define STARTING_BEST_FITNESS 0
#define STARTING_FITNESS -1
//#define DETERMINISTIC_EXPERIENCE //PROBABILISTIC_EXPERIENCE
#define PROBABILISTIC_EXPERIENCE


//Evolution
#define CR_PARAMETER 0.2
#define RANDOM_F_PARAMETER
#define F_PARAMETER 2.0
#define DNA_LIMITS false 	//limit the DNA values [-1,1]
#define ORIGINAL_DE_OPERATOR false

#define BEST_SOLUTIONS_SIZE 10
#define NOVEL_SOLUTIONS_SIZE 10
#define MAX_NEIGHBOR_DISTANCE 4
#define DE_NEIGHBORHOOD 2	//distance from the cell which is still considered neighborhood for the DE
#define EXPERIENCE_PER_INDIVIDUAL 10
#define PROBABILITY_OF_USING_DE 0.5
#define USE_SURROGATE false
#define EXPLORATION_CYCLES_CHANGE_WITH_SHOCK false //true - exploration and exploitation cycles change after negative reward; false - exploration and exploitation cycles change each step



//Multi Objective Evolution
#define NUMBER_OF_POPULATIONS 2

//Neural Model
#define NUMBER_OF_HIDDEN_NODES 10
#define NUMBER_OF_HIDDEN_LAYERS 1

//SOM
#define NOVELTY_POP 40
enum{SOM,PARAMETER_LESS, DSOM};
#define SOM_TYPE PARAMETER_LESS
#define SOM_SIZE 5
#define TOGGLE_PARAMETER_LESS true


// ------------ NEAT --------------//

//additional fitness added to avoid negative fitness values
//for some reason it seems that NEAT does not work with negative fitness values
#define NEAT_ADDITIONAL_FITNESS_HACK 2000

#define SHOW_NEAT_PARAMETERS false
#define USE_INITIAL_BIAS
#define NEAT_INITIAL_BIAS 1.0
#define NEAT_PARAMETERS "agents/neat/pole2_markov.ne"
//#define NEAT_PARAMETERS "agents/neat/parameters.ne"

// -----------  Unified Neural Model ------------- //

#define EXTREME_NEGATIVE_REWARD -100000000
#define NUMBER_OF_INITIAL_MUTATIONS 200
#define NUMBER_OF_STEP_MUTATIONS 5
#define INITIAL_ALLOCATION_LENGTH 100000
#define SUBPOPULATION_SIZE 100
#define MAX_NEURON_OUTPUT 1000000


#define EPISODES_PER_INDIVIDUAL 1.0

//subpopulations
enum{FITNESS_BASED, NEURON_EFFICIENT, CONNECTION_EFFICIENT, NEURON_RICH, CONNECTION_RICH, NUMBER_OF_SUBPOPULATIONS};
#define MAIN_SUBPOP FITNESS_BASED

//spectrum diversity
#define SPECTRUM_DIVERSITY
#define SPECTRUM_SIZE 6
#define NOVELTY_MAP_SIZE 20
#define NUMBER_OF_SUBPOPULATIONS 1
//#define NORMALIZED_SPECTRUM_DIVERSITY

//1-add neuron, 2-del neuron, 3-add connection, 4-del connection
//#define MUTATION_PROBABILITIES {0.25, 0.05, 0.6, 0.1}
#define MUTATION_PROBABILITIES {0.01, 0.01, 0.49, 0.49}
#define CHANCE_OF_NEUROMODULATION 0.1
#define CHANCE_OF_CONTROL_NEURON 0.2
#define EXCITATION_THRESHOLD 0.0		//minimum excitation necessary to activate the neuron
#define CHANCE_OF_WEIGHT_MUTATION 0.5
#define WEIGHT_MUTATION_CHANGE_PERCENTAGE 1.0 //10 = 1000% change, 1 = 100% change possible


//  ----------  Environments  ---------- //

//general features

//#define SET_NORMALIZED_INPUT
//#define SET_NORMALIZED_OUTPUT
//stop and exit the process if the max steps are reached in one trial, it means that the algorithm reached the best solution
//#define TERMINATE_IF_MAX_STEPS_REACHED		



//Classification 
//#define	CLASSIFICATION_VERBOSE
#define RATIO_OF_SAMPLES_FOR_TRAINING 0.9
#define NUMBER_OF_TRAINING_REPETITIONS 100000
#define ADAPTIVE_REWARD		//more reward to missclassified datasets
///#define BOOSTING		//resample the dataset making missclassified objects appear more frequently

////////Reinforcement Learning///////

//General Maze 
#define MAX_MAZE_STEPS 500
#define ENVIRONMENT_NOISY_INPUT false
#define PATH_TO_MAZE_CONFIG "maze"

//Mountain Car
#define MAX_MOUNTAIN_CAR_STEPS 1000
//#define	NOISY_MOUNTAIN_CAR
#define CONTINUOUS_MOUNTAIN_CAR	//if defined only one continuous output, otherwise 3 outputs

//#define CHANGING_MCAR_MAX_VELOCITY
#define MODIFIED_MCAR_MAX_VELOCITY 0.04
//#define MCAR_TRIALS_TO_CHANGE 10000
//#define MCAR_TRIALS_TO_CHANGE 100
#define MCAR_TRIALS_TO_CHANGE 1

#define MCAR_MIN_POSITION -1.2
#define MCAR_MAX_POSITION 0.6
#define MCAR_MAX_VELOCITY 0.07            // the negative of this in the minimum velocity
#define MCAR_GOAL_POSITION 0.6

//Double Pole Balancing
#define MAX_DOUBLE_POLE_STEPS 100000
#define NON_MARKOV_DOUBLE_POLE

//Swing Up Pendulum
#define MAX_PENDULUM_STEPS 1000

//Function Approximation
#define	SEQUENTIAL_FUNCTION_APPROXIMATION	
//#define	SUPERVISED_FUNCTION_APPROXIMATION
//#define NUMBER_OF_FUNCTION_APPROXIMATION 2
//#define	MULTIPLE_RANDOM_FUNCTION_APPROXIMATION

//Single Pole Balancing
//#define MAX_SINGLE_POLE_STEPS 10000
#define MAX_SINGLE_POLE_STEPS 10000
#define RANDOM_START false

//Go Game
//A Go game should not be ended because of maximum steps, therefore 
//a big enough number was chosen 
#define MAX_GO_STEPS 1000000 
#define BOARDSIZE 9


struct cell_contents
{
	int best_index[BEST_SOLUTIONS_SIZE];
	double best_fitness[BEST_SOLUTIONS_SIZE];
	int best_counter[BEST_SOLUTIONS_SIZE];

	int novel_index[NOVEL_SOLUTIONS_SIZE];
	double novel_fitness[NOVEL_SOLUTIONS_SIZE];
	int novel_counter[NOVEL_SOLUTIONS_SIZE];

	int index_of_last_acted;	//population index, of the individual who acted last in this cell
	int when_last_acted;		//generation or related number, when the last acted from this cell
	
	double experience;
	double accum_experience;
};


#endif
