CC = gcc
CFLAGS = -Iscr -g -Wall -Wextra
#-O2 -flto -march=native
LDFLAGS = -lm
OBJ_SIMULA = src/lattice.o src/simula.o src/parisi.o
SRC_SIMULA = lattice.c simula.c parisi.c
OBJ_ANALYZE = src/analyze.o
SRC_ANALYZE = analyze.c

all: 
	make simula
	@echo "simula compiled"
	@echo ""
	make analyze
	@echo "analyze compiled"
	@echo ""
	@echo "simula & analyze compiled successfuly"

simula: $(OBJ_SIMULA)
	$(CC) $(OBJ_SIMULA) -o simula $(CFLAGS) $(LDFLAGS)

src/parisi.o: src/parisi.c
src/lattice.o: src/lattice.c
src/simula.o: src/simula.c src/lattice.h

analyze: $(OBJ_ANALYZE)
	$(CC) $(OBJ_ANALYZE) -o analyze $(CFLAGS) $(LDFLAGS)

src/analyze.o: src/analyze.c

clean:
	@rm src/*.o
	@echo "deleting of object files completed"
	@rm simula analyze
	@echo "simula & analyze deleted"
