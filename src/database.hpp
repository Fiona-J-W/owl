#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "assignment.hpp"
#include "id.hpp"
#include "solution.hpp"
#include "student.hpp"

namespace db {

void load(const std::string& filename);
void save(const std::string& filename);

assignment& get_assignment(assignment_id);

student& get_student(student_id);

solution& get_solution(solution_id);

}

#endif
