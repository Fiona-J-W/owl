#include "database.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <cereal/cereal.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include "../yoga/yoga.hpp"


void database::load(const std::string& filename) {
	std::ifstream file{filename};
	if(!file.is_open()) {
		throw std::runtime_error{filename + " could not be opened"};
	}
	cereal::JSONInputArchive archive{file};
	archive(cereal::make_nvp("students", m_students), cereal::make_nvp("solutions", m_solutions), 
			cereal::make_nvp("assignments", m_assignments),
			cereal::make_nvp("highest_solution_id", m_highest_solution_id),
			cereal::make_nvp("achievements", m_achievements));
}
void database::save(const std::string& filename) {
	std::ofstream file{filename};
	if(!file.is_open()) {
		throw std::runtime_error{filename + " could not be opened"};
	}
	cereal::JSONOutputArchive archive{file};
	archive(cereal::make_nvp("students", m_students), cereal::make_nvp("solutions", m_solutions), 
			cereal::make_nvp("assignments", m_assignments),
			cereal::make_nvp("highest_solution_id", m_highest_solution_id),
			cereal::make_nvp("achievements", m_achievements));
}

assignment& database::get_assignment(assignment_id id) {
	return m_assignments.at(id);
}

student& database::get_student(student_id id) {
	return m_students.at(id);
}

solution& database::get_solution(solution_id id) {
	return m_solutions.at(id);
}

const assignment& database::get_assignment(assignment_id id) const {
	return m_assignments.at(id);
}

const student& database::get_student(student_id id) const {
	return m_students.at(id);
}

const solution& database::get_solution(solution_id id) const {
	return m_solutions.at(id);
}

void database::add_assignment(assignment new_assignment) {
	if (m_assignments.count(new_assignment.id())) {
		throw std::invalid_argument{"there is already an assignment with this id"};
	}
	m_assignments.insert(std::make_pair(new_assignment.id(), std::move(new_assignment)));
}

void database::add_student(student new_student) {
	if (m_students.count(new_student.id())) {
		throw std::invalid_argument{"there is already a student with this id"};
	}
	m_students.insert(std::make_pair(new_student.id(), std::move(new_student)));
}

void database::add_solution(solution new_solution) {
	if (m_solutions.count(new_solution.id())) {
		throw std::invalid_argument{"there is already a solution with this id"};
	}
	m_solutions.insert(std::make_pair(new_solution.id(), std::move(new_solution)));
}

std::vector<reference<const student>> database::get_student_list() const {
	std::vector<reference<const student>> returnlist;
	for(const auto& student: m_students) {
		returnlist.emplace_back(student.second);
	}
	return returnlist;
}

std::vector<reference<student>> database::get_student_list() {
	std::vector<reference<student>> returnlist;
	for(auto& student: m_students) {
		returnlist.emplace_back(student.second);
	}
	return returnlist;
}

unsigned database::max_total_points() const {
	unsigned points = 0;
	for(const auto& x: m_assignments) {
		points += x.second.max_total_points();
	}
	return points;
}

solution_id database::new_solution_id() {
	return solution_id{++m_highest_solution_id};
}

std::vector<student_id> database::parse_students_string(const std::string& str) const {
	std::istringstream stream{str};
	std::vector<student_id> return_vec;
	std::string line;
	student_id last_id{0};
	while(std::getline(stream, line, ',')) {
		if(line.empty()) {
			continue;
		} else if (line == "..." || line == "…") {
			if(last_id == student_id{0}) {
				throw std::runtime_error{"elipses must directly follow a valid student-id"};
			}
			const auto& partners = get_student(last_id).partners();
			std::copy(partners.begin(), partners.end(), std::back_inserter(return_vec));
			last_id = student_id{0};
		} else {
			student_id id{line};
			return_vec.push_back(id);
			last_id = id;
		}
	}
	std::sort(return_vec.begin(), return_vec.end());
	return_vec.erase(std::unique(return_vec.begin(), return_vec.end()), return_vec.end());
	return return_vec;
}


void database::make_team(const std::vector<student_id>& students) {
	for(std::size_t i = 0; i < students.size(); ++i) {
		for(auto j = i; j < students.size(); ++j) {
			get_student(students[i]).add_partner(students[j]);
			get_student(students[j]).add_partner(students[i]);
		}
	}
}
