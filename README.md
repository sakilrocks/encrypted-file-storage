# secure-store

A C++17 command line tool that encrypts and decrypts files using *AES-256-CBC* encryption with OpenSSL.   
It provides a simple way to securely store files locally.

---

## Features

- *AES-256-CBC* symmetric encryption  
- Password-based key derivation (PBKDF2 + SHA-256)  
- Random salt and IV for every file  

---

## Project Structure
```
secure-store/
├── include/
│   ├── crypto_utils.hpp     # encryption/decryption functions
│   └── file_utils.hpp       # file I/O helpers
├── src/
│   ├── main.cpp             # CLI entry point
│   ├── crypto_utils.cpp     # AES + key derivation logic
│   └── file_utils.cpp       # binary read/write logic
├── Makefile
└── README.md
```

---

## How It Works

	1.	A random salt and IV (Initialization Vector) are generated for each encryption.
	2.	The user’s password is turned into a 256-bit key using PBKDF2-HMAC-SHA256.
	3.	The file is encrypted with AES-256-CBC.
	4.	Salt + IV are stored at the start of the .enc file.
	5.	During decryption, the same key is derived from the password to recover the original data.

---

## Usage

a. Encrypt a File

``` 
./bin/securestore encrypt secret.txt
```

b. Decrypt a File  

``` 
./bin/securestore decrypt secret.txt.enc
```
