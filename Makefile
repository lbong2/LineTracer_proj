RESULT = term_project_lt
SRC = $(RESULT).cpp
CCC = g++

all:
	$(CCC) -o $(RESULT) $(SRC) $(pkg-config opencv4 --libs --cflags) -lwiringPi

clean:
	rm -f $(RESULT)
