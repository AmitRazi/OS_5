# define the C++ compiler to use
CXX = g++

# define any compile-time flags
CFLAGS = -Wall -g

# define the executable file
MAIN = st_pipeline

.PHONY: clean

all: $(MAIN)
	@echo Program has been compiled

$(MAIN): ActiveObject.o concurrentQueue.o st_pipeline.o
	$(CXX) $(CFLAGS) -o $(MAIN) ActiveObject.o concurrentQueue.o main.o -lpthread

ActiveObject.o: ActiveObject.cpp
	$(CXX) $(CFLAGS) -c ActiveObject.cpp

concurrentQueue.o: concurrentQueue.cpp
	$(CXX) $(CFLAGS) -c concurrentQueue.cpp

st_pipeline.o: st_pipeline.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	$(RM) *.o *~ $(MAIN)
