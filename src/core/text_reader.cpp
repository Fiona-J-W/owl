#include "text_reader.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include <boost/filesystem.hpp>

std::string read_text(const std::string& prompt) {
	auto filename = boost::filesystem::unique_path();
	{
		std::ofstream file{filename.string()};
		if(!file.is_open()) {
			throw std::runtime_error{"cannot open tempfile for writing"};
		}
		file << prompt;
	}
	{
		auto editor_ptr = std::getenv("EDITOR");
		std::string editor;
		if (editor_ptr) {
			editor = editor_ptr;
		} else {
			editor = "vim";
		}
		std::system((editor + " " + filename.string()).c_str());
	}
	std::string returnstring;
	{
		std::ifstream file{filename.string()};
		if(!file.is_open()) {
			boost::filesystem::remove(filename);
			throw std::runtime_error{"cannot open tempfile for reading"};
		}
		returnstring = std::string{std::istreambuf_iterator<char>{file}, {}};
	}
	boost::filesystem::remove(filename);
	return returnstring;
}
