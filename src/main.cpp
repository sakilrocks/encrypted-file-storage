
#include "crypto_utils.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: \n"
                  << "  " << argv[0] << " encrypt <filename>\n"
                  << "  " << argv[0] << " decrypt <filename>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string file = argv[2];
    std::string password;

    std::cout << "enter password: ";
    std::cin >> password;

    if (mode == "encrypt")
        crypto::encrypt_file(file, password);
    else if (mode == "decrypt")
        crypto::decrypt_file(file, password);
    else
        std::cerr << "invalid mode. use 'encrypt' or 'decrypt'.\n";

    return 0;
}