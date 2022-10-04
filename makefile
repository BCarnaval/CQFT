OPTIONS = -O2 -Wall -ffast-math
EXEC = ./build/compute
SOURCE = $(shell find . -name "*.c" -type f)

all: executable

executable: $(SOURCE)
	@gcc $(OPTIONS) -o $(EXEC) $(SOURCE) -lm

