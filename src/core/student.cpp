#include "student.hpp"

#include <algorithm>

#include "database.hpp"

student::student(student_id id, std::string name, std::string pseudonym,
		std::vector<student_id> partners):
	m_id{id}, m_name{std::move(name)}, m_pseudonym{std::move(pseudonym)},
	m_partners{std::move(partners)} {}

unsigned student::current_points(const database& db) const {
	unsigned points = 0;
	for(auto solution_id: m_solutions) {
		points += db.get_solution(solution_id).total_points();
	}
	return points;
}

void student::add_solution(solution_id id) {
	auto it = std::find(m_solutions.begin(), m_solutions.end(), id);
	if (it != m_solutions.end()) {
		throw std::invalid_argument{
			"This solution is already markes as solved by this student"};
	}
	m_solutions.emplace_back(id);
}

void print_overview(const student& stud, std::ostream& stream) {
	stream
		<< "name    : " << stud.name() << '\n'
		<< "mat     : " << stud.id() << '\n'
		<< "overview: <base_url>" << stud.pseudonym() << ".html\n"
		<< "<further_notes>\n\n";
}
