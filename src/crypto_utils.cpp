
#include "crypto_utils.hpp"
#include "file_utils.hpp"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <iostream>
#include <cstring>


namespace crypto {

static const int SALT_SIZE = 16;
static const int IV_SIZE = 16;
static const int KEY_SIZE = 32;

bool derive_key(const std::string& password, const unsigned char* salt, unsigned char* key) {
    return PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt, SALT_SIZE,
                             10000, EVP_sha256(), KEY_SIZE, key);
}



bool encrypt_file(const std::string& input_file, const std::string& password) {
    auto plaintext = file::read_binary(input_file);

    if (plaintext.empty()) {
        std::cerr << "error: cannot read file " << input_file << "\n";
        return false;
    }

    unsigned char salt[SALT_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char key[KEY_SIZE];

    RAND_bytes(salt, SALT_SIZE);
    RAND_bytes(iv, IV_SIZE);
    derive_key(password, salt, key);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
    int len, total_len = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
    total_len += len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + total_len, &len);
    total_len += len;
    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(total_len);



    // write salt + IV + ciphertext

    std::vector<unsigned char> out;
    out.insert(out.end(), salt, salt + SALT_SIZE);
    out.insert(out.end(), iv, iv + IV_SIZE);
    out.insert(out.end(), ciphertext.begin(), ciphertext.end());

    std::string outname = input_file + ".enc";
    file::write_binary(outname, out);
    std::cout << "encrypted -> " << outname << "\n";
    return true;
}




bool decrypt_file(const std::string& input_file, const std::string& password) {
    auto data = file::read_binary(input_file);

    if (data.size() < SALT_SIZE + IV_SIZE) {
        std::cerr << "error: invalid or corrupted file.\n";
        return false;
    }

    unsigned char salt[SALT_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char key[KEY_SIZE];

    memcpy(salt, data.data(), SALT_SIZE);
    memcpy(iv, data.data() + SALT_SIZE, IV_SIZE);
    derive_key(password, salt, key);

    std::vector<unsigned char> ciphertext(data.begin() + SALT_SIZE + IV_SIZE, data.end());
    std::vector<unsigned char> plaintext(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);

    int len, total_len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
    total_len += len;

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + total_len, &len) <= 0) {
        std::cerr << "error: incorrect password or corrupted data.\n";
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    total_len += len;
    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(total_len);

    std::string outname = input_file.substr(0, input_file.find_last_of('.'));
    file::write_binary(outname, plaintext);
    std::cout << "decrypted -> " << outname << "\n";
    return true;

}
}