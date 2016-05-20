#!/bin/bash

for i in `ls som_map*`
do
	#echo $i > processing_som_map
	Rscript plot_som_map.r $i 
done

#wait

mv *.jpg movie/

rm som_map*

cd movie

ffmpeg -start_number 0 -i som_map%d.jpg -vcodec mpeg4 test.avi

rm som_map*

echo pause| mplayer -slave test.avi
