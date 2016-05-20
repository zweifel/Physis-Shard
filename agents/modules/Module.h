///////////////////////////////////////////////////////////////////
//
//		Modules  - create the building blocks for the system (a system is composed of a set of interconnected modules)
//
//	Temporarely some features let the Modules be used as an entire system to be evaluated:
//	(they should be removed and substituted for the real module file)
//		-createFixedInterfaceNeurons()
//		-do not remove Input or Output neurons
//		-do not create Input or Output neurons (the position of NUMBER_OF_NEURON_TYPE and add interface_index to the creation of neurons )
//		-add a vector with the id of  input and output neurons, keeping track of the interface_index
//
///////////////////////////////////////////////////////////////////


#ifndef MODULE_H	
#define MODULE_H

#include"structural_dna.h"
#include"stdlib.h"
#include<stack>
#include<list>
#include"string.h"
#include"useful/useful_utils.h"
#include"../../parameters.h"

#define RECURRENT -1
#define FEEDFORWARD 1
#define MAXIMUM_WEIGHT 100000000000000000000.0

struct connection_type {int index; int type;} ;

class Module
{
	public:

		Module(int number_of_inputs, int number_of_outputs, neuron* n, connection* c, int dna_allocated_length);
		Module(int number_of_inputs, int number_of_outputs, int suggested_allocation_length);
		~Module();	

		static void setRandom(Random* random);

		int allocated_space;
		int max_neuron_id;
		int number_of_neurons;
		int number_of_connections;
		int number_of_primers;

		//DNA
		neuron* n;
		connection* c;

		static Random* random;

		double* input;
		double* output;

		//neuron states:
		//
		//previous_neuron_state -> t-1
		//neuron_state -> t
		//internal_neuron_state -> t , but not acessed by other neurons, used by slow neurons to accumulate the input
		double* internal_neuron_state;
		double* neuron_state;
		double* previous_neuron_state;
		
		//neuron_excitation -> excitation (positive)/inhibition(zero or negative) -> positive values let the neuron activate
		//activation_counter:
		//	- negative values, the neuron was already activated in this process
		//	- positive values, count the number of times that the neuron was activated without giving the output away, mainly for slow neurons
		double* neuron_excitation;	
		//int* activation_counter;
		bool* is_fired;
		//bool* is_feedback;
		int* connection_state;
		int* previous_connection_state;

		//ready to use information	
		//int* map_id_to_dna;
		int* primer_list;

		void clone(Module* brother);
		void process(double* input, double* output);
		void structuralMutation();
		void firingRateMutation();
		void weightMutation();
		void addConnection(int from_neuron_id, int to_neuron_id, int neuro_modulation, double weight);
		void connectNewNeuronToNetwork(int new_neuron_id);
		void loadDNA(const char* filename);
		void saveDNA(const char* filename);

		//auxiliary
		int neuronIdToDNAIndex(int id);
		int smallestFreeId();
		void updatePrimerList();
		bool checkValidity();
		void clearMemory();
		void updateInformation();
		double execute(int neuron_index, bool ignore_if_all_recurrent = false);
		void processPrimers();
		void processControlNeurons();
		void processInputNeurons();
		void processRemainingNeurons();
		void removeConnection(int index);
		void reallocEverything();
		void reallocEverything(int suggested_allocation_length);
		void createFixedInterfaceNeurons(int number_of_inputs, int number_of_outputs, int input_type, int output_type );

		//debug & visualize
		void printGraph(const char* filename);
		void printInformationFlowGraph(const char* filename);
		void printDNA();
		void printDNA(const char* filename);
		void printVars();	//print module's vectors 	
		void printInternalStates();
		void printFiredStates();
};

#endif
