/**************************************************
 * project name: SM3
 * author: wang binbin
 * completion time: 2024.4.3
 *************************************************/

#include "SM3.h"


const std::string IV = "7380166f4914b2b9172442d7da8a0600a96f30bc163138aae38dee4db0fb0e4e";    // 256 bits
const uint32_t T1 = 0x79cc4519;
const uint32_t T2 = 0x7a879d8a;

SM3::SM3() {};

uint32_t SM3::leftRotate(uint32_t num, uint8_t len)
{
    return (num << len) | (num >> (32 - len));
}

uint32_t SM3::ffggFounction(uint32_t X, uint32_t Y, uint32_t Z)
{
    return X ^ Y ^ Z;
}

uint32_t SM3::ffFounction(uint32_t X, uint32_t Y, uint32_t Z)
{
    return (X & Y) | (X & Z) | (Y & Z);
}

uint32_t SM3::ggFounction(uint32_t X, uint32_t Y, uint32_t Z)
{
    return (X & Y) | (~X & Z);
}

uint32_t SM3::p0Function(uint32_t X)
{
    return X ^ leftRotate(X, 9) ^ leftRotate(X, 17);
}

uint32_t SM3::p1Function(uint32_t X)
{
    return X ^ leftRotate(X, 15) ^ leftRotate(X, 23);
}

std::string SM3::intToBinaryString(uint64_t n, int k)       // k is the width of the binary bit string
{
    std::string binaryString;

    // Iterate over each bit of the 64-bit integer
    for (int i = k-1; i >= 0; i--)
    {
        uint64_t bit = (n >> i) & 1;            // Check if the i-th bit is set (1) or not (0)
        binaryString += std::to_string(bit);    // Append the bit to the binary string
    }
    return binaryString;
}

std::string SM3::performBitPadding(std::string input)   // input is a binary bit string
{
    int k = 448 - 1 - (input.length() % 512);
    return input + "1" + std::string(k, '0') + intToBinaryString(input.length(), 64);
}

uint32_t SM3::binaryStringToInt32(std::string binaryString)
{
    std::bitset<32> bits(binaryString);                // Create a bitset with the binary string
    return static_cast<uint32_t>(bits.to_ulong());     // Convert the bitset to an unsigned long and then to uint32_t
}

void SM3::messageExtension(std::string B)
{
    // Decompose a string of length 512 into sixteen strings of length 32
    std::vector<std::string> chunks;
    size_t pos = 0;
    while (pos < B.length())
    {
        chunks.push_back(B.substr(pos, 32));
        pos = pos + 32;
    }

    for (int i = 0; i < 16; i++)
    {
        W[i] = std::stoul(chunks[i], nullptr, 2);    // Converts a binary string of length 32 to an unsigned 32-bit integer
    }

    for (int i = 16; i < 68; i++)
    {
        W[i] = p1Function(W[i - 16] ^ W[i - 9] ^ leftRotate(W[i - 3], 15)) ^ leftRotate(W[i - 13], 7) ^ W[i - 6];
    }

    for (int i = 0; i < 64; i++)
    {
        W_[i] = W[i] ^ W[i + 4];
    }
}

std::string SM3::binaryToHexChar(const std::string binary_str)
{
    char c;
    string rst;   // Store return value

    size_t pos = 0;
    while (pos < binary_str.length())
    {
        int decimal = std::bitset<4>(binary_str.substr(pos, 4)).to_ulong();       // Converts a binary string to a decimal number
        pos = pos + 4;

        if (decimal >= 0 && decimal <= 9)
        {
            c = '0' + decimal;            // '0' ~ '9'
        }
        else if (decimal >= 10 && decimal <= 15)
        {
            c = 'a' + (decimal - 10);    // 'a' ~ 'f'
        }

        rst = rst + c;
    }

    return rst;
}

