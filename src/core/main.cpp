#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include "../yoga/yoga.hpp"

#include "database.hpp"
#include "html_gen.hpp"
#include "text_reader.hpp"

void require(bool b, const char* msg = "") {
	if(!b) {
		throw std::runtime_error{msg};
	}
}

int main(int argc, char** argv) try {
	if(argc < 3) return 1;
	
	
	//yoga::settings::set_priority(yoga::priority::trace);
	database db{argv[1]};
	
	if(argc >= 4) {
		std::string command{argv[3]};
		if (command == "add_solution") {
			throw std::runtime_error{"not yet implemented"};
			require(argc == 5);
			assignment_id asst_id{argv[4]};
			add_solution(
					db,
					asst_id,
					db.parse_students_string(argv[5]),
					//std::vector<task>{task{"1", 1}, task{"2.1", 5,"great"}}
					std::vector<task>{}
					);
			
		} else if (command == "add_assignment") {
			db.add_assignment(create_assignment());
			
		} else if (command =="add_student") {
			require(argc == 7);
			db.add_student(student{
					student_id{argv[4]},
					argv[5], argv[6]});
			
		} else if (command == "make_team") {
			require(argc == 5);
			db.make_team(db.parse_students_string(argv[4]));
		} else {
			throw std::invalid_argument{"invalid operation"};
		}
	}
	
	
	generate_html(db, argv[2]);
	db.save(argv[1]);
} catch(std::runtime_error& e) {
	std::cerr << "Error: " << e.what() << '\n';
	return 1;
}
