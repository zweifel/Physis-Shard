#!/bin/bash

repetitions=30
folder=mean_results/

mkdir $folder

for i in `seq 1 $repetitions`
do
	echo "test $i"
	./rl > $folder/$i
	cp dna_best_individual $folder/dna$i
done

cp parameters.h $folder
cp main.cpp $folder
cp calculate_average.r $folder
cp equalize_results.r $folder
