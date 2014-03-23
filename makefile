# Makefile for bin/gradegen
# created with makefile-creator


####################
#Settings:


CXX ?= g++
FLAGS +=  -Wall -Wextra -pedantic -std=c++11 -O0 -D_GLIBCXX_DEBUG -g 
LIBS += 
INCLUDES += 
TARGET = bin/gradegen
OBJECTS = \
	build/assignment.o \
	build/database.o \
	build/main.o \
	build/solution.o \
	build/student.o \
	build/task.o \



####################
#Rules:


$(TARGET) : $(OBJECTS)
	$(CXX) $(FLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

build/%.o:
	@if test ! -d 'build'; then mkdir 'build'; echo "created 'build'" ; fi
	$(CXX) $(FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm build/*.o

all: $(TARGET)


####################
#Dependencies:


build/assignment.o: src/assignment.cpp src/assignment.hpp src/id.hpp makefile

build/database.o: src/database.cpp src/assignment.hpp src/database.hpp src/id.hpp src/reference.hpp src/solution.hpp src/student.hpp src/task.hpp makefile

build/main.o: src/main.cpp src/assignment.hpp src/database.hpp src/id.hpp src/reference.hpp src/solution.hpp src/student.hpp src/task.hpp makefile

build/solution.o: src/solution.cpp src/id.hpp src/solution.hpp src/task.hpp makefile

build/student.o: src/student.cpp src/assignment.hpp src/database.hpp src/id.hpp src/reference.hpp src/solution.hpp src/student.hpp src/task.hpp makefile

build/task.o: src/task.cpp src/task.hpp makefile

