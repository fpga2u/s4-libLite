#pragma once

#include <string>
#include <vector>

namespace S4{

void string_replace(const std::string& oldValue, const std::string& newValue,
	std::string& str);

std::string string_replace(const std::string& oldValue, const std::string& newValue,
	const std::string& str);

std::string string_toLower(const std::string& value);
std::string string_toUpper(const std::string& value);

//drop /t/n/r at head and tail of string
std::string string_strip(const std::string& value);

std::string string_strip_atTail(const std::string& value, const char * dropChars);

//split src to vector of strings by splitor
std::vector<std::string> string_split(const std::string& src, const std::string& splitor);

bool write_string_to_file(const std::string & file_name, const std::string & str, bool newFile);

//1000.0 -> 1,000.0
//WARNING: do not check if num contains non-digit characters.
std::string string_format_number_comma(const std::string& num);

}
