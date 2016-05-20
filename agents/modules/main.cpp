
#include"stdlib.h"
#include"stdio.h"
#include"time.h"

#include"Module.h"

int main()
{
/*	neuron n[5];
	
	n[0].id=0;
	n[0].firing_rate=1;
	n[0].type=SIGMOID;
	
	n[1].id=1;
	n[1].firing_rate=1;
	n[1].type=CONTROL;
	
	n[2].id=2;
	n[2].firing_rate=2;
	n[2].type=RANDOM;
	
	n[3].id=3;
	n[3].firing_rate=3;
	n[3].type=OUTPUT_SIGMOID;

	n[4].id=-1;


	connection c[6];

	c[0].from_neuron_id=1;
	c[0].to_neuron_id=2;
	c[0].weight=-1;
	c[0].neuro_modulation=-1;
	
	c[1].from_neuron_id=1;
	c[1].to_neuron_id=3;
	c[1].weight=1;
	c[1].neuro_modulation=-1;
	
	c[2].from_neuron_id=0;
	c[2].to_neuron_id=3;
	c[2].weight=1;
	c[2].neuro_modulation=2;
	
	c[3].from_neuron_id=3;
	c[3].to_neuron_id=3;
	c[3].weight=1;
	c[3].neuro_modulation=1;
	
	c[4].from_neuron_id=2;
	c[4].to_neuron_id=0;
	c[4].weight=1;
	c[4].neuro_modulation=2;

	c[5].from_neuron_id=-1;
*/			

	neuron* n= (neuron*)malloc(sizeof(neuron)*8);

	n[0].id=-1;

	connection *c= (connection*)malloc(sizeof(connection)*8);

	c[0].from_neuron_id=-1;


	Random* random= new State_of_Art_Random(time(NULL));

	int input_size=2;
	float* input= (float*)malloc(sizeof(float)*input_size);
	int output_size=1;
	float* output= (float*)malloc(sizeof(float)*output_size);

	Module* m= new Module(input_size, output_size, n,c,8);

	Module::setRandom(random);
	
	m->printVars();
	
	int tsuzuku=1;
	for(;tsuzuku==1;)
	{
		m->structuralMutation();
		m->structuralMutation();
		m->structuralMutation();
		m->structuralMutation();
		m->structuralMutation();
		
		m->process(input, output);

		m->printInformationFlowGraph("graph.dot");
		//m->printVars();

		scanf("%d",&tsuzuku);
	}
/*	
	m->printDNA();
	m->printGraph("graph.dot");

	m->printVars();
*/
	return 0;
};
