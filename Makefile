# EXEC = "name of the executable"
EXEC=main
# OBJECTS = all .o object files compiled but not linked
OBJECTS= *.o
# SCR = list of all .cpp source files
SRC=main.cpp 
# object files --> executable (linking)
$(EXEC): $(OBJECTS)
	g++ $(OBJECTS) -o $(EXEC)
# source files --> object files (compilation)
$(OBJECTS): $(SRC)
	g++ $(SRC) -c
# cleaning and running routine (optional)
clean:
	@rm -f  *.o ./main
run:
	@./$(EXEC)

