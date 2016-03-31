echo '\
set terminal png size 1920,1080
set output "fire.png"
set size ratio 1
set palette gray negative
# grid lines
set x2tics 1 format "" scale 0,0.001
set y2tics 1 format "" scale 0,0.001
set mx2tics 2
set my2tics 2
\
# labeling
set xtics 5 out nomirror
set ytics 5 out nomirror
\
set grid front mx2tics my2tics lw 1.5 lt -1 lc rgb "white"
\
set xrange[-0.5:39.5]
set yrange[-0.5:39.5]
set x2range[-0.5:39.5]
set y2range[-0.5:39.5]
\
'|gnuplot


# echo '\
# set terminal png size 1920,1080
# set output "fire.png"
# set border linewidth 0
# unset key
# unset colorbox
# unset tics
# set size ratio 1
# set lmargin screen 0.1
# set rmargin screen 0.9
# set tmargin screen 0.9
# set bmargin screen 0.1
# set palette grey negative
# plot "forest.tr" matrix with image
# ' | gnuplot
