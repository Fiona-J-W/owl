#include <iostream>
#include <fstream>

#include "database.hpp"

int main(int argc, char** argv) {
	if(argc != 2) return 1;
	
	db::load(argv[1]);
	db::save(argv[1]);
}
