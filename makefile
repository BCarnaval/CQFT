OPTIONS = -O2 -Wall -ffast-math
EXEC = ./build/compute
SOURCE = $(shell find . -name "*.c" -type f)
EX_SRC = ./examples/
RM=-rm -f
GARBAGE={eps,tex,log,aux}

.PHONY: all executable plot clean

all: executable

executable: $(SOURCE)
	@gcc $(OPTIONS) -o $(EXEC) $(SOURCE) -lm

plot:
	@$(EXEC)
	@gnuplot -p $(EX_SRC)plot.gp

clean:
	@$(RM) $(EX_SRC)*.$(GARBAGE)
	@$(RM) $(EX_SRC)*-inc-*
