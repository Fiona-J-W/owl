#include "database.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>

namespace db {

namespace {
	[[noreturn]]
	void not_yet_implemented(const std::string& what) {
		std::cerr << "Error: " << what << " is not yet implemented!\n";
		std::terminate();
	}
}

static std::vector<student_id> student_ids;
static std::vector<solution_id> solution_ids;
static std::vector<assignment_id> assignment_ids;

static std::unordered_map<student_id, student> students;
static std::unordered_map<solution_id, solution> solutions;
static std::unordered_map<assignment_id, assignment> assignments;

void load(const std::string& filename) {
	std::ifstream file{filename};
	if(!file.is_open()) {
		throw std::runtime_error{filename + " could not be opened"};
	}
	cereal::JSONInputArchive archive{file};
	archive(
			CEREAL_NVP(student_ids), CEREAL_NVP(solution_ids), CEREAL_NVP(assignment_ids),
			CEREAL_NVP(students), CEREAL_NVP(solutions), CEREAL_NVP(assignments));
}
void save(const std::string& filename) {
	std::ofstream file{filename};
	if(!file.is_open()) {
		throw std::runtime_error{filename + " could not be opened"};
	}
	cereal::JSONOutputArchive archive{file};
	archive(
			CEREAL_NVP(student_ids), CEREAL_NVP(solution_ids), CEREAL_NVP(assignment_ids),
			CEREAL_NVP(students), CEREAL_NVP(solutions), CEREAL_NVP(assignments));
}

assignment& get_assignment(assignment_id) {
	not_yet_implemented("get_assignment");
}

student& get_student(student_id) {
	not_yet_implemented("get_student");
}

solution& get_solution(solution_id) {
	not_yet_implemented("get_solution");
}

}
