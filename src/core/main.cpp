#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include "../yoga/yoga.hpp"

#include "database.hpp"
#include "html_gen.hpp"

int main(int argc, char** argv) try {
	if(argc != 3) return 1;
	
	yoga::settings::set_priority(yoga::priority::trace);
	
	database db{argv[1]};
	
	//std::stringstream stream{"1:1\n2.1:5\n2.2:10\n"};
	//assignment asst{stream};
	//db::add_assignment(std::move(asst));
	//student stud{student_id{1234567}, "Max Mustermann", "MM"};
	//db::add_student(std::move(stud));
	//add_solution(db, assignment_id{1}, std::vector<student_id>{student_id{1234567}},
	//		std::vector<task>{task{"1", 1}, task{"2.1", 5,"great"}});
	
	generate_html(db, argv[2]);
	
	db.save(argv[1]);
} catch(std::runtime_error& e) {
	std::cerr << "Error: " << e.what() << '\n';
	return 1;
}
