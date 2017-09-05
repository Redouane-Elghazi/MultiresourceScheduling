PROG		= main
PRODNAME	= schedule_sim
CXX			= g++
CXXFLAGS	= -Wall -O3 -std=c++11
LIBRARY		= multivect task lowerbound approx_transfopack approx_transfononpack approx_multipack approx_nonpack transformation profiles

OBJECTS		= $(LIBRARY:=.o) $(PROG:=.o)
TASKCREATORS= tasks/ahmdalmax tasks/ahmdalsum tasks/alphamax tasks/alphaprod
TASKLIB		= multivect task profiles

default : $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(STD) -o $(PRODNAME) $^

all: default $(TASKCREATORS)
	
main.o : simulator/main.cpp simulator/multivect.hpp simulator/task.hpp simulator/lowerbound.hpp simulator/approx_transfopack.hpp simulator/approx_transfononpack.hpp simulator/approx_multipack.hpp simulator/approx_nonpack.hpp simulator/profiles.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

multivect.o : simulator/multivect.cpp simulator/multivect.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

task.o : simulator/task.cpp simulator/task.hpp simulator/multivect.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
lowerbound.o : simulator/lowerbound.cpp simulator/lowerbound.hpp simulator/multivect.hpp simulator/task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
approx_transfopack.o : simulator/approx_transfopack.cpp simulator/approx_transfopack.hpp simulator/multivect.hpp simulator/task.hpp simulator/approx_multipack.hpp simulator/transformation.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
approx_transfononpack.o : simulator/approx_transfononpack.cpp simulator/approx_transfononpack.hpp simulator/multivect.hpp simulator/task.hpp simulator/approx_nonpack.hpp simulator/transformation.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
approx_multipack.o : simulator/approx_multipack.cpp simulator/approx_multipack.hpp simulator/multivect.hpp simulator/task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
approx_nonpack.o : simulator/approx_nonpack.cpp simulator/approx_nonpack.hpp simulator/multivect.hpp simulator/task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
transformation.o : simulator/transformation.cpp simulator/multivect.hpp simulator/task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

profiles.o : simulator/profiles.cpp simulator/profiles.hpp simulator/multivect.hpp simulator/task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

tasks/ahmdalmax : tasks/ahmdalmax.cpp $(TASKLIB:=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(STD) -o $@ $< $(TASKLIB:=.o)

tasks/ahmdalsum : tasks/ahmdalsum.cpp $(TASKLIB:=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(STD) -o $@ $< $(TASKLIB:=.o)

tasks/alphamax : tasks/alphamax.cpp $(TASKLIB:=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(STD) -o $@ $< $(TASKLIB:=.o)

tasks/alphaprod : tasks/alphaprod.cpp $(TASKLIB:=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(STD) -o $@ $< $(TASKLIB:=.o)
	
cleanall : clean
	rm -f $(PRODNAME) $(TASKCREATORS)

clean :
	rm -f $(OBJECTS) $(PROG:=.o) $(TASKCREATORS:=.o)
