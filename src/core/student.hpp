#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <ostream>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

#include "../yoga/yoga.hpp"

#include "id.hpp"
#include "reference.hpp"
#include "solution.hpp"


class database;

class student {
public:
	student() = default;
	student(student_id id, std::string name, std::string pseudonym,
			std::vector<student_id> partners = std::vector<student_id>{});
	
	const std::string& name() const {return m_name;}
	const std::string& pseudonym() const {return m_pseudonym;}
	void set_pseudonym(std::string new_pseudonym) {m_pseudonym = std::move(new_pseudonym);}
	student_id id() const {return m_id;}
	unsigned current_points(const database& db) const;
	const std::vector<solution_id>& solutions() const {return m_solutions;}
	const std::vector<student_id>& partners() const {return m_partners;}
	
	void add_solution(solution_id id);
	void add_partner(student_id id) {m_partners.push_back(id);}
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("name", m_name),
				cereal::make_nvp("pseudonym", m_pseudonym),
				cereal::make_nvp("partners", m_partners),
				cereal::make_nvp("solutions", m_solutions));
	}
private:
	student_id m_id;
	std::string m_name;
	std::string m_pseudonym;
	std::vector<student_id> m_partners;
	std::vector<solution_id> m_solutions;
};

inline std::ostream& operator<<(std::ostream& stream, const student& s) {
	yoga::print_to_stream_formated(stream, "student(%s, %s)", s.id(), s.name());
	return stream;
}

#endif
