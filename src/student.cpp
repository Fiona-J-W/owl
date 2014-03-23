#include "student.hpp"

#include "database.hpp"

unsigned student::current_points() const {
	unsigned points = 0;
	for(auto solution_id: m_solutions) {
		points += db::get_solution(solution_id).total_points();
	}
	return points;
}
