
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

#include "common/s4string.h"
#include "common/s4logger.h"
#include <iostream>
#include <fstream>

#include <algorithm>

#if defined(_MSC_VER)
#pragma warning( pop )
#endif


namespace S4{

void string_replace(const std::string& oldValue, const std::string& newValue,
	std::string& str)
{
	for (std::string::size_type pos = str.find(oldValue);
		pos != std::string::npos;
		pos = str.find(oldValue, pos))
	{
		str.replace(pos, oldValue.size(), newValue);
		pos += newValue.size();
	}
}

std::string string_replace(const std::string& oldValue, const std::string& newValue,
	const std::string& str)
{
    std::string rslt(str);
    string_replace(oldValue, newValue, rslt);
    return (rslt);
}

std::string string_toUpper(const std::string& value)
{
	std::string copy(value);

	std::transform(copy.begin(), copy.end(), copy.begin(), toupper);

	return copy;
}

std::string string_toLower(const std::string& value)
{
	std::string copy(value);

	std::transform(copy.begin(), copy.end(), copy.begin(), tolower);

	return copy;
}

std::string string_strip(const std::string& value)
{
	if (!value.size())
		return value;

	size_t startPos = value.find_first_not_of(" \t\r\n");
	size_t endPos = value.find_last_not_of(" \t\r\n");

	if (startPos == std::string::npos)
		return value;

	return std::string(value, startPos, endPos - startPos + 1);
}


std::string string_strip_atTail(const std::string& value, const char * dropChars)
{
	if (!value.size())
		return value;

	size_t endPos = value.find_last_not_of(dropChars);

	if (endPos == std::string::npos)
		return value;

	return std::string(value, 0, endPos + 1);
}

std::vector<std::string> string_split(const std::string& src, const std::string& splitor)
{
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
    pos2 = src.find(splitor);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(src.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + splitor.size();
        pos2 = src.find(splitor, pos1);
    }
    if(pos1 != src.length()){
        v.push_back(src.substr(pos1));
    }

	return (v);
}



bool write_string_to_file(const std::string & file_name, const std::string & str, bool newFile) 
{ 
	try {
		std::ofstream	OsWrite(file_name, newFile? (std::ofstream::out):(std::ofstream::app));
		OsWrite << str;
		OsWrite << std::endl;
		OsWrite.close();
		return true;
	}
	catch (std::exception& e) {
		ERR("write string to file({}) fail:{}", file_name, e.what());
		throw e;
	}
}



#define DIGIT_DOT '.'
#define DIGIT_COMMA ','
#define DIGIT_MAX 50
std::string string_format_number_comma(const std::string& num)
{
    if (num.size() < 3) return num;

    char commas[DIGIT_MAX];                    // Where the result is
    char *dot = strchr((char*)num.c_str(), DIGIT_DOT); // do we have a '.'?
    char *src, *dst;                     // source, dest
    size_t dot_len = 0;
    if (dot)
    {                                         // Yes
        dot_len = strlen(dot);
        dst = commas + DIGIT_MAX - dot_len - 1; // set dest to allow the fractional part to fit
        strcpy(dst, dot);                     // copy that part
        // *dot = 0;                             // 'cut' that frac part in tmp
        src = --dot;                          // point to last non frac char in tmp
        dst--;                                // point to previous 'free' char in dest
    }
    else
    {                                              // No
        src = (char*)num.data() + num.size() - 1; // src is last char of our float string
        dst = commas + DIGIT_MAX - 1;                    // dst is last char of commas
        *dst-- = '\0';
    }

    size_t len = num.size() - dot_len;      // len is the mantissa size
    int cnt = 0;                  // char counter

    while (len--!=0)
    {
        if (*src <= '9' && *src >= '0')
        { // add comma is we added 3 digits already
            if (cnt && !(cnt % 3))
                *dst-- = DIGIT_COMMA;
            cnt++; // mantissa digit count increment
        }
        *dst-- = *src--;
    }
    dst++;
    return std::string(dst);
}

}//namespace S4


