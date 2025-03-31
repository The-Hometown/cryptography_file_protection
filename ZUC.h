#ifndef ZUC_H
#define ZUC_H

/**************************************************
 * project name: ZUC
 * author: wang binbin
 * completion time: 2024.4.11
 *************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <bitset>
#include <ctime>

using namespace std;

#define KEYANDIV_PATH "CK.txt"
#define ENCRYPTED_FILE_PATH "encrypted_file.txt"       // File path to store the encrypted content
#define DECRYPTED_FILE_PATH "decrypted_file.txt"       // File path to store the decrypted content

class ZUC
{
public:
    uint32_t LFSR_S[17];    // Linear feedback shift register 16 31-bit register unit variables
    uint32_t BR_X[4];       // Bit reorganization output of four 32-bit words
    uint32_t R1, R2;        // Two 32 bit memory unit variables of a nonlinear function F

    ZUC();

    // Generate input parameters and key CK
    void genKey(std::string outputname);

    // Gets the input parameters and key CK from the file
    void getKey(std::string filename);

    // Encryption and decryption function
    std::string enc_dec(std::string in_hex_str);

    // Text encryption and decryption function
    bool enc_dec_file(std::string inputfile, std::string outputfile, int model);
private:
    std::string CK;    // key
    std::string COUNT, BEARER, DIRECTION;  // input parameter

    // Converts a two-digit hexadecimal string to an integer
    int hexToInt(std::string hexStr);

    // Converts a character to a two-digit hexadecimal string
    std::string charToHex(unsigned char ch);

    // Converts a binary bit string to a 32-bit integer
    uint32_t binToInt32(std::string bin_str);

    // Converts a hexadecimal string to a binary string
    std::string hexToBin(std::string hexStr);

    // Converts a binary string to a hexadecimal string
    std::string binToHex(const std::string bin_str);

    // Converts a 32-bit integer to an 8-bit hexadecimal string
    std::string int32ToHex(uint32_t value);

    // Module 2^31-1 addition operation
    uint32_t mod_add(uint32_t a, uint32_t b);

    // Module 2^31-1 multiplication, rst = [x * (2^k)] mod (2^31-1)
    uint32_t mod_2exp_mul(uint32_t x, int exp);

    // LFSR initialization mode
    void lfsrWithInitialisationMode(uint32_t u);

    // LFSR working mode
    void lfsrWithWorkMode();

    // Bit recombination function
    void bitReconstruction();

    // Rotate the 32-bit integer to the left len bits
    uint32_t leftRotate(uint32_t num, uint8_t len);

    // L1 function for nonlinear functions
    uint32_t L1Function(uint32_t X);

    // L2 function for nonlinear functions
    uint32_t L2Function(uint32_t X);

    // Word replacement function for non-linear functions
    uint32_t subWord(uint32_t m);

    // Nonlinear function F
    uint32_t fFunction(uint32_t X0, uint32_t X1, uint32_t X2);

    // Initializes 16 register variables in the LFSR
    void initializeLFSR_S();

    // Algorithm run - initialization step
    void initialisationMode();

    // Algorithm run - work steps
    std::string workMode(int L);
};

#endif // ZUC_H
