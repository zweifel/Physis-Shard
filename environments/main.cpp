#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include<string.h>

#define PARENT_READ (read_fd[0])
#define PARENT_WRITE (write_fd[1])
#define CHILD_READ (write_fd[0])
#define CHILD_WRITE (read_fd[1])
	
int read_fd[2];
int write_fd[2];
char message[1024];
	
void readOpponentMove(char* received_msg)
{
	char buffer[100];
	int count = read(PARENT_READ, buffer, sizeof(buffer)-1);
	buffer[count]=0;   

	//clean the received_msg
	received_msg[0]=0;
	
	sprintf(received_msg,"%s%s",received_msg,buffer);
	while(buffer[count-1]!=10 || buffer[count-2]!=10)
	{
		count = read(PARENT_READ, buffer, sizeof(buffer)-1);
	
		buffer[count]=0;   
		sprintf(received_msg,"%s%s",received_msg,buffer);
	}
	printf("%s", received_msg);
}

void moveToMessage(int* move)
{
	if(move[0]>=9)
	{
		move[0]++;	
	}

	char number= move[0]+64;
	sprintf(message,"play black %c%d\n",number,move[1]);
	int count=write(PARENT_WRITE, message, (strlen(message)));
	printf("message: %s\n",message);
	
	//printf("sent\n%s\n",received_msg);

}

double messageToReward(char* received_msg)
{
	printf("%s\n",received_msg);

	bool win;
	if(received_msg[2]=='B')
	{
		win=true;
	}
	else
	{
		if(received_msg[2]=='W')
		{
			win=false;
		}
		//draw
		else
		{
			return 0.0;
		}
		
	}
	
	
	char buffer[8];
	int i=3;
	for(i=3;received_msg[i]!=32&&received_msg[i]!=10;++i)
	{
		buffer[i-3]=received_msg[i];
	}
	buffer[i-3]=0;
	double score= atof(buffer);

	if(win==false)
	{
		score= -score;
	}

	printf("score: %f\n",score);
	return score;
}

void messageToMove(int* move, char* received_msg)
{
	char number= received_msg[2];
	//the char 'A' is the same as 64 in integer
	move[0]= number -64;
	
	//correct the very strange absence of 'I' in the Go board
	if(move[0]>=10)
	{
		move[0]--;
	}

	char buffer[3];
	int i=3;
	for(i=3;received_msg[i]!=10;++i)
	{
		buffer[i-3]=received_msg[i];
	}
	buffer[i-3]=0;
	move[1]=atoi(buffer);

	printf("read %d %d\n",move[0],move[1]);

}

int main()
{

	int move[2];

	pipe(read_fd);
	pipe(write_fd);
	
	//spawn a child 
	pid_t pid=fork();

	//child process
	if (pid==0) 
	{ 
		dup2(CHILD_READ, STDIN_FILENO);
		dup2(CHILD_WRITE, STDOUT_FILENO);

		close((read_fd[0]));
		close((read_fd[1]));
		close((write_fd[0]));
		close((write_fd[1]));
		
		execl("/usr/bin/gnugo","/usr/bin/gnugo","--mode","gtp",NULL);
	}
	// pid!=0; parent process
	else 
	{
		char buffer[100];
		char received_msg[100];
		received_msg[0]=0;
		
		//closes output
		close(CHILD_WRITE);
		//closes input
		close(CHILD_READ);

		//waitpid(pid,0,0); /* wait for child to exit */

		int board_size=9;
		int count;

		
		move[0]=17;
		move[1]=17;
		moveToMessage(move);
		
		readOpponentMove(received_msg);
		//messageToMove(move, received_msg);
		
		write(PARENT_WRITE, "genmove white\n", 14);
		readOpponentMove(received_msg);
		messageToMove(move, received_msg);
		
		write(PARENT_WRITE, "genmove white\n", 14);
		readOpponentMove(received_msg);
		messageToMove(move, received_msg);
		
		write(PARENT_WRITE, "estimate_score\n", 15);
		readOpponentMove(received_msg);
		double score= messageToReward(received_msg);	
		
		/*sprintf(message,"showboard\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		readOpponentMove(received_msg);
		*/
		exit(1);
		
		write(PARENT_WRITE, "genmove white\n", 14);
		readOpponentMove(received_msg);
		
		sprintf(message,"genmove white\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message: %s\n",message);

		// Read from child’s stdout
		readOpponentMove(received_msg);
		
		messageToMove(move, received_msg);

		received_msg[0]=0;
		
		
		sprintf(message,"genmove white\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		
		// Read from child’s stdout
		readOpponentMove(received_msg);
		
		messageToMove(move, received_msg);
		
		sprintf(message,"genmove white\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		
		// Read from child’s stdout
		readOpponentMove(received_msg);
		messageToMove(move, received_msg);
	
		/*	sprintf(message,"clear_board\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		
		readOpponentMove(received_msg);
		received_msg[0]=0;
	*/	
		sprintf(message,"showboard\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		
		readOpponentMove(received_msg);
		
		sprintf(message,"genmove white\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		
		// Read from child’s stdout
		readOpponentMove(received_msg);
		
		sprintf(message,"genmove white\n");
		count=write(PARENT_WRITE, message, (strlen(message)));
		printf("message2 %s\n",message);
		
		// Read from child’s stdout
		readOpponentMove(received_msg);

		printf("finished:\n");

		close(PARENT_READ); 
		close(PARENT_WRITE); 
	}
	
		

	return 0;
}