std::string SM3::cfFunction(std::string V)
{
    uint32_t A, B, C, D, E, F, G, H;

    // Decompose a string of length 64 into eight strings of length 8
    std::vector<std::string> chunks;
    size_t pos = 0;
    while (pos < V.length())
    {
        chunks.push_back(V.substr(pos, 8));
        pos = pos + 8;
    }

    // Converts a hexadecimal string of length 8 to an unsigned 32-bit integer
    A = std::stoul(chunks[0], nullptr, 16);
    B = std::stoul(chunks[1], nullptr, 16);
    C = std::stoul(chunks[2], nullptr, 16);
    D = std::stoul(chunks[3], nullptr, 16);
    E = std::stoul(chunks[4], nullptr, 16);
    F = std::stoul(chunks[5], nullptr, 16);
    G = std::stoul(chunks[6], nullptr, 16);
    H = std::stoul(chunks[7], nullptr, 16);

    // Store the value before V changed
    std::string V_binary_str = intToBinaryString(A, 32) + intToBinaryString(B, 32) + intToBinaryString(C, 32) + intToBinaryString(D, 32) + intToBinaryString(E, 32)
        + intToBinaryString(F, 32) + intToBinaryString(G, 32) + intToBinaryString(H, 32);

    uint32_t SS1, SS2, TT1, TT2;    // Store intermediate
    for (int i = 0; i < 64; i++)
    {
        if (i <= 15)
        {
            SS1 = leftRotate((leftRotate(A, 12) + E + leftRotate(T1, i)), 7);
            SS2 = SS1 ^ leftRotate(A, 12);
            TT1 = ffggFounction(A, B, C) + D + SS2 + W_[i];
            TT2 = ffggFounction(E, F, G) + H + SS1 + W[i];
        }
        else
        {
            SS1 = leftRotate((leftRotate(A, 12) + E + leftRotate(T2, i % 32)), 7);
            SS2 = SS1 ^ leftRotate(A, 12);
            TT1 = ffFounction(A, B, C) + D + SS2 + W_[i];
            TT2 = ggFounction(E, F, G) + H + SS1 + W[i];
        }

        D = C;
        C = leftRotate(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = leftRotate(F, 19);
        F = E;
        E = p0Function(TT2);
    }

    std::string A_H_str = intToBinaryString(A, 32) + intToBinaryString(B, 32) + intToBinaryString(C, 32) + intToBinaryString(D, 32) + intToBinaryString(E, 32)
        + intToBinaryString(F, 32) + intToBinaryString(G, 32) + intToBinaryString(H, 32);

    std::string rst;
    for (int i = 0; i < 256; i++)    // Use A-H xor v[i] to get V[i+1]
    {
        if (A_H_str[i] == V_binary_str[i])
        {
            rst = rst + "0";
        }
        else
        {
            rst = rst + "1";
        }
    }

    rst = binaryToHexChar(rst);    // Converts the result from a binary string to a hexadecimal string

    return rst;
}

std::string SM3::hashCalculate(std::string binary_str)
{
    /* test case
    binary_str = "01100001011000100110001101100100011000010110001001100011011001000110000101100010011000110110010001100001011000100110001101100100011000010110001001100011011001000110000101100010011000110110010001100001011000100110001101100100011000010110001001100011011001000110000101100010011000110110010001100001011000100110001101100100011000010110001001100011011001000110000101100010011000110110010001100001011000100110001101100100011000010110001001100011011001000110000101100010011000110110010001100001011000100110001101100100";
    binary_str = "011000010110001001100011";  */

    binary_str = performBitPadding(binary_str);
    uint64_t group_num = binary_str.length() / 512;

    size_t pos = 0;
    std::string hash_value = IV;
    for (uint64_t i = 0; i < group_num; i++)
    {
        messageExtension(binary_str.substr(pos, 512));
        pos = pos + 512;
        hash_value = cfFunction(hash_value);
    }

    return hash_value;
}


bool SM3::hashCalculateFile(const std::string filename, std::string outname)
{
    std::ifstream inputFile(filename, ifstream::binary);
    std::ofstream outputFile(outname, ifstream::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open input file" << std::endl;
        return false;
    }

    inputFile.seekg(0, std::ifstream::end);          // Move the file pointer to the end of the file
    std::streampos fileSize = inputFile.tellg();     // Gets the current position of the file pointer, that is, the size of the file
    inputFile.seekg(0, std::ifstream::beg);          // Move the file pointer back to the beginning of the file
    uint64_t str_len = fileSize;


    char* buffer = new char[str_len];
    inputFile.read(buffer, str_len);       // Reads the contents of the file into the buffer

    std::string m_binary_str;
    for (int i = 0; i < str_len; i++)      // Converts the contents of a file to a binary string
    {
        m_binary_str = m_binary_str + intToBinaryString(uint8_t(buffer[i]), 8);
    }

    std::string hash_value = hashCalculate(m_binary_str);
    outputFile.write(hash_value.c_str(), hash_value.size());        // Writes the encrypted data to the output file

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
