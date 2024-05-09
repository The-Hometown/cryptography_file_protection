#ifndef AES_H
#define AES_H

/**************************************************
 * project name: AES
 * author: wang binbin
 * completion time: 2024.3.26
 *************************************************/


#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>
#include<cstdio>
#include<bitset>
#include<stack>
#include<cmath>
#include<random>
#include<ctime>

using namespace std;

#define NUMBER 16
#define SECRET_KEY_PATH "key.txt"


class AES
{
public:
    AES();

    // Generate a random key
    void genKey(std::string outputname);

    // Get key
    void getKey(const std::string filename);

    // AES encryption
    void encrypt(int ptext[4][4]);

    // AES decryption
    void decrypt(int ctext[4][4]);

    // Encrypt the file
    bool encrypt_file(const std::string filename, const std::string outname);

    // Decrypt the file
    bool decrypt_file(const std::string filename, const std::string outname);

private:
    std::string Key;

    // Generating S-box
    void genSbox(int box[16][16]);

    // Generating inverse S-box
    void genSbox2(int box[16][16]);

    // Multiplication over finite fields
    int gfMul(int n, int m);

    // Find inverses over finite fields
    int invElement(int m);

    // Byte move
    void leftByte(int word[4]);

    // G function for MixColumn
    void gFunction(int word[4], int round);

    // key expansion
    void keyExpansion(int word[4][44]);

    // Byte conversion
    void subBytes(int m[4][4]);

    // Inverse byte conversion
    void subBytes2(int m[4][4]);

    // Row shift
    void shiftRow(int m[4][4]);

    // Inverse row shift
    void shiftRow2(int m[4][4]);

    // Column mix
    void mixColumn(int m[4][4]);

    // Inverse column mix
    void mixColumn2(int m[4][4]);

    // RoundKey Add
    void addRoundKey(int m[4][4], int key[4][4]);

    // Integer to hexadecimal
    std::string intToHex(int value);

    // Hexadecimal to integer
    int hexToInt(const std::string& hexStr);
};

#endif // AES_H
