CC=g++
CFLAGS=-c -Wall -O3
#CFLAGS = -c -Wall -ggdb
SOURCES=gmst.cpp graph.cpp instance.cpp constructor.cpp heuristic6.cpp heuristic7.cpp heap.cpp heapmember.cpp heuristic1.cpp searcher.cpp heuristic9.cpp parameters.cpp  heuristic5.cpp util.cpp eliteset.cpp instancegenerator.cpp tabu.cpp grasp.cpp setunion.cpp heuristic2.cpp evolution.cpp population.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gmst

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o gmst
	echo Clean done
