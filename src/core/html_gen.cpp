#include "html_gen.hpp"

#include <fstream>

#include "../html_builder/html_builder.hpp"
#include "../yoga/yoga.hpp"

#include "student.hpp"


void generate_userpage(const student& stud, const database& db, const std::string& filename);
htb::html_table<htb::plaintext, htb::plaintext, htb::plaintext> make_solution_table(
		const database& db, const solution& s);

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
	document.add(htb::main_heading_element{yoga::format("Results of %s (%s)", name, id)});
	document.add(htb::simple_paragraph{yoga::format(
			"%s has <b>%s</b> of %s possible points, which is equivalent to <b>%s%%</b>.",
			name, points, max_points, points > 0 ? (100.0*points)/max_points : 0)});
	auto partners = stud.partners();
	if(partners.empty()) {
		document.add(htb::simple_paragraph{name + " has no partner."});
	} else if (partners.size() == 1) {
		auto partner_id = partners.at(0);
		auto partner_name = db.get_student(partner_id).name();
		document.add(htb::simple_paragraph{yoga::format("%s has one partner: %s (%s).",
					name, partner_name, partner_id)});
	} else {
		document.add(htb::simple_paragraph{name + "has the following partners:"});
		htb::unordered_html_list partner_list;
		for(auto partner_id: partners) {
			auto partner_name = db.get_student(partner_id).name();
			partner_list.add(htb::plaintext{yoga::format("%s (%s)", partner_name,
						partner_id)});
		}
		document.add(std::move(partner_list));
	}
	document.add(htb::simple_paragraph{"The following achievments can be unlocked:"});
	htb::unordered_html_list achievments_listing;
	for (const auto& x: db.achievments()) {
		achievments_listing.add(htb::plaintext{yoga::format("At %s%%: %s", x.first, x.second)});
	}
	document.add(std::move(achievments_listing));
	document.add(htb::simple_paragraph{"The following assignments were edited:"});
	htb::unordered_html_list edited_assignments_list;
	for(auto id: stud.solutions()) {
		auto& solution = db.get_solution(id);
		auto points = solution.total_points();
		auto max_points = db.get_assignment(solution.assignment()).max_total_points();
		edited_assignments_list.add(htb::html_block_collection{
				htb::simple_paragraph{yoga::format(
					"Assignment %s: %s of %s possible points (%s%%)",
					solution.assignment(), points, max_points,
					(100.0 * points) / max_points)
				},
				make_solution_table(db, solution)
			});
	}
	document.add(std::move(edited_assignments_list));
	std::ofstream file{filename};
	document.write_to_stream(file);
}

htb::html_table<htb::plaintext, htb::plaintext, htb::plaintext> make_solution_table(
		const database& db, const solution& s) {
	htb::html_table<htb::plaintext, htb::plaintext, htb::plaintext> table{
		{{"task", "points", "comment"}}};
	const auto& a = db.get_assignment(s.assignment());
	for (const auto& x: s.editied_tasks()) {
		const auto& id = x.id();
		auto points = x.points();
		auto max_points = a.points_for(id);
		auto percentage = (points*100.0) / max_points;
		table.add_row(
				htb::plaintext{x.id()},
				htb::plaintext{yoga::format("<b>%s</b>/%s (%s%%)", points,
					max_points, percentage)},
				htb::plaintext{x.comment()});
	}
	return table;
}
