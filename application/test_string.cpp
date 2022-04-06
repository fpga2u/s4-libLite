#include "common/s4string.h"
#include <iostream>

using namespace S4;
int main(int argc, char** argv)
{
    std::cout << string_format_number_comma("0.31415") << std::endl;
    std::cout << string_format_number_comma("3.1415") << std::endl;
    std::cout << string_format_number_comma("31.415") << std::endl;
    std::cout << string_format_number_comma("314.15") << std::endl;
    std::cout << string_format_number_comma("3141.5") << std::endl;
    std::cout << string_format_number_comma("31415") << std::endl;
    std::cout << string_format_number_comma("-0.31415") << std::endl;
    std::cout << string_format_number_comma("-3.1415") << std::endl;
    std::cout << string_format_number_comma("-31.415") << std::endl;
    std::cout << string_format_number_comma("-314.15") << std::endl;
    std::cout << string_format_number_comma("-3141.5") << std::endl;
    std::cout << string_format_number_comma("-31415") << std::endl;
    std::cout << string_format_number_comma("0") << std::endl;
    std::cout << string_format_number_comma("1234567890") << std::endl;
    std::cout << string_format_number_comma("123456789.0") << std::endl;
    std::cout << string_format_number_comma("12345678.90") << std::endl;
    std::cout << string_format_number_comma("1234567.890") << std::endl;
    std::cout << string_format_number_comma("123456.7890") << std::endl;
    std::cout << string_format_number_comma("12345.67890") << std::endl;
    std::cout << string_format_number_comma("1234.567890") << std::endl;
    std::cout << string_format_number_comma("123.4567890") << std::endl;
    std::cout << string_format_number_comma("12.34567890") << std::endl;
    std::cout << string_format_number_comma("1.234567890") << std::endl;
    std::cout << string_format_number_comma(".1234567890") << std::endl;
    std::cout << string_format_number_comma("a.1234567890") << std::endl;
    std::cout << string_format_number_comma("ab.1234567890") << std::endl;
    std::cout << string_format_number_comma("1.ab234567890") << std::endl;
    std::cout << string_format_number_comma("abcd.0") << std::endl;
    std::cout << string_format_number_comma("ab1234555cdefg") << std::endl;
    std::cout << string_format_number_comma("abcdefg.0") << std::endl;
    std::cout << string_format_number_comma("12345fg.0") << std::endl;
    std::cout << string_format_number_comma("123.45fg.0") << std::endl;
    std::cout << string_format_number_comma("1234.5fg.0") << std::endl;
    std::cout << string_format_number_comma("000123.40005.61117.822290") << std::endl;
}