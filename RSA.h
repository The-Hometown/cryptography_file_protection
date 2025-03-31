#ifndef RSA_H
#define RSA_H

#pragma once
#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/multiprecision/random.hpp>
#include<boost/multiprecision/miller_rabin.hpp>
#include<fstream>
#include<vector>
#include<time.h>

using namespace std;

namespace br = boost::random;
namespace bm = boost::multiprecision;

#define NUMBER 128                    // Temporary buffer size
#define PUBLIC_KEY_PATH "public_key.txt"     // Store key n, e，d
#define PRIVATE_KEY_PATH "private_key.txt"     // Store key n, e，d
#define SIZE 128                      // Control the random number size

typedef struct
{
    bm::int1024_t mkey_e;    // public key e
    bm::int1024_t mkey_d;    // private key d
    bm::int1024_t mkey_n;    // n = p * q
}Key;

class RSA
{
public:
    RSA();

    // Generate random key
    void genKeys(std::string publickey_file, std::string privatekey_file);

    // Generate a key or read an existing key
    void getPublicKey(std::string filename);

    // Generate a key or read an existing key
    void getPrivateKey(std::string filename);

    // Encrypt the file
    bool encrypt_file(const std::string filename, const std::string outname);

    // Decrypt the file
    bool decrypt_file(const std::string filename, const std::string outname);
private:
    Key mKey;

    // prime detection
    bool isPrime(bm::int1024_t num);

    // get a prime
    bm::int1024_t getPrime();

    // get key n
    bm::int1024_t getKey_n(bm::int1024_t p, bm::int1024_t q);

    //get euler function f(n)
    bm::int1024_t getFn(bm::int1024_t p, bm::int1024_t q);

    // get public key
    bm::int1024_t getKey_e(bm::int1024_t euler);

    // extended Euclidean algorithm
    void exGcd(bm::int1024_t a, bm::int1024_t b, bm::int1024_t* x, bm::int1024_t* y);

    // get private key
    bm::int1024_t getKey_d(bm::int1024_t key_e, bm::int1024_t euler);

    // get the greatest common divisor
    bm::int1024_t getGcd(bm::int1024_t n1, bm::int1024_t n2);

    // Fast Modular Exponentiation
    bm::int1024_t fastExp(bm::int1024_t data, bm::int1024_t key, bm::int1024_t key_n);
};

#endif // RSA_H

