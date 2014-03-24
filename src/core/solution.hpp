#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <string>
#include <vector>

#include "id.hpp"
#include "task.hpp"

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

class database;

class solution {
public:
	solution() = default;
	solution(assignment_id a_id, solution_id id, std::vector<student_id> solvers,
			std::vector<task> editied_tasks);
	
	assignment_id assignment() const {return m_assignment;}
	solution_id id() const {return m_id;}
	unsigned total_points() const;
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(m_id, m_assignment, m_solvers, m_edited_tasks);
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("assignment", m_assignment),
				cereal::make_nvp("solvers", m_solvers),
				cereal::make_nvp("edited_tasks", m_edited_tasks));
	}
private:
	assignment_id m_assignment;
	solution_id m_id;
	std::vector<student_id> m_solvers;
	std::vector<task> m_edited_tasks;
};

solution_id add_solution(database& db, assignment_id assignment, std::vector<student_id> solvers,
		std::vector<task> editied_tasks);

#endif
