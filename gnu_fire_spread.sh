# echo "\
# set terminal png size 1920,1080
# set palette grey negative
# stats 'forest.tr' nooutput
# #print int(STATS_blank)
# do for [i=0:(int(STATS_blank)-1)] {
#     set output sprintf('matrix%03.0f.png',i)
#     plot 'forest.tr' index i matrix with image
# }" | gnuplot


echo "\
set terminal gif animate delay 50
unset tics
unset key
set output 'full_gnuplot.gif'
set palette grey negative
stats 'forest.tr' nooutput
set yrange [:] reverse
do for [i=0:int(STATS_blank)-1] {
   plot 'forest.tr' index (i) matrix with image
}" | gnuplot
