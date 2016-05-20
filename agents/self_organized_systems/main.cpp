
#include"random/State_of_Art_Random.h"
#include"Cell.h"
#include"Som_Map.h"
#include"time.h"
#include"useful/read_csv.h"
#include"Novelty_Map.h"

int main()
{
	int width=7;
	int height=7;
	int size=2;
	int number_of_basis_vectors=0;
	Som_Map* som;
	Novelty_Map* nmap;

	Random* random= new State_of_Art_Random(time(NULL));

	som= new Som_Map(width,height,size,number_of_basis_vectors);
	//nmap= new Novelty_Map(width*height,size);

	//som->print();

	int attributes;
	int samples;
	double** matrix;
	matrix= readCSV("circular_exp", samples, attributes, false," ");
	//matrix= readCSV("datasets/ruspini.txt", samples, attributes, false," ");

	printf("samples %d attributes %d\n",samples, attributes);
		
	double input_array[2];

	char print_filename[128];

	int i;
	for(i=0;i<samples;++i)
	{
		input_array[0]= matrix[i][0];
		input_array[1]= matrix[i][1];

		//printf("%f %f\n",input_array[0],input_array[1]);

		som->input(input_array);
		//nmap->input(input_array);

		//som->insertRow(1,1,0);
		//som->insertColumn(1,1,-1);
		
		sprintf(print_filename,"som_map%d",i);
		
		som->printToFile(print_filename);
		//nmap->printToFile(print_filename);
	}

	return 0;
}
