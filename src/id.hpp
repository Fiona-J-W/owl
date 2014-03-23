#ifndef ID_HPP
#define ID_HPP

#include <cstdint>
#include <functional>
#include <limits>
#include <ostream>

#include <cereal/cereal.hpp>

template<class> class id {
public:
	id(): m_id{std::numeric_limits<std::uint32_t>::max()} {}
	explicit id(std::uint32_t id): m_id{id} {}
	
	friend bool operator==(id l, id r) {return l.m_id == r.m_id;}
	friend bool operator!=(id l, id r) {return l.m_id != r.m_id;}
	friend bool operator<(id l, id r)  {return l.m_id <  r.m_id;}
	friend bool operator>(id l, id r)  {return l.m_id >  r.m_id;}
	friend bool operator<=(id l, id r) {return l.m_id <= r.m_id;}
	friend bool operator>=(id l, id r) {return l.m_id >= r.m_id;}
	
	friend struct std::hash<id>;
	
	friend std::ostream& operator<<(std::ostream& stream, id val) {
		return stream << '#' << val.m_id;
	}
	
	template<class Archive>
	void serialize(Archive & archive) {
		archive(cereal::make_nvp("id", m_id));
	}
private:
	std::uint32_t m_id;
};

struct solution_id_tag{};
using solution_id = id<solution_id_tag>;

struct student_id_tag{};
using student_id = id<student_id_tag>;

struct assignment_id_tag{};
using assignment_id = id<assignment_id_tag>;

namespace std {

template<typename T>
struct hash<::id<T>> {
	std::size_t operator()(::id<T> value) const {
		return std::hash<std::uint32_t>{}(value.m_id);
	}
};

}



#endif
