# Global settings
set terminal epslatex standalone
set output 'examples/s_w.tex'

set xrange[0:400]
set yrange[0:400]
unset key
set view map
set tics out
set tics nomirror

set pm3d map
set palette rgb 33,13,10

splot 'examples/spectralWeight.dat' w image

unset output

# System commands
system("pdflatex -output-directory='./examples/' './examples/s_w.tex'")
system("make clean")
system("open examples/s_w.pdf")
