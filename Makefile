ROOT=/home/meskal_01
# EXEC = "name of the executable"
EXEC=main
# OBJECTS = all .o object files compiled but not linked
OBJECTS= *.o
# SCR = list of all .cpp source files
SRC=main.cpp 
# object files --> executable (linking)
$(EXEC): $(OBJECTS)
	g++ $(OBJECTS) -L$(ROOT)/pacs-examples/Examples/lib -lmuparser -o $(EXEC)
# source files --> object files (compilation)
$(OBJECTS): $(SRC)
	g++ $(SRC) -I$(ROOT)/pacs-examples/Examples/include -lmuParser -c
# cleaning routine (optional)
clean:
	@rm -f  *.o ./main
run:
	@LD_LIBRARY_PATH=$(ROOT)/pacs-examples/Examples/lib  ./$(EXEC)

