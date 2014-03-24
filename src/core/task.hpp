#ifndef TASK_HPP
#define TASK_HPP

#include <limits>
#include <string>
#include <utility>

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

class task {
public:
	task(): m_points{std::numeric_limits<unsigned>::max()} {}
	
	task(std::string id, unsigned points, std::string comment = ""):
		m_id{std::move(id)}, m_points{points}, m_comment{std::move(comment)} {}
	
	const std::string& id() const {return m_id;}
	const std::string& comment() const {return m_comment;}
	bool has_comment() const {return !m_comment.empty();}
	unsigned points() const {return m_points;}
	
	void set_points(unsigned points) {m_points = points;}
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(m_id, m_comment, m_points);
		archive(cereal::make_nvp("id", m_id), cereal::make_nvp("max_comment", m_comment),
				cereal::make_nvp("points", m_points));
	}
private:
	std::string m_id;
	unsigned m_points;
	std::string m_comment;
};

#endif
