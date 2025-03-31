#ifndef TDEA_H
#define TDEA_H

/**************************************************
 * project name: 3DES
 * author: wang binbin
 * completion time: 2024.4.16
 *************************************************/


#pragma once
#include "DES.h"
#include <iostream>

using namespace std;


#define SECRET_KEY_PATH "key.txt"
#define ENCRYPTED_FILE "encrypted_file.txt"
#define DECRYPTED_FILE "decrypted_file.txt"



class TDEA
{
public:
    TDEA();

    // Generate random key
    void genKey(std::string outputname);

    // Get the key from the file
    void getKey(std::string filename);

    // Encrypt file
    bool encrypt_file(const string filename, const string outname);

    // Decrypt file
    bool decrypt_file(const string filename, const string outname);
private:
    std::string Key;
    std::string key1, key2, key3;

    // Converts the character to the corresponding two-digit hexadecimal number
    std::string charToHex(unsigned char ch);

    // Converts two hexadecimal digits to the corresponding integer
    int hexToInt(string hex_str);
};

#endif // TDEA_H
