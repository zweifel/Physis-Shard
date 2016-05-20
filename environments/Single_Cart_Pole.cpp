#include"Single_Cart_Pole.h"

Single_Cart_Pole::Single_Cart_Pole(Random* random)
{
	this->random= random;
	trial=-1;	//the first trial will be zero, because restart() is called making it increment
	MAX_STEPS= MAX_SINGLE_POLE_STEPS;
}

Single_Cart_Pole::~Single_Cart_Pole()
{

}

void Single_Cart_Pole::start(int &number_of_observation_vars, int& number_of_action_vars)
{
	number_of_observation_vars= 4;
	this->number_of_observation_vars= 4;
	observation= (double*) malloc(number_of_observation_vars*sizeof(double));

	number_of_action_vars=2;
	this->number_of_action_vars=2;

	restart();
}

//     cart_and_pole() was take directly from the pole simulator written
//     by Richard Sutton and Charles Anderson.
//     This simulator uses normalized, continous inputs instead of 
//    discretizing the input space.
/*----------------------------------------------------------------------
  NOW cart_pole takes a continuous action from the spectrum (-10,10) and the 
  current values of the four state variables and updates their values by estimating the state
 TAU seconds later. 
  
  Originally, cart_pole took an action (0 or 1) 
----------------------------------------------------------------------*/
void Single_Cart_Pole::cart_pole(double force, double *x,double *x_dot, double *theta, double *theta_dot) 
{
  double xacc,thetaacc,costheta,sintheta,temp;
  
  const double GRAVITY=9.81;
  const double MASSCART=1.0;
  const double MASSPOLE=0.1;
  const double TOTAL_MASS=(MASSPOLE + MASSCART);
  const double LENGTH=0.5;	  /* actually half the pole's length */
  const double POLEMASS_LENGTH=(MASSPOLE * LENGTH);
  const double TAU=0.02;	  /* seconds between state updates */
  const double FOURTHIRDS=1.3333333333333;

  //force = (action>0)? FORCE_MAG : -FORCE_MAG;
  //check if the force exceeds the boundaries
  if(force > FORCE_MAG)
  {
	  force= FORCE_MAG;
  }
  else
  {
	  if(force < -FORCE_MAG)
	  {
		force = -FORCE_MAG;
	  }
  }

  costheta = cos(*theta);
  sintheta = sin(*theta);
  
  temp = (force + POLEMASS_LENGTH * *theta_dot * *theta_dot * sintheta)
    / TOTAL_MASS;
  
  thetaacc = (GRAVITY * sintheta - costheta* temp)
    / (LENGTH * (FOURTHIRDS - MASSPOLE * costheta * costheta
		 / TOTAL_MASS));
  
  xacc  = temp - POLEMASS_LENGTH * thetaacc* costheta / TOTAL_MASS;
  
  /*** Update the four state variables, using Euler's method. ***/
  
  (*x)  += TAU * (*x_dot);
  (*x_dot) += TAU * xacc;
  (*theta) += TAU * (*theta_dot);
  (*theta_dot) += TAU * thetaacc;
}

double Single_Cart_Pole::step(double* action)
{
	// initial reward
	if(action==NULL)
	{
		return 1;
		//return 1/(x*x + theta*theta + 0.001);             
	}

   	const double TWELVE_DEGREES=0.2094384;

	double force;

	if(action[0] > action[1])
	//if(action[0] > 0.0)
	{
		force= -FORCE_MAG;
	}
	else
	{
		force= FORCE_MAG;
	}

	//--- Apply action to the simulated cart-pole ---
	cart_pole(force, &x, &x_dot, &theta, &theta_dot);
	
	//copy the state to the agents' observable variables
	observation[0]=x;
	//observation[0]= (x + 2.4) / 4.8;
	observation[1]=x_dot;
	//observation[1]=(x_dot + .75) / 1.5;
	observation[2]=theta;
	//observation[2]=(theta + TWELVE_DEGREES) / .41;
	observation[3]=theta_dot;
	//observation[3]=(theta_dot + 1.0) / 2.0;

	//--- Check for failure.  If so, return steps ---
	if(x < -2.4 || x > 2.4  || theta < -TWELVE_DEGREES ||	theta > TWELVE_DEGREES) 
	{
		restart();
		//return 1/(x*x + theta*theta + 0.001);             
		return -10;
	}

	return 1;
	//return 1/(x*x + theta*theta + 0.001);             
}

double Single_Cart_Pole::restart()
{
	trial++;

	if(RANDOM_START)
	{
		x= random->uniform(-1.4, 1.4);
		//x_dot = random->uniform(-1, 1);
		x_dot = 0;
		theta = random->uniform(-0.1, 0.1);
		//theta_dot = random->uniform(-1.5, 1.5);
		theta_dot= 0;
	}
	else
	{
		x = 0;
		x_dot = 0;
		theta = 0;
		theta_dot = 0;
	}

	//copy the state to the agents' observable variables
	observation[0]=x;
	//observation[0]= (x + 2.4) / 4.8;
	observation[1]=x_dot;
	//observation[1]=(x_dot + .75) / 1.5;
	observation[2]=theta;
	//observation[2]=(theta + TWELVE_DEGREES) / .41;
	observation[3]=theta_dot;
	//observation[3]=(theta_dot + 1.0) / 2.0;

	return 1;
	//return 1/(x*x + theta*theta + 0.001);             
}

void Single_Cart_Pole::print()
{

}



/*
	double tmp= (force + pole_mass*length*angular_velocity*angular_velocity*sin(angle))/(cart_mass + pole_mass);

	angular_acceleration= gravity*sin(angle) - cos(angle)*tmp;
	angular_accelaration/= l*( (4/3) - pole_mass*cos(angle)*cos(angle)/(cart_mass+pole_mass) );

	cart_accel= F - pole_mass*l*(angular_acceleration*cos(angle)-angular_velocity*angular_velocity*sin(angle));
	cart_accel/= cart_mass + pole_mass;

	double k= 
	cart_position+=  

	*/
