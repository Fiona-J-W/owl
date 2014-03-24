#include "html_gen.hpp"

#include <fstream>

#include "../html_builder/html_builder.hpp"
#include "../yoga/yoga.hpp"

#include "student.hpp"


void generate_userpage(const student& stud, const database& db, const std::string& filename);

void generate_html(const database& db, const std::string& basedir) {
	auto students = db.get_student_list();
	for(const auto& student: students) {
		generate_userpage(*student, db, basedir + student->pseudonym());
	}
}

void generate_userpage(const student& stud, const database& db, const std::string& filename) {
	auto name       = stud.name();
	auto id         = stud.id();
	auto points     = stud.current_points(db);
	auto max_points = db.max_total_points();
	
	htb::html_document document{"Results of " + name};
	document.add(htb::main_heading_element{yoga::format("Results for %s (%s)", name, id)});
	document.add(htb::simple_paragraph{yoga::format(
			"%s has <b>%s</b> of %s possible points, which is equivalent to <b>%s%%</b>.",
			name, points, max_points, points > 0 ? (100.0*points)/max_points : 0)});
	document.add(htb::simple_paragraph{"The following assignments were edited:"});
	htb::unordered_html_list edited_assignments_list;
	for(auto id: stud.solutions()) {
		auto& solution = db.get_solution(id);
		auto points = solution.total_points();
		auto max_points = db.get_assignment(solution.assignment()).max_total_points();
		edited_assignments_list.add(htb::plaintext{yoga::format(
				"Assignment %s: %s of %s possible points (%s%%)",
				solution.assignment(), points, max_points,
				(100.0 * points) / max_points)});
	}
	document.add(std::move(edited_assignments_list));
	std::ofstream file{filename};
	document.write_to_stream(file);
}
