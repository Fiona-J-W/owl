#ifndef HTML_BUILDER_HPP
#define HTML_BUILDER_HPP

#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../util/util.hpp"

namespace htb {

class html_element_interface {
public:
	virtual ~html_element_interface() = default;
	virtual std::unique_ptr<html_element_interface> clone() const = 0;
	virtual void write_to_stream(std::ostream& stream, unsigned depth) const = 0;
};

template<typename T>
class html_element_impl final: public html_element_interface {
public:
	html_element_impl(T value): m_value{std::move(value)} {}
	
	std::unique_ptr<html_element_interface> clone() const {
		return make_unique<html_element_impl>(m_value);
	}
	
	void write_to_stream(std::ostream& stream, unsigned depth) const {
		m_value.write_to_stream(stream, depth);
	}
	
	T m_value;
};

template<typename T>
std::unique_ptr<html_element_interface> make_html_element(T value) {
	return make_unique<html_element_impl<T>>(std::move(value));
}

std::vector<std::unique_ptr<html_element_interface>> copy_html_elements_vec(
		const std::vector<std::unique_ptr<html_element_interface>>& vec) {
	std::vector<std::unique_ptr<html_element_interface>> return_vector;
	for(const auto& elem: vec) {
		return_vector.emplace_back(elem->clone());
	}
	return return_vector;
}

inline void print_indent(std::ostream& stream, unsigned n) {
	for (unsigned i = 0; i < n; ++i) {
		stream << '\t';
	}
}


template<typename T = const char*>
class block_tag_printer {
public:
	block_tag_printer(std::ostream& stream, T tagname, unsigned depth = 0):
			m_stream{stream}, m_tagname{std::move(tagname)}, m_depth{depth} {
		print_indent(m_stream, m_depth);
		m_stream << '<' << m_tagname << ">\n";
	}
	~block_tag_printer() {
		print_indent(m_stream, m_depth);
		m_stream << "</" << m_tagname << ">\n";
	}
private:
	std::ostream& m_stream;
	T m_tagname;
	unsigned m_depth;
};

template<typename T = const char*>
class line_tag_printer {
public:
	line_tag_printer(std::ostream& stream, T tagname, unsigned depth = 0):
			m_stream{stream}, m_tagname{std::move(tagname)}, m_depth{depth} {
		print_indent(m_stream, m_depth);
		m_stream << '<' << m_tagname << '>';
	}
	~line_tag_printer() {
		m_stream << "</" << m_tagname << ">\n";
	}
private:
	std::ostream& m_stream;
	T m_tagname;
	unsigned m_depth;
};

template<typename T = const char*>
class inline_tag_printer {
public:
	inline_tag_printer(std::ostream& stream, T tagname):
			m_stream{stream}, m_tagname{std::move(tagname)} {
		m_stream << '<' << m_tagname << '>';
	}
	~inline_tag_printer() {
		m_stream << "</" << m_tagname << '>';
	}
private:
	std::ostream& m_stream;
	T m_tagname;
};

class html_block_collection {
public:
	template<typename...T>
	explicit html_block_collection(T...args) {
		add(args...);
	}
	
	html_block_collection(html_block_collection&&) = default;
	html_block_collection(const html_block_collection& other): m_title{other.m_title} {
		m_elements = copy_html_elements_vec(other.m_elements);
	}
	
	html_block_collection& operator=(html_block_collection&&) = default;
	html_block_collection& operator=(const html_block_collection& other) {
		m_title = other.m_title;
		m_elements = copy_html_elements_vec(other.m_elements);
		return *this;
	}
	
	void write_to_stream(std::ostream& stream, unsigned depth) const {
		for(const auto& element: m_elements) {
			element->write_to_stream(stream, depth);
		}
	}
	
	template<typename Element, typename...Tail>
	void add(Element element, Tail...tail) {
		m_elements.emplace_back(make_unique<html_element_impl<Element>>(std::move(element)));
		add(std::move(tail)...);
	}
	void add() const {}
	
	template<typename Element>
	Element& get(std::size_t index) {
		return dynamic_cast<html_element_impl<Element>&>(m_elements.at(index)).m_value;
	}
	
	template<typename Element>
	const Element& get(std::size_t index) const {
		return dynamic_cast<html_element_impl<const Element>&>(m_elements.at(index)).m_value;
	}
private:
	std::string m_title;
	std::vector<std::unique_ptr<html_element_interface>> m_elements;
};

class html_document: public html_block_collection {
public:
	explicit html_document(std::string title): m_title{std::move(title)} {}
	
	html_document(html_document&&) = default;
	html_document(const html_document& other): html_block_collection{other}, 
		m_title{other.m_title} {}
	
	html_document& operator=(html_document&&) = default;
	html_document& operator=(const html_document& other) {
		html_block_collection::operator=(other);
		m_title = other.m_title;
		return *this;
	}
	
	using html_block_collection::write_to_stream;

