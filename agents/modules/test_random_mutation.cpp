
#include"stdlib.h"
#include"stdio.h"
#include"time.h"

#include"Module.h"

int main()
{
	Random* random= new State_of_Art_Random(time(NULL));
	int initial_size= 1000;
	
	int input_size=2;
	double* input= (double*)malloc(sizeof(double)*input_size);
	int output_size=1;
	double* output= (double*)malloc(sizeof(double)*output_size);
	
	Module::setRandom(random);

	input[0]=1;
	input[1]=1;
		

	Module* m= new Module(input_size, output_size, initial_size);

	int tsuzuku=1;
	for(;tsuzuku==1;)
	{
		for(int i=0;i<1000;++i)
		{
			m->structuralMutation();
			m->printGraph("graph.dot");
			//m->updatePrimerList();
			bool validity= m->checkValidity();

			if(validity==false)
			{
				printf("not valid\n");
				exit(1);
			}
		}

/*		
		m->process(input, output);

		//m->printVars();

		//m->printInternalStates();

		printf("output: %f \n",output[0]);
*/		
		//m->printInformationFlowGraph("graph.dot");
		
		printf("b");
		scanf("%d",&tsuzuku);
		
	}
/*	
	m->printDNA();
	m->printGraph("graph.dot");

	m->printVars();
*/
	return 0;
};
