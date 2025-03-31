/**************************************************
 * project name: DES
 * author: wang binbin
 * completion time: 2024.4.15
 *************************************************/

#include "DES.h"


// Initialize permutation table
const static uint8_t IP_1[64] =
{
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// Terminal permutation
const static uint8_t IP_2[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

// Expansion permutation, expanding data from 32-bit to 48 bits
static const uint8_t E_box[48] =
{
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// S box replacement
const static uint8_t Sbox[8][64] =
{
    {//S1 box
        14 , 4 , 13 , 1 , 2 , 15 , 11 , 8 , 3 , 10 , 6 , 12 , 5 , 9 , 0 , 7 ,
        0 , 15 , 7 , 4 , 14 , 2 , 13 , 1 , 10 , 6 , 12 , 11 , 9 , 5 , 3 , 8 ,
        4 , 1 , 14 , 8 , 13 , 6 , 2 , 11 , 15 , 12 , 9 , 7 , 3 , 10 , 5 , 0 ,
        15 , 12 , 8 , 2 , 4 , 9 , 1 , 7 , 5 , 11 , 3 , 14 , 10 , 0 , 6 , 13
    },
    {//S2 box
        15 , 1 , 8 , 14 , 6 , 11 , 3 , 4 , 9 , 7 , 2 , 13 , 12 , 0 , 5 , 10 ,
        3 , 13 , 4 , 7 , 15 , 2 , 8 , 14 , 12 , 0 , 1 , 10 , 6 , 9 , 11 , 5 ,
        0 , 14 , 7 , 11 , 10 , 4 , 13 , 1 , 5 , 8 , 12 , 6 , 9 , 3 , 2 , 15 ,
        13 , 8 , 10 , 1 , 3 , 15 , 4 , 2 , 11 , 6 , 7 , 12 , 0 , 5 , 14 , 9
    },
    {//S3 box
        10 , 0 , 9 , 14 , 6 , 3 , 15 , 5 , 1 , 13 , 12 , 7 , 11 , 4 , 2 , 8 ,
        13 , 7 , 0 , 9 , 3 , 4 , 6 , 10 , 2 , 8 , 5 , 14 , 12 , 11 , 15 , 1 ,
        13 , 6 , 4 , 9 , 8 , 15 , 3 , 0 , 11 , 1 , 2 , 12 , 5 , 10 , 14 , 7 ,
        1 , 10 , 13 , 0 , 6 , 9 , 8 , 7 , 4 , 15 , 14 , 3 , 11 , 5 , 2 , 12
    },
    {//S4 box
        7 , 13 , 14 , 3 , 0 , 6 , 9 , 10 , 1 , 2 , 8 , 5 , 11 , 12 , 4 , 15 ,
        13 , 8 , 11 , 5 , 6 , 15 , 0 , 3 , 4 , 7 , 2 , 12 , 1 , 10 , 14 , 9 ,
        10 , 6 , 9 , 0 , 12 , 11 , 7 , 13 , 15 , 1 , 3 , 14 , 5 , 2 , 8 , 4 ,
        3 , 15 , 0 , 6 , 10 , 1 , 13 , 8 , 9 , 4 , 5 , 11 , 12 , 7 , 2 , 14
    },
    {//S5 box
        2 , 12 , 4 , 1 , 7 , 10 , 11 , 6 , 8 , 5 , 3 , 15 , 13 , 0 , 14 , 9 ,
        14 , 11 , 2 , 12 , 4 , 7 , 13 , 1 , 5 , 0 , 15 , 10 , 3 , 9 , 8 , 6 ,
        4 , 2 , 1 , 11 , 10 , 13 , 7 , 8 , 15 , 9 , 12 , 5 , 6 , 3 , 0 , 14 ,
        11 , 8 , 12 , 7 , 1 , 14 , 2 , 13 , 6 , 15 , 0 , 9 , 10 , 4 , 5 , 3
    },
    {//S6 box
        12 , 1 , 10 , 15 , 9 , 2 , 6 , 8 , 0 , 13 , 3 , 4 , 14 , 7 , 5 , 11 ,
        10 , 15 , 4 , 2 , 7 , 12 , 9 , 5 , 6 , 1 , 13 , 14 , 0 , 11 , 3 , 8 ,
        9 , 14 , 15 , 5 , 2 , 8 , 12 , 3 , 7 , 0 , 4 , 10 , 1 , 13 , 11 , 6 ,
        4 , 3 , 2 , 12 , 9 , 5 , 15 , 10 , 11 , 14 , 1 , 7 , 6 , 0 , 8 , 13
    },
    {//S7 box
        4 , 11 , 2 , 14 , 15 , 0 , 8 , 13 , 3 , 12 , 9 , 7 , 5 , 10 , 6 , 1 ,
        13 , 0 , 11 , 7 , 4 , 9 , 1 , 10 , 14 , 3 , 5 , 12 , 2 , 15 , 8 , 6 ,
        1 , 4 , 11 , 13 , 12 , 3 , 7 , 14 , 10 , 15 , 6 , 8 , 0 , 5 , 9 , 2 ,
        6 , 11 , 13 , 8 , 1 , 4 , 10 , 7 , 9 , 5 , 0 , 15 , 14 , 2 , 3 , 12
    },
    {//S8 box
        13 , 2 , 8 , 4 , 6 , 15 , 11 , 1 , 10 , 9 , 3 , 14 , 5 , 0 , 12 , 7 ,
        1 , 15 , 13 , 8 , 10 , 3 , 7 , 4 , 12 , 5 , 6 , 11 , 0 , 14 , 9 , 2 ,
        7 , 11 , 4 , 1 , 9 , 12 , 14 , 2 , 0 , 6 , 10 , 13 , 15 , 3 , 5 , 8 ,
        2 , 1 , 14 , 7 , 4 , 10 , 8 , 13 , 15 , 12 , 9 , 0 , 3 , 5 , 6 , 11
    }
};

// P-box replacement
const static uint8_t P[32] =
{
    16 , 7 , 20 , 21 , 29 , 12 , 28 , 17 , 1 , 15 , 23 , 26 , 5 , 18 , 31 , 10 ,
    2 , 8 , 24 , 14 , 32 , 27 , 3 , 9 , 19 , 13 , 30 , 6 , 22 , 11 , 4 , 25
};

// Key permutation table, compressing 64 bit key permutations into 56 bit permutations
const static uint8_t PC_1[56] =
{
    57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 ,
    58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 ,
    59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
    60 , 52 , 44 , 36 , 63 , 55 , 47 , 39 ,
    31 , 23 , 15 , 7 , 62 , 54 , 46 , 38 ,
    30 , 22 , 14 , 6 , 61 , 53 , 45 , 37 ,
    29 , 21 , 13 , 5 , 28 , 20 , 12 , 4
};

// The number of digits moved in each round
const static uint8_t bit_shift[16] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// Compressed permutation table, 56 bit key, compressed 48 bit key
const static uint8_t PC_2[48] =
{
    14 , 17 , 11 , 24 , 1 , 5 , 3 , 28 ,
    15 , 6 , 21 , 10 , 23 , 19 , 12 , 4 ,
    26 , 8 , 16 , 7 , 27 , 20 , 13 , 2 ,
    41 , 52 , 31 , 37 , 47 , 55 , 30 , 40 ,
    51 , 45 , 33 , 48 , 44 , 49 , 39 , 56 ,
    34 , 53 , 46 , 42 , 50 , 36 , 29 , 32
};

DES::DES() {};

std::string DES::charToHex(unsigned char ch)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    return ss.str();
}

std::string DES::intToHex(int value)
{
    stringstream stream;
    stream << hex << setw(2) << setfill('0') << value;
    return stream.str();
}

int DES::hexToInt(string hex_str)
{
    std::stringstream stream(hex_str);
    int value;
    stream >> hex >> value;
    return value;
}

std::string DES::hexToBin(std::string hex_str)
{
    std::string bin_str;
    for (char hexChar : hex_str)
    {
        switch (hexChar)
        {
            case '0': bin_str += "0000"; break;
            case '1': bin_str += "0001"; break;
            case '2': bin_str += "0010"; break;
            case '3': bin_str += "0011"; break;
            case '4': bin_str += "0100"; break;
            case '5': bin_str += "0101"; break;
            case '6': bin_str += "0110"; break;
            case '7': bin_str += "0111"; break;
            case '8': bin_str += "1000"; break;
            case '9': bin_str += "1001"; break;
            case 'A': case 'a': bin_str += "1010"; break;
            case 'B': case 'b': bin_str += "1011"; break;
            case 'C': case 'c': bin_str += "1100"; break;
            case 'D': case 'd': bin_str += "1101"; break;
            case 'E': case 'e': bin_str += "1110"; break;
            case 'F': case 'f': bin_str += "1111"; break;
            default:
                std::cerr << "Invalid hex character: " << hexChar << std::endl;
                return ""; // If an invalid hexadecimal character is entered, an empty string is returned
        }
    }
    return bin_str;
}

std::string DES::binToHex(std::string bin_str)
{
    char c;
    string rst;   // Store return value

    size_t pos = 0;
    while (pos < bin_str.length())
    {
        int decimal = std::bitset<4>(bin_str.substr(pos, 4)).to_ulong();       // Converts a binary string to a decimal number
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

std::string DES::intToBin(uint8_t n, int k)       // k is the width of the binary bit string
{
    std::string bin_str;

    // Iterate over each bit of the 64-bit integer
    for (int i = k - 1; i >= 0; i--)
    {
        uint8_t bit = (n >> i) & 1;              // Check if the i-th bit is set (1) or not (0)
        bin_str = bin_str + std::to_string(bit);    // Append the bit to the binary string
    }
    return bin_str;
}

int DES::binToInt(std::string bin_str)
{
    std::bitset<32> bits(bin_str);    // Assume that the string length does not exceed 32 bits
    int decimal_num = static_cast<int>(bits.to_ulong());

    return decimal_num;
}

std::string DES::binstrXor(std::string bin_str1, std::string bin_str2)
{
    std::string rst_bin_str;
    for (int i = 0; i < bin_str1.length(); i++)   // Xor two binary strings of the same length
    {
        if (bin_str1[i] == bin_str2[i])
        {
            rst_bin_str = rst_bin_str + "0";
        }
        else
        {
            rst_bin_str = rst_bin_str + "1";
        }
    }

    return rst_bin_str;
}

void DES::genKey(std::string outputname)
{
    cout << "生成密钥中......" << endl;

    std::string key;

    const std::string charset = "0123456789abcdef";
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);

    for (int i = 0; i < 16; i++)         // Generate random key
    {
        key = key + charset[dist(gen)];
    }

    Key = key;   // Assign the generated key to the private key variable

    // Write the generated key to a file
    std::ofstream outputFile(outputname);
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file: " << outputname << std::endl;
    }
    outputFile << key << std::endl;
    outputFile.close();
    cout << "生成的密钥已写入 " << outputname << " 中。" << endl;
}

void DES::getKey(std::string filename)
{
    std::ifstream file(filename);          // open file
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filename << "!" << std::endl;
    }
    else
    {
        if (!(std::getline(file, Key)))
        {
            std::cerr << "Failed to read " << filename << "!" << std::endl;
        }
    }

    file.close();    // closed file
    genRoundKey();   // The round key generation function is called after the key is obtained
}

void DES::setKey(std::string key_hex_str)
{
    Key = key_hex_str;
    genRoundKey();  // The round keys is regenerated each time the key variable is changed
}

std::string DES::initialPermutation(std::string hex_str)
{
    std::string bin_str = "0" + hexToBin(hex_str);

    std::string rst_bin_str;
    for (int i = 0; i < 64; i++)    // Perform the initial permutation according to the IP_1 table
    {
        rst_bin_str = rst_bin_str + bin_str[IP_1[i]];
    }

    return rst_bin_str;
}

void DES::genRoundKey()
{
    std::string bin_Key = "0" + hexToBin(Key);
    std::string compressed_bin_Key;

    for (int i = 0; i < 56; i++)   // Compress the 64-bit key to a 56-bit key according to the PC_1 table
    {
        compressed_bin_Key = compressed_bin_Key + bin_Key[PC_1[i]];
    }

    std::string C_bin_str = compressed_bin_Key.substr(0, 28);    // Gets the left half of the compressed key
    std::string D_bin_str = compressed_bin_Key.substr(28);       // Gets the right half of the compressed key
    for (int i = 0; i < 16; i++)
    {
        std::string bin_round_key;
        std::string CandD_bin_str;          // Stores the result of the recombination of C and D
        C_bin_str = C_bin_str.substr(bit_shift[i]) + C_bin_str.substr(0, bit_shift[i]);      // Move the C loop left by bitshift[i] bits
        D_bin_str = D_bin_str.substr(bit_shift[i]) + D_bin_str.substr(0, bit_shift[i]);      // Move the D loop left by bitshift[i] bits

        CandD_bin_str = "0" + C_bin_str + D_bin_str;
        for (int j = 0; j < 48; j++)     // Get the round key according to PC_2
        {
            bin_round_key = bin_round_key + CandD_bin_str[PC_2[j]];
        }
        round_key[i] = bin_round_key;
    }
}

std::string DES::extendOperation(std::string bin_str)
{
    bin_str = "0" + bin_str;
    std::string rst_bin_str;
    for (int i = 0; i < 48; i++)   // Expand 32 bits to 48 bits according to the E_box table
    {
        rst_bin_str = rst_bin_str + bin_str[E_box[i]];
    }

    return rst_bin_str;
}

std::string DES::subSbox(std::string bin_str)
{
    std::string arr[8];
    size_t pos = 0;
    for (int i = 0; i < 8; i++)
    {
        arr[i] = bin_str.substr(pos, 6);
        pos = pos + 6;
    }

    std::string rst_bin_str;
    for (int i = 0; i < 8; i++)   // 6 In and 4 out, replacing 48 bits with 32 bits
    {
        int row = binToInt(arr[i].substr(0, 1) + arr[i].substr(5, 1));
        int col = binToInt(arr[i].substr(1, 4));
        rst_bin_str = rst_bin_str + intToBin(Sbox[i][row * 16 + col], 4);
    }

    return rst_bin_str;
}

std::string DES::pPermutation(std::string bin_str)
{
    bin_str = "0" + bin_str;
    std::string rst_bin_str;
    for (int i = 0; i < 32; i++)  // Perform P permutation according to P table
    {
        rst_bin_str = rst_bin_str + bin_str[P[i]];
    }

    return rst_bin_str;
}

std::string DES::fFounction(std::string bin_str, std::string round_key)
{
    std::string L_bin_str = bin_str.substr(0, 32);
    std::string R_bin_str = bin_str.substr(32);
    std::string temp_bin_str = R_bin_str;   // Back up the right half first

    // Operate on the right half
    R_bin_str = extendOperation(R_bin_str);       // E Extend
    R_bin_str = binstrXor(R_bin_str, round_key);  // Xor and round key
    R_bin_str = subSbox(R_bin_str);               // S_box substitution
    R_bin_str = pPermutation(R_bin_str);          // P permutation
    R_bin_str = binstrXor(R_bin_str, L_bin_str);  // Xor with the left half

    return temp_bin_str + R_bin_str;
}

std::string DES::invInitialPermutation(std::string hex_str)
{
    std::string bin_str = "0" + hexToBin(hex_str);
    std::string rst_bin_str;
    for (int i = 0; i < 64; i++)  // Perform inverse initial permutation according to the IP_2 table
    {
        rst_bin_str = rst_bin_str + bin_str[IP_2[i]];
    }

    return rst_bin_str;
}

std::string DES::enc_dec(std::string hex_str, int model)
{
    std::string out_bin_str;
    out_bin_str = initialPermutation(hex_str);

    if (model == 0)   // The encryption mode uses round keys in positive sequence
    {
        for (int i = 0; i < 16; i++)
        {
            out_bin_str = fFounction(out_bin_str, round_key[i]);
        }
    }
    else if (model == 1)   // Decryption mode uses round key in reverse order
    {
        for (int i = 0; i < 16; i++)
        {
            out_bin_str = fFounction(out_bin_str, round_key[15 - i]);
        }
    }

    out_bin_str = out_bin_str.substr(32) + out_bin_str.substr(0, 32);         // At the end of round 16, the left and right parts are exchanged again

    out_bin_str = invInitialPermutation(binToHex(out_bin_str));

    // cout << binToHex(out_bin_str) << endl;

    return binToHex(out_bin_str);
}

bool DES::encrypt_file(const string filename, const string outname)
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
    uint64_t data_len = fileSize;

    char* buffer = new char[data_len];
    inputFile.read(buffer, data_len);

    std::string in_str;
    for (uint64_t i = 0; i < data_len; i++)
    {
        in_str = in_str + buffer[i];
    }

    // character fill
    int n = 8 - data_len % 8;
    std::string space_str(n, ' ');
    in_str = in_str + space_str;

    for (int i = 0; i < data_len + n; i = i + 8)
    {
        std::string in_hex_str;
        std::string out_hex_str;
        for (int j = 0; j < 8; j++)
        {
            in_hex_str = in_hex_str + charToHex(uint8_t(in_str[(i + j)]));   // Convert the characters to uint8_t to avoid garbled Chinese characters
        }

        out_hex_str = enc_dec(in_hex_str , 0);

        std::string out_str;
        size_t pos = 0;
        for (int j = 0; j < 8; j++)
        {
            out_str = out_str + char(hexToInt(out_hex_str.substr(pos, 2)));
            pos = pos + 2;
        }
        outputFile.write(out_str.c_str(), out_str.size());   // The encrypted ciphertext is written to the file
    }

    delete[] buffer;   // Free pointer space
    inputFile.close();
    outputFile.close();

    return true;
}

bool DES::decrypt_file(const string filename, const string outname)
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
    uint64_t data_len = fileSize;

    outputFile << "\xEF\xBB\xBF";       // set the text encoding format to utf-8

    char* buffer = new char[data_len];
    inputFile.read(buffer, data_len);

    std::string in_str;
    for (uint64_t i = 0; i < data_len; i++)
    {
        in_str = in_str + buffer[i];
    }

    for (int i = 0; i < data_len; i = i + 8)
    {
        std::string in_hex_str;
        std::string out_hex_str;
        for (int j = 0; j < 8; j++)
        {
            in_hex_str = in_hex_str + charToHex(uint8_t(in_str[(i + j)]));
        }

        out_hex_str = enc_dec(in_hex_str, 1);
        std::string out_str;
        size_t pos = 0;
        for (int j = 0; j < 8; j++)
        {
            out_str = out_str + char(hexToInt(out_hex_str.substr(pos, 2)));
            pos = pos + 2;
        }
        outputFile.write(out_str.c_str(), out_str.size());
    }

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
