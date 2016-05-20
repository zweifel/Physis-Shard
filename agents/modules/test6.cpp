
#include"stdlib.h"
#include"stdio.h"
#include"time.h"

#include"Module.h"

int main()
{
	int initial_size= 10;
	neuron* n= (neuron*)malloc(sizeof(neuron)*initial_size);

	n[0].id=0;
	n[0].firing_rate=1;
	n[0].type=CONTROL;
	
	n[1].id=1;
	n[1].firing_rate=3;
	n[1].type=IDENTITY;
	
	n[2].id=2;
	n[2].firing_rate=1;
	n[2].type=IDENTITY;
	
	n[3].id=3;
	n[3].firing_rate=3;
	n[3].type=IDENTITY; ///SIGMOID;
	
	n[4].id=4;
	n[4].firing_rate=1;
	n[4].type=CONTROL;

	n[5].id=-1;
	
	connection *c= (connection*)malloc(sizeof(connection)*initial_size);

	c[0].from_neuron_id=0;
	c[0].to_neuron_id=1;
	c[0].weight=1;
	c[0].neuro_modulation=-1;
	
	c[1].from_neuron_id=1;
	c[1].to_neuron_id=3;
	c[1].weight=1;
	c[1].neuro_modulation=-1;
	
	c[2].from_neuron_id=0;
	c[2].to_neuron_id=1;
	c[2].weight=1;
	c[2].neuro_modulation=-1;
	
	c[3].from_neuron_id=0;
	c[3].to_neuron_id=2;
	c[3].weight=1;
	c[3].neuro_modulation=-1;
	
	c[4].from_neuron_id=0;
	c[4].to_neuron_id=3;
	c[4].weight=1;
	c[4].neuro_modulation=-2;
	
	c[5].from_neuron_id=2;
	c[5].to_neuron_id=3;
	c[5].weight=1;
	c[5].neuro_modulation=-1;
	
	c[6].from_neuron_id=4;
	c[6].to_neuron_id=3;
	c[6].weight=1;
	c[6].neuro_modulation=-1;
	
	c[7].from_neuron_id=0;
	c[7].to_neuron_id=4;
	c[7].weight=1;
	c[7].neuro_modulation=-1;
	
	c[8].from_neuron_id=3;
	c[8].to_neuron_id=4;
	c[8].weight=-1;
	c[8].neuro_modulation=-1;
	
	c[9].from_neuron_id=1;
	c[9].to_neuron_id=4;
	c[9].weight=1;
	c[9].neuro_modulation=-1;

	c[10].from_neuron_id=-1;



	Random* random= new State_of_Art_Random(time(NULL));

	int input_size=2;
	float* input= (float*)malloc(sizeof(float)*input_size);
	int output_size=1;
	float* output= (float*)malloc(sizeof(float)*output_size);
	
	Module::setRandom(random);

	Module* m= new Module(input_size, output_size, n,c,initial_size);
		
	//void addConnection(int from_neuron_id, int to_neuron_id, int neuro_modulation, float weight);
	m->addConnection(5, 1, -1, 1.0);
	m->addConnection(0, 5, -1, 1.0);
	m->addConnection(0, 6, -1, 1.0);
	m->addConnection(3, 7, -1, 1.0);
	m->addConnection(6, 2, -1, 1.0);
	m->addConnection(2, 7, -1, 1.0);
	
	m->printVars();

	input[0]=1;
	input[1]=1;
		
	m->printGraph("graph.dot");
	
	int tsuzuku=1;
	for(;tsuzuku==1;)
	{
		//m->structuralMutation();
		//m->structuralMutation();
		//m->structuralMutation();
		//m->structuralMutation();
		//m->structuralMutation();
		
		m->process(input, output);

		//m->printVars();

		//m->printInternalStates();

		printf("output: %f \n",output[0]);
		
		//m->printInformationFlowGraph("graph.dot");
		
		
		scanf("%d",&tsuzuku);
	}
/*	
	m->printDNA();
	m->printGraph("graph.dot");

	m->printVars();
*/
	return 0;
};
