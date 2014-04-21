# Makefile for bin/owl
# created with makefile-creator


####################
#Settings:


CXX ?= g++
FLAGS +=  -Wall -Wextra -pedantic -std=c++11 
LIBS += -lboost_filesystem -lboost_system 
INCLUDES += 
TARGET = bin/owl
OBJECTS = \
	build/assignment.o \
	build/database.o \
	build/html_gen.o \
	build/main.o \
	build/solution.o \
	build/student.o \
	build/task.o \
	build/text_reader.o \
	build/print.o \



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


build/assignment.o: src/core/assignment.cpp src/core/assignment.hpp src/core/id.hpp src/core/text_reader.hpp makefile

build/database.o: src/core/database.cpp src/core/assignment.hpp src/core/database.hpp src/core/id.hpp src/core/reference.hpp src/core/solution.hpp src/core/student.hpp src/core/task.hpp src/yoga/format.hpp src/yoga/macros.hpp src/yoga/print.hpp src/yoga/util.hpp src/yoga/yoga.hpp makefile

build/html_gen.o: src/core/html_gen.cpp src/core/assignment.hpp src/core/database.hpp src/core/html_gen.hpp src/core/id.hpp src/core/reference.hpp src/core/solution.hpp src/core/student.hpp src/core/task.hpp src/html_builder/html_builder.hpp src/util/util.hpp src/yoga/format.hpp src/yoga/macros.hpp src/yoga/print.hpp src/yoga/util.hpp src/yoga/yoga.hpp makefile

build/main.o: src/core/main.cpp src/core/assignment.hpp src/core/database.hpp src/core/html_gen.hpp src/core/id.hpp src/core/reference.hpp src/core/solution.hpp src/core/student.hpp src/core/task.hpp src/core/text_reader.hpp src/yoga/format.hpp src/yoga/macros.hpp src/yoga/print.hpp src/yoga/util.hpp src/yoga/yoga.hpp makefile

build/solution.o: src/core/solution.cpp src/core/assignment.hpp src/core/database.hpp src/core/id.hpp src/core/reference.hpp src/core/solution.hpp src/core/student.hpp src/core/task.hpp src/core/text_reader.hpp src/yoga/format.hpp src/yoga/macros.hpp src/yoga/print.hpp src/yoga/util.hpp src/yoga/yoga.hpp makefile

build/student.o: src/core/student.cpp src/core/assignment.hpp src/core/database.hpp src/core/id.hpp src/core/reference.hpp src/core/solution.hpp src/core/student.hpp src/core/task.hpp src/yoga/format.hpp src/yoga/macros.hpp src/yoga/print.hpp src/yoga/util.hpp src/yoga/yoga.hpp makefile

build/task.o: src/core/task.cpp src/core/task.hpp makefile

build/text_reader.o: src/core/text_reader.cpp src/core/text_reader.hpp makefile

build/print.o: src/yoga/print.cpp src/yoga/format.hpp src/yoga/print.hpp src/yoga/util.hpp makefile

