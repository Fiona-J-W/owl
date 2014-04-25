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

void print_usage() {
	std::cout << "Usage: owl <database> <output-directory> <command> <command-options>\n"
		"Where command is one of the following:\n"
		"\tadd_solution <assignment-id> <students>\n"
		"\tadd_assignment\n"
		"\tadd_student <student-id> <name> <pseudonym>\n"
		"\tmake_team <students>\n"
		"\tscrample_pseudonyms\n"
		"\tprint_students\n";
}

int main(int argc, char** argv) try {
	if(argc < 3) {
		print_usage();
		return 1;
	}
	
	
	//yoga::settings::set_priority(yoga::priority::trace);
	database db{argv[1]};
	
	if(argc >= 4) {
		std::string command{argv[3]};
		if (command == "add_solution") {
			require(argc == 6, "invalid number of arguments");
			assignment_id asst_id{argv[4]};
			add_solution(
					db,
					asst_id,
					db.parse_students_string(argv[5]),
					read_tasks(db.get_assignment(asst_id)));
			
		} else if (command == "add_assignment") {
			db.add_assignment(create_assignment());
			
		} else if (command =="add_student") {
			require(argc == 7, "invalid number of arguments");
			db.add_student(student{
					student_id{argv[4]},
					argv[5], argv[6]});
			
		} else if (command == "make_team") {
			require(argc == 5, "invalid number of arguments");
			db.make_team(db.parse_students_string(argv[4]));
		} else if (command == "scrample_pseudonyms") {
			db.scrample_pseudonyms();
		} else if (command == "print_students") {
			for(const auto& stud: db.get_student_list()) {
				std::cout << "\n\n===========================================\n\n";
				print_overview(*stud);
			}
			std::cout << "\n\n===========================================\n\n";
		} else {
			std::cerr << "Error: invalid operation\n";
			return 1;
		}
	}
	
	
	generate_html(db, argv[2]);
	db.save(argv[1]);
} catch(std::runtime_error& e) {
	std::cerr << "Error: " << e.what() << '\n';
	return 1;
}
