set border lc rgb "white"
set grid lc rgb "white" lt 2
set key opaque box
set object 1 rect behind from screen 0,0 to screen 1,1 fc rgb "#B8B2C3" fillstyle solid

plot 'error_loss.dat' using 1:2 t "varidation"  with line linewidth 2 linecolor rgbcolor "red"
replot 'error_loss.dat' using 1:3  t "test sample" with line linewidth 2 linecolor rgbcolor "blue"
pause 10
reread
