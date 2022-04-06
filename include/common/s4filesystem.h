#pragma once

#include <filesystem>

namespace S4
{
    
//精确匹配文件名全名
//recursive : ture=递归搜索子目录；false=仅搜索一级目录
std::vector<std::filesystem::path> search_file(const std::filesystem::path & root, const std::string & file_name, bool recursive);

} // namespace S4
