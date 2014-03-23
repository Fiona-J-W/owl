#include "solution.hpp"

unsigned solution::total_points() const {
	unsigned points = 0;
	for(const auto& task: m_edited_tasks) {
		points += task.points();
	}
	return points;
}
