#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <unordered_map>
#include <iosfwd>

#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>

#include "id.hpp"

class assignment {
public:
	assignment() = default;
	assignment(std::istream& stream);
	
	assignment_id id() const {return m_id;}
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("max_points", m_max_points));
	}
	
private:
	assignment_id m_id;
	std::unordered_map<std::string, unsigned> m_max_points;
};

#endif
