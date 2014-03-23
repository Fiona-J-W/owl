#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <map>
#include <iosfwd>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>

#include "id.hpp"

class assignment {
public:
	assignment() = default;
	assignment(std::istream& stream);
	
	assignment_id id() const {return m_id;}
	unsigned max_total_points() const {return m_max_total_points;}
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("max_points", m_max_points));
	}
	
private:
	assignment_id m_id;
	std::map<std::string, unsigned> m_max_points;
	unsigned m_max_total_points = 0;
	
	void calculate_max_total_points();
};

#endif
