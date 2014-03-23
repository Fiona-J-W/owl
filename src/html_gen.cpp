#include "html_gen.hpp"

#include <fstream>

#include "student.hpp"


void generate_userpage(const student& stud, const database& db, const std::string& filename);

void generate_html(const database& db, const std::string& basedir) {
	auto students = db.get_student_list();
	for(const auto& student: students) {
		generate_userpage(*student, db, basedir + student->pseudonym());
	}
}

void generate_userpage(const student& stud, const database& db, const std::string& filename) {
	std::ofstream file{filename};
	auto name = stud.name();
	file << "<!DOCTYPE html>\n<html>\n<head>\n<title>Results of " << name << " ("
		<< stud.id() << ")</title>\n</head>\n<body>\n";
	file << "<h1>Results for " << name << "</h1>\n";
	file << "<p>" << name << " has <b>" << stud.current_points(db) << "</b>/"
		<< db.max_total_points() << " points.</p>";
	file << "</body>\n</html>";
}
