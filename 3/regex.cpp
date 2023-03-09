#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>

int main()
{
	std::ifstream out("input.txt");
	std::stringstream buffer;
	buffer << out.rdbuf();
	std::string input = buffer.str(); // file -> string bruh

	std::smatch match;
	std::regex email_regex("[A-Za-z0-9_.-]+[@]{1}[A-Za-z0-9]+[.]{1}[A-Za-z]{1,5}");
	while (std::regex_search(input, match, email_regex)) {
		std::cout << match.str() << std::endl;
		input = match.suffix();
	}

}