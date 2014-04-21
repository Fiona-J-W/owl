#include "solution.hpp"

#include <sstream>

#include "database.hpp"
#include "student.hpp"
#include "text_reader.hpp"

solution::solution(assignment_id a_id, solution_id id, std::vector<student_id> solvers,
		std::vector<task> editied_tasks):
	m_assignment{a_id}, m_id{id}, m_solvers{std::move(solvers)},
	m_edited_tasks{std::move(editied_tasks)} {}


unsigned solution::total_points() const {
	unsigned points = 0;
	for(const auto& task: m_edited_tasks) {
		points += task.points();
	}
	return points;
}

solution_id add_solution(database& db, assignment_id assignment, std::vector<student_id> solvers,
		std::vector<task> editied_tasks) {
	auto id = db.new_solution_id();
	for(auto solver: solvers) {
		db.get_student(solver).add_solution(id);
	}
	auto tmp = solution{assignment, id, std::move(solvers), std::move(editied_tasks)};
	db.add_solution(std::move(tmp));
	return id;
}
