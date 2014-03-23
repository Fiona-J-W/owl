#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <iosfwd>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

#include "solution.hpp"
#include "id.hpp"


class student {
public:
	
	const std::string& name() const {return m_name;}
	student_id id() const {return m_id;}
	unsigned current_points() const;

	template<class Archive>
	void serialize(Archive & archive) {
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("name", m_name),
				cereal::make_nvp("pseudonym", m_pseudonym),
				cereal::make_nvp("solutions", m_solutions));
	}
private:
	student_id m_id;
	std::string m_name;
	std::string m_pseudonym;
	std::vector<student_id> m_partners;
	std::vector<solution_id> m_solutions;
};

#endif