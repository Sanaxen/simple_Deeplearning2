set border lc rgb "white"
set grid lc rgb "white" lt 2
set key opaque box
set object 1 rect behind from screen 0,0 to screen 1,1 fc rgb "#B8B2C3" fillstyle solid
set key right bottom

# smooth [unique, csplines, acsplines, bezier, sbezier]

plot 'accuracy_rate.dat' using 1:2  t "varidation accuracy"  with lines linewidth 2 linecolor rgbcolor "red"
replot 'accuracy_rate.dat' using 1:3  t "test sample accuracy" with lines linewidth 2 linecolor rgbcolor "blue"

pause 10
reread
