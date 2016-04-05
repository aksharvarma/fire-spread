echo 'set terminal png size 1920,1080
set output "time_all.png"
set grid
set key inside left top width 5
set key font ",18"
set key spacing 2
set title "Time (all)" enhanced
set logscale x
plot	"all.tr" using 1:2 with lines lw 2 title "Serial",\
 	"all.tr" using 1:3 with lines lw 2 title "Parallel"\
' | gnuplot


echo 'set terminal png size 1920,1080
set output "speed_all.png"
set grid
set key inside left top width 5
set key font ",18"
set key spacing 2
set title "Speedup (all)"
set logscale x
plot 	\
 	"all.tr" using 1:4 with lines lw 2 title "Parallel"\
' | gnuplot