	void write_to_stream(std::ostream& stream) const {
		stream << "<!DOCTYPE html>\n";
		block_tag_printer<> html{stream, "html", 0};
		{
			block_tag_printer<> head{stream, "head", 1};
			line_tag_printer<> title{stream, "title", 2};
			stream << m_title;
		}
		block_tag_printer<> body{stream, "body", 1};
		write_to_stream(stream, 2);
	}
	
private:
	std::string m_title;
};

template<std::size_t Index, std::size_t MaxIndex, typename...Elements>
struct tuple_printer{
	static void print(std::ostream& stream, const std::tuple<Elements...>& tuple,
			const std::string& tagname, unsigned depth) {
		{
			block_tag_printer<std::string> tag{stream, tagname, depth};
			std::get<Index>(tuple).write_to_stream(stream, depth + 1);
		}
		tuple_printer<Index + 1, MaxIndex, Elements...>::print(stream, tuple, tagname, depth);
	}
};

template<std::size_t MaxIndex, typename...Elements>
struct tuple_printer<MaxIndex, MaxIndex, Elements...>{
	static void print(std::ostream&, const std::tuple<Elements...>&,const std::string&, unsigned) {}
};


template<typename...Columns>
class html_table {
	public:
	static const std::size_t column_count = sizeof...(Columns);
	
	html_table(const std::array<std::string, column_count>& headings):
		m_headings(headings) {}
	
	void add_row(Columns...data) {
		m_rows.emplace_back(std::move(data)...);
	}
	
	void write_to_stream(std::ostream& stream, unsigned depth) const {
		print_indent(stream, depth);
		stream << "<table border=\"1\">\n";
		{
			block_tag_printer<> thead_tag{stream, "thead", depth + 1};
			block_tag_printer<> tr_tag{stream, "tr", depth + 2};
			for(const auto& heading: m_headings) {
				line_tag_printer<> th_tag{stream, "th", depth + 3};
				stream << heading;
			}
		}
		{
			block_tag_printer<> tbody_tag{stream, "tbody", depth + 1};
			for(const auto& row: m_rows) {
				block_tag_printer<> tr_tag{stream, "tr", depth + 2};
				tuple_printer<0, column_count, Columns...>::print(
						stream, row, "td", depth + 3);
				//print_row<0>(stream, row, depth + 3);
			}
		}
		print_indent(stream, depth);
		stream << "</table>\n";
	}
private:
	std::array<std::string, column_count> m_headings;
	std::vector<std::tuple<Columns...>> m_rows;
};


template<typename ListType>
class basic_html_list {
public:
	basic_html_list(basic_html_list&&) = default;
	basic_html_list(const basic_html_list& other):
		m_elements{copy_html_elements_vec(other.m_elements)} {}
	
	basic_html_list& operator=(basic_html_list&&) = default;
	basic_html_list& operator=(const basic_html_list& other) {
		m_elements = copy_html_elements_vec(other.m_elements);
	}
	
	template<typename...T>
	basic_html_list(T...elements) {
		add(std::move(elements)...);
	}
	
	template<typename...T>
	void add(T...elements) {
		add_impl(std::move(elements)...);
	}
	
	void write_to_stream(std::ostream& stream, unsigned depth) const {
		block_tag_printer<> list_tags{stream, ListType::name(), depth};
		for (const auto& element: m_elements) {
			block_tag_printer<> inner_list_tags{stream, ListType::element_name(), depth+1};
			element->write_to_stream(stream, depth+2);
		}
		
	}
private:
	template<typename Head, typename...Tail>
	void add_impl(Head head, Tail...elements) {
		m_elements.emplace_back(make_html_element(std::move(head)));
		add_impl(std::move(elements)...);
	}
	void add_impl(){}
	std::vector<std::unique_ptr<html_element_interface>> m_elements;
};

struct unordered_html_list_helper {
	static constexpr const char * name() {return "ul";}
	static constexpr const char * element_name() {return "li";}
};
using unordered_html_list = basic_html_list<unordered_html_list_helper>;


template<typename T>
class html_line_element {
public:
	html_line_element(std::string content): m_content{std::move(content)} {}
	
	void write_to_stream(std::ostream& stream, unsigned depth) const {
		line_tag_printer<decltype(T::name())> tags{stream, T::name(), depth};
		stream << m_content;
	}
private:
	std::string m_content;
};

struct main_heading_element_helper {static constexpr const char * name() {return "h1";} };
using main_heading_element = html_line_element<main_heading_element_helper>;

struct simple_paragraph_helper {static constexpr char name() {return 'p';} };
using simple_paragraph = html_line_element<simple_paragraph_helper>;

class plaintext {
public:
	plaintext(std::string text): m_text{std::move(text)} {}
	void write_to_stream(std::ostream& stream, unsigned depth) const {
		print_indent(stream, depth);
		stream << m_text << '\n';
	}
private:
	std::string m_text;
};

} //namespace htb

#endif
