# Physis Shard

Physis Shard is a framework for developing learning systems.
It defines basically two simple interfaces that allows easy addition of agents and problems.
In other words, to add agents or problems they only need to implement these interfaces.

## Contents

Agents:
 - **S**pectrum-diverse **U**nified **N**euron **E**volution **A**rchitecture (SUNA) 
 - Random agent (named Mysterious_Agent)
 - Dummy agent (the user define its output every iteration)

Environments (i.e., Problems):
 - Mountain Car
 - Double Cart Pole (with and without velocities)
 - Function Approximation
 - Multiplexer
 - Single Cart Pole
 
## Install 

This library depends on the zweifel library.
You can install it from the git address:
https://github.com/zweifel/zweifel

After installing the zweifel library, its full path need to be defined in the Physis Shard's Makefile.
In other words, change the following variable in the Makefile to point to zweifel library's correct installing location:
```
PATH_TO_ZWEIFEL_LIBRARY=/home/user/zweifel
```

Run make afterwards:
```
make
```

It should produce two executables: rl and rl_live.
To run tests run:
```
./rl
```

At the end of the test, the best solution's dna will be saved with the name dna_best_individual

To test this final solution run:
```
./rl_live dna_best_individual
```

## Changing Environments 

Environment can be changed in main.cpp.
For example commenting out where the Reinforcement_Environment is defined and
uncommenting the line with:
```
Reinforcement_Environment* env= new Double_Cart_Pole(random);
```

If the environment should be terminated when the maximum steps is reached
uncomment the following in parameters.h:
```
#define TERMINATE_IF_MAX_STEPS_REACHED		
```
Do not forget to comment it out when surpassing the maximum number of steps is
not a termination condition! For example, montain car does not need it while
double cart pole does.

## Changing Parameters

Many parameters of the environment as well as of the agent can be changed by modifying some definitions
in parameters.h

## Running Experiments 

To run a trial until its maximum number of trials defined in main.cpp, run: 
```
./rl
```

To test the best individual, run:
```
./rl_live dna_best_individual
```

A series of trials can be run by using the script mean_curve.sh

## Adding Agents or Problems

An agent needs to implement the Reinforcement_Agent.h while a problem needs to implement the Reinforcement_Environment.h.
There are simple examples of agents and problems inside respectively the agents/ and environments/ directories.
Most of the examples were built with the general reinforcement learning in mind, however they can be applied to supervised learning as well as unsupervised learning (e.g., consider the reward from the system as an error).

## License

Apache License Version 2.0
