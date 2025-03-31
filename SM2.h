#ifndef SM2_H
#define SM2_H

/********************************
* project name: SM2
* author: wangbinbin
* completion time:  2024.4.7
********************************/


#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<string>
#include<vector>
#include "SM3.h"


extern "C"
{
#include "miracl.h"
#include "mirdef.h"
}

using namespace std;

#define SEED_CONST 0x1BD8C95A
#define PRIVATE_KEY_PATH "private_key.txt"    // The path where the generated private key is stored
#define PUBLIC_KEY_PATH "public_key.txt"      // The path where the generated public key is stored
#define PLAIN_PATH "I believe I can fly .txt"          // The path to store the plaintext
#define ENCRYPTED_FILE_PATH "encrypted_file.txt"       // File path to store the encrypted content
#define DECRYPTED_FILE_PATH "decrypted_file.txt"       // File path to store the decrypted content

typedef struct
{
    epoint* G;
    epoint* pB;
}PubKey;  // public key of user B


class SM2
{
public:
    big p, a, b, n, x, y;      // n is order of G. G = (x,y)
    big xB, yB;

    SM2();

    // Generates A pair of public and private keys, stored in PUBLIC_KEY_PATH and PRIVATE_KEY_PATH respectively
    bool genKeys(std::string publickey_file, std::string privatekey_file);

    void getPublicKey(std::string filename);

    void getPrivateKey(std::string filename);

    // Encrypts the plaintext binary bit string and returns the encrypted string
    std::string encrypt(std::string m_bin_str);

    // Decrypt the ciphertext and return the decrypted string
    std::string decrypt(std::string C_str);

    // Encrypt the file
    bool encrypt_file(std::string inputname, std::string outputname);

    // Decrypt the file
    bool decrypt_file(std::string inputname, std::string outputname);
private:
    big k;            // 用户私钥
    PubKey publickey;    // 用户公钥

    // Elliptic curve parameter initialization
    void paraInit();

    // Read pB from the file and initialize xB and yB
    void readpB(std::string filepath);

    // Read dA from the file and initialize k
    void readdA(std::string filepath);

    // Reads a dB assignment from a file
    void readdB(std::string filepath, big& dB);

    int hexToInt(std::string hexStr);

    // Converts the hexadecimal string to the corresponding binary bit string, reads the hexadecimal string, and returns the corresponding binary bit string
    std::string hexToBin(std::string hexStr);

    // Converts the binary bit string to the corresponding hexadecimal string, reads the binary bit string, and returns the corresponding hexadecimal string
    std::string binToHex(std::string binary_str);

    // Converts a character to a two-digit hexadecimal string, reads in an unsigned character, and returns a hexadecimal string
    std::string charToHex(unsigned char ch);

    // Converts a variable of type big to the corresponding binary string
    std::string bigToBin(big x);

    // Converts a variable of type big to the corresponding hexadecimal string
    std::string bigToHex(big x);

    // Converts a point on an elliptic curve to a string of binary bits (x_str || y_str)
    std::string epointToBin(epoint* P);

    // Converts an integer to the corresponding binary bit string
    std::string intToBin(uint32_t n, int k);

    // Key derivation function
    std::string kdfFunction(std::string Z, int klen);
};

#endif // SM2_H
