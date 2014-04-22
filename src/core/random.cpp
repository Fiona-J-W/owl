#include "random.hpp"

#include <array>
#include <random>

std::string random_pseudonym() {
	std::array<char, 62> identifiers =
	{{'a','b','c','d','e','f','g','h','i','j','k','l','m',
	  'n','o','p','q','r','s','t','u','v','w','x','y','z',
	  'A','B','C','D','E','F','G','H','I','J','K','L','M',
	  'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	  '0','1','2','3','4','5','6','7','8','9'}};
	std::uniform_int_distribution<unsigned short> dist{0, identifiers.size() - 1};
	std::random_device gen{};
	std::string returnstring;
	for(unsigned i=0; i < 10; ++i) {
		returnstring.push_back(identifiers[dist(gen)]);
	}
	return returnstring;
}

