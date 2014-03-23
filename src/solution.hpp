#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <string>
#include <vector>

#include "id.hpp"
#include "task.hpp"

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

class solution {
public:
	
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
	solution_id m_id;
	assignment_id m_assignment;
	std::vector<student_id> m_solvers;
	std::vector<task> m_edited_tasks;
};

#endif
