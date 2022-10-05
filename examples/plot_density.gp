# Global settings
set terminal epslatex standalone
set output 'examples/density.tex'
set colorsequence podo

# Actual plotting
set xlabel '$\mu$'
set ylabel 'Density'
set key left top
plot 'examples/conductivities.dat' u 1:2 t 'Density $n(\mu)$' w lp lc 3 ps 0.5

# Close outputed file
unset output

# System commands
system("pdflatex -output-directory='./examples/' './examples/density.tex'")
system("make clean")
system("open examples/density.pdf")
