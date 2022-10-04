/* Global settings */
set terminal epslatex standalone
set output 'examples/test.tex'

/* Actual plotting */
set title 'Density as a function of $\mu$'
set xlabel '$\mu$'
set ylabel 'Density'
plot 'examples/conductivities.dat' using 1:2 title 'Density $n(\mu)$'

/* Close outputed file */
unset output

/* System commands */
system("pdflatex -output-directory='./examples/' './examples/test.tex'")
system("make clean")
system("open examples/test.pdf")
