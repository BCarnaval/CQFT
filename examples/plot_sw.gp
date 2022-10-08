# Global settings
set output 'examples/s_w.pdf'

set xrange[-pi:pi]
set yrange[-pi:pi]
unset key
set view map
set tics out
set tics nomirror

set pm3d map
set palette defined (-10 "white", 0 "orange", 10 "grey")

splot 'examples/spectralWeight.dat'
