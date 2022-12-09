RM=-rm -f
EX_SRC=./examples/
EXEC=./build/compute
GARBAGE={eps,tex,log,aux}
OPTIONS=-O2 -Wall -ffast-math
SOURCE=$(shell find . -name "*.c" -type f)

.PHONY: all executable plot clean

all: executable

plot: executable
	@$(EXEC)
	@gnuplot -p $(EX_SRC)plot_density.gp

executable: $(SOURCE)
	gcc $(OPTIONS) -o $(EXEC) $(SOURCE) -lm

clean:
	@$(RM) $(EX_SRC)*.$(GARBAGE)
	@$(RM) $(EX_SRC)*-inc-*
