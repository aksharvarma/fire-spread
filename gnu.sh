echo 'set terminal png size 1920,1080
set output "time_all.png"
set grid
set key inside left top width 5
set key font ",18"
set key spacing 2
set title "How the forest evolves" enhanced
set logscale x
plot	"forest_data.tr" using 1:2 with lines lw 2 title "Empty",\
 	"forest_data.tr" using 1:3 with lines lw 2 title "Burning",\
 	"forest_data.tr" using 1:4 with lines lw 2 title "Trees Left",\
' | gnuplot



