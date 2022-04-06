#include "common/s4filesystem.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER(fs)

using namespace std::filesystem;

namespace S4
{
    
std::vector<path> search_file(const path & root, const std::string & file_name, bool recursive) {
	std::vector<path> ret;
	if (!exists(root)) {		//.
		LCL_WARN("search_file terminate: no such path:{:}", root.string());
		return ret;
	}
	directory_entry entry(root);		//
	if (!is_directory(root) && root.filename().string() == file_name) {
		//It's a file
		ret.push_back(entry.path());
		// LCL_INFO("List single file: {:}//{:}", ret.back().first, ret.back().second);
	}
	else {
		directory_iterator list(root);	        //
		// LCL_INFO("List files in folder {:} :", root);
		for (auto& it : list) {
			if (is_directory(it)) {
				if (!recursive) {
					continue;
				}
				auto sub = search_file(it.path(), file_name, recursive);
				ret.insert(ret.end(), sub.begin(), sub.end());
			}
			else if (it.path().filename().string() == file_name){
				ret.push_back(it.path());
				// LCL_INFO("{:}//{:}", ret.back().first, ret.back().second);
			}
		}
		// LCL_INFO("List files done!");
	}

	return ret;
}

} // namespace S4
