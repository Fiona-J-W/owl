#include "assignment.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include <boost/algorithm/string/trim.hpp>

#include "text_reader.hpp"

static const std::string id_string= "ID =";

assignment::assignment(std::istream& stream) {
	namespace balg = boost::algorithm;
	bool found_id = false;
	for(std::string line; std::getline(stream, line);) {
		balg::trim(line);
		if(line.empty() || line[0] == '#') {
			continue;
		}
		if (!found_id) {
			if(line.find(id_string) != 0) {
				throw std::runtime_error{"The first physical line of an "
					"assignment-description must be it's ID!"};
			} else {
				m_id = assignment_id{static_cast<uint32_t>(std::stoul(line.substr(
						id_string.size())))};
				found_id = true;
			}
			continue;
		}
		auto split_point = line.find(':');
		if(split_point == std::string::npos) {
			throw std::runtime_error{"invalid line in configuration (no ':' found): “"
				+ line + "”"};
		}
		auto id = line.substr(0, split_point);
		balg::trim_right(id);
		auto points = std::stoul(line.substr(split_point+1));
		m_max_points.insert(std::make_pair(std::move(id), points));
	}
	if(!found_id) {
		throw std::runtime_error{"Empty assignment-configuration"};
	}
}

unsigned assignment::max_total_points() const {
	unsigned tmp = 0;
	for(const auto& x: m_max_points) {
		tmp += x.second;
	}
	return tmp;
}

assignment create_assignment() {
	std::istringstream stream{read_text("#\n# Plese enter the tasks in the form\n"
			"#taskname: max_points\n#\n" + id_string + "\n\n\n")};
	return assignment{stream};
}
