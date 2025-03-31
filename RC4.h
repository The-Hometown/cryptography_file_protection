#ifndef RC4_H
#define RC4_H

/**************************************************
 * project name: RC4
 * author: wang binbin
 * completion time: 2024.3.30
 *************************************************/


#pragma once
#include <iostream>
#include <string>
#include<random>
#include <fstream>
#include<sstream>
#include <vector>
#include<ctime>

using namespace std;

#define SECRET_KEY_PATH "key.txt"

class RC4
{
public:
    RC4();

    // Generate random key
    void genKey(std::string outputname);

    // Get key
    void getKey(std::string filename);

    // The variable len is the length of the data. Encryption is performed when model= 0 and decryption is performed when model=1.
    void enc_dec(char* data, long long len, int model);

    // Process the file. Encryption is performed when model= 0 and decryption is performed when model=1.
    bool enc_dec_file(const std::string filename, const std::string outname, int model);
private:
    std::string Key;

    // Swap two elements
    void swap(unsigned char& a, unsigned char& b);

    // Example Initialize the Sbox
    void initSbox();
};

#endif // RC4_H
