#include "assignment.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include <boost/algorithm/string/trim.hpp>

assignment::assignment(std::istream& stream) {
	namespace balg = boost::algorithm;
	for(std::string line; std::getline(stream, line);) {
		balg::trim(line);
		if(line.empty() || line[0] == '#') {
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
	calculate_max_total_points();
}

void assignment::calculate_max_total_points() {
	m_max_total_points = 0;
	for(const auto& x: m_max_points) {
		m_max_total_points += x.second;
	}
}
