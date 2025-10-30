
#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <string>
#include <vector>

namespace file {
    std::vector<unsigned char> read_binary(const std::string& path);
    bool write_binary(const std::string& path, const std::vector<unsigned char>& data);
}


#endif