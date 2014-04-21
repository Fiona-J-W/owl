#include "assignment.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <tuple>

#include <boost/algorithm/string/trim.hpp>

#include "text_reader.hpp"
#include "task.hpp"

static const std::string id_string= "ID =";

std::tuple<bool, std::string> next_line(std::istream& stream);

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

std::vector<task> read_tasks(const assignment& asst, const std::string& comment) {
	namespace balg = boost::algorithm;
	std::vector<task> tasks;
	std::ostringstream ostream;
	ostream << comment << '\n';
	
	for(const auto& x : asst.point_map()) {
		ostream << x.first << ":     ;; #max = " << x.second << '\n';
	}
	std::istringstream istream{read_text(ostream.str())};
	bool valid_line;
	std::string line;
	auto enforce = [&](bool b) {
		if(!b) {
			throw std::runtime_error{"invlid line in task-configuration: " + line};
		}
	};
	while(true) {
		std::tie(valid_line, line) = next_line(istream);
		if(!valid_line) {
			break;
		}
		std::istringstream line_stream{line};
		std::string task_id;
		enforce(std::getline(line_stream, task_id, ':'));
		balg::trim_right(task_id);
		
		std::string points_str;
		enforce(std::getline(line_stream, points_str, ';'));
		balg::trim(points_str);
		auto points = std::stoul(points_str);
		
		std::string comment;
		enforce(std::getline(line_stream, comment, ';'));
		balg::trim(comment);
		
		tasks.emplace_back(task_id, points, comment);
	}
	return tasks;
}


std::tuple<bool, std::string> next_line(std::istream& stream) {
	namespace balg = boost::algorithm;
	std::string line;
	while(std::getline(stream, line)) {
		balg::trim(line);
		if(line.empty() || line.front() == '#') {
			continue;
		}
		return std::make_tuple(true, std::move(line));
	}
	return std::make_tuple(false, "");
}
