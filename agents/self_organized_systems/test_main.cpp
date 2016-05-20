
#include"random/State_of_Art_Random.h"
#include"Cell.h"
#include"Som_Map.h"
#include"time.h"

int main()
{
	int width=3;
	int height=3;
	int size=2;
	int number_of_basis_vectors=0;
	Som_Map* som;

	Random* random= new State_of_Art_Random(time(NULL));

	som= new Som_Map(width,height,size,number_of_basis_vectors);

	som->print();

	//exit(1);
		
	double input_array[2];
		
	int input=1;
	while(input != 0)
	{
		printf("Range\n");
		double x,y;
		printf("x ");
		scanf("%lf",&x);
		printf("y ");
		scanf("%lf",&y);

		int i;
		for(i=0;i<10;++i)
		{
			//input_array[0]= input_array[0] + x*rand()/double(RAND_MAX);
			//input_array[1]= input_array[1] + y*rand()/double(RAND_MAX);
			input_array[0]= random->uniform(x,y);
			input_array[1]= random->uniform(x,y);

			//printf("\ninput %f\n",input_array[0], input_array[1]);
			som->input(input_array);
		}

		som->print();
		som->printError();

		//som->insertRow(1,1,0);
		//som->insertColumn(1,1,-1);
		printf("\n");
		som->print();

		printf("One more time (1 - yes/0 - no)): ");
		scanf("%d",&input);
	}

	return 0;
}
