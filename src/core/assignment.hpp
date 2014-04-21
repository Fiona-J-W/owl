#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <map>
#include <iosfwd>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>

#include "id.hpp"
#include "task.hpp"

class assignment {
public:
	assignment() = default;
	assignment(std::istream& stream);
	
	assignment_id id() const {return m_id;}
	unsigned max_total_points() const;
	unsigned points_for(const std::string& id) const {return m_max_points.at(id);}
	const std::map<std::string, unsigned>& point_map() const {return m_max_points;}
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("max_points", m_max_points));
	}
	
private:
	assignment_id m_id;
	std::map<std::string, unsigned> m_max_points;
	
};

assignment create_assignment();

std::vector<task> read_tasks(const assignment& asst, const std::string& comment = "");

#endif
