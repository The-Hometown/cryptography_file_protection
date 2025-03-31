#ifndef DES_H
#define DES_H

/**************************************************
 * project name: DES
 * author: wang binbin
 * completion time: 2024.4.15
 *************************************************/


#pragma once
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<random>
#include<string>
#include<bitset>

using namespace std;


#define SECRET_KEY_PATH "key.txt"
#define ENCRYPTED_FILE "encrypted_file.txt"
#define DECRYPTED_FILE "decrypted_file.txt"


class DES
{
public:
    DES();

    // Generate random key
    void genKey(std::string outputname);

    // Get the key from the file
    void getKey(std::string filename);

    // Set key
    void setKey(std::string key_hex_str);

    // The hexadecimal string is encrypted when model=0 and decrypted when model=1
    std::string enc_dec(std::string hex_str, int model);

    // Encrypt the file
    bool encrypt_file(const string filename, const string outname);

    // Decrypt the file
    bool decrypt_file(const string filename, const string outname);
private:
    std::string Key;
    std::string round_key[16];    // Store the generated round key

    // Replace a single character with a corresponding two-digit hexadecimal
    std::string charToHex(unsigned char ch);

    // Converts an 8-digit integer to a two-digit hexadecimal number
    std::string intToHex(int value);

    // Converts two hexadecimal digits to the corresponding integer
    int hexToInt(std::string hex_str);

    // Converts a hexadecimal string to the corresponding binary string
    std::string hexToBin(std::string hex_str);

    // Converts a binary string to the corresponding hexadecimal string
    std::string binToHex(std::string bin_str);

    // Converts an 8-bit integer to the corresponding binary string
    std::string intToBin(uint8_t n, int k);

    // Converts a binary string to the corresponding integer
    int binToInt(std::string bin_str);

    // Xor two binary strings of the same length and return the binary string after the XOR
    std::string binstrXor(std::string bin_str1, std::string bin_str2);

    // initial permutation function
    std::string initialPermutation(std::string hex_str);

    // Generate round keys based on the Key value
    void genRoundKey();

    // E extension function
    std::string extendOperation(std::string bin_str);

    // S_box substitution function
    std::string subSbox(std::string bin_str);

    // P-replacement function
    std::string pPermutation(std::string bin_str);

    // Round function f
    std::string fFounction(std::string bin_str, std::string round_key);

    // Inverse initial permutation function
    std::string invInitialPermutation(std::string hex_str);
};

#endif // DES_H
