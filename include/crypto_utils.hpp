
#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <string>
#include <vector>

namespace crypto {
    bool encrypt_file(const std::string& input_file, const std::string& password);
    bool decrypt_file(const std::string& input_file, const std::string& password);
}


#endif