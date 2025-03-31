#ifndef SM3_H
#define SM3_H

/**************************************************
 * project name: SM3
 * author: wang binbin
 * completion time: 2024.4.3
 *************************************************/


#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<bitset>
#include<vector>

using namespace std;

#define HASH_VALUE_PATH "hash_value.txt"    // A file used to store hash values

class SM3
{
public:
    SM3();

    // Calculate the hash value, the parameter is a binary string
    std::string hashCalculate(const std::string binary_str);

    // Calculate the hash value of the file
    bool hashCalculateFile(std::string filename, std::string outname);
private:
    uint32_t W[68], W_[64];   // For message extension

    // Rotate the 32-bit integer to the left len bits
    uint32_t leftRotate(uint32_t num, uint8_t len);

    // Boolean function 0<=i<=15
    uint32_t ffggFounction(uint32_t X, uint32_t Y, uint32_t Z);

    // Boolean function 16<=i<=63
    uint32_t ffFounction(uint32_t X, uint32_t Y, uint32_t Z);

    // Boolean function 16<=i<=63
    uint32_t ggFounction(uint32_t X, uint32_t Y, uint32_t Z);

    // Displacement function P0
    uint32_t p0Function(uint32_t X);

    // Displacement function P1
    uint32_t p1Function(uint32_t X);

    // Converts an integer to a binary string
    std::string intToBinaryString(uint64_t n, int k);

    // Bit filling function
    std::string performBitPadding(std::string input);

    // Converts a binary string to a 32-bit integer
    uint32_t binaryStringToInt32(std::string binaryString);

    // Message extension function
    void messageExtension(std::string B);

    // Converts a binary string to a hexadecimal string
    std::string binaryToHexChar(const std::string binary_str);

    // compressibility function
    std::string cfFunction(std::string V);
};

#endif // SM3_H
