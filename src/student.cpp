#include "student.hpp"

#include "database.hpp"

student::student(student_id id, std::string name, std::string pseudonym,
		std::vector<student_id> partners):
	m_id{id}, m_name{std::move(name)}, m_pseudonym{std::move(pseudonym)},
	m_partners{std::move(partners)} {}

unsigned student::current_points(const database& db) const {
	unsigned points = 0;
	for(auto solution_id: m_solutions) {
		points += db.get_solution(solution_id).total_points();
	}
	return points;
}
