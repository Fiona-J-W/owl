#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include "database.hpp"

int main(int argc, char** argv) try {
	if(argc != 2) return 1;
	
	database db{argv[1]};
	
	//std::stringstream stream{"1:1\n2.1:5\n2.2:10\n"};
	//assignment asst{stream};
	//db::add_assignment(std::move(asst));
	//student stud{student_id{1234567}, "Max Mustermann", "MM"};
	//db::add_student(std::move(stud));
	
	db.save(argv[1]);
} catch(std::runtime_error& e) {
	std::cerr << "Error: " << e.what() << '\n';
	return 1;
}
