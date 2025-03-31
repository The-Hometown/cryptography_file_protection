/**************************************************
 * project name: ZUC
 * author: wang binbin
 * completion time: 2024.4.11
 *************************************************/

#include "ZUC.h"


// S0_box, S0_box = S2_box
static const uint8_t S0_box[16][16] =
{
    {0x3e, 0x72, 0x5b, 0x47, 0xca, 0xe0, 0x00, 0x33, 0x04, 0xd1, 0x54, 0x98, 0x09, 0xb9, 0x6d, 0xcb},
    {0x7b, 0x1b, 0xf9, 0x32, 0xaf, 0x9d, 0x6a, 0xa5, 0xb8, 0x2d, 0xfc, 0x1d, 0x08, 0x53, 0x03, 0x90},
    {0x4d, 0x4e, 0x84, 0x99, 0xe4, 0xce, 0xd9, 0x91, 0xdd, 0xb6, 0x85, 0x48, 0x8b, 0x29, 0x6e, 0xac},
    {0xcd, 0xc1, 0xf8, 0x1e, 0x73, 0x43, 0x69, 0xc6, 0xb5, 0xbd, 0xfd, 0x39, 0x63, 0x20, 0xd4, 0x38},
    {0x76, 0x7d, 0xb2, 0xa7, 0xcf, 0xed, 0x57, 0xc5, 0xf3, 0x2c, 0xbb, 0x14, 0x21, 0x06, 0x55, 0x9b},
    {0xe3, 0xef, 0x5e, 0x31, 0x4f, 0x7f, 0x5a, 0xa4, 0x0d, 0x82, 0x51, 0x49, 0x5f, 0xba, 0x58, 0x1c},
    {0x4a, 0x16, 0xd5, 0x17, 0xa8, 0x92, 0x24, 0x1f, 0x8c, 0xff, 0xd8, 0xae, 0x2e, 0x01, 0xd3, 0xad},
    {0x3b, 0x4b, 0xda, 0x46, 0xeb, 0xc9, 0xde, 0x9a, 0x8f, 0x87, 0xd7, 0x3a, 0x80, 0x6f, 0x2f, 0xc8},
    {0xb1, 0xb4, 0x37, 0xf7, 0x0a, 0x22, 0x13, 0x28, 0x7c, 0xcc, 0x3c, 0x89, 0xc7, 0xc3, 0x96, 0x56},
    {0x07, 0xbf, 0x7e, 0xf0, 0x0b, 0x2b, 0x97, 0x52, 0x35, 0x41, 0x79, 0x61, 0xa6, 0x4c, 0x10, 0xfe},
    {0xbc, 0x26, 0x95, 0x88, 0x8a, 0xb0, 0xa3, 0xfb, 0xc0, 0x18, 0x94, 0xf2, 0xe1, 0xe5, 0xe9, 0x5d},
    {0xd0, 0xdc, 0x11, 0x66, 0x64, 0x5c, 0xec, 0x59, 0x42, 0x75, 0x12, 0xf5, 0x74, 0x9c, 0xaa, 0x23},
    {0x0e, 0x86, 0xab, 0xbe, 0x2a, 0x02, 0xe7, 0x67, 0xe6, 0x44, 0xa2, 0x6c, 0xc2, 0x93, 0x9f, 0xf1},
    {0xf6, 0xfa, 0x36, 0xd2, 0x50, 0x68, 0x9e, 0x62, 0x71, 0x15, 0x3d, 0xd6, 0x40, 0xc4, 0xe2, 0x0f},
    {0x8e, 0x83, 0x77, 0x6b, 0x25, 0x05, 0x3f, 0x0c, 0x30, 0xea, 0x70, 0xb7, 0xa1, 0xe8, 0xa9, 0x65},
    {0x8d, 0x27, 0x1a, 0xdb, 0x81, 0xb3, 0xa0, 0xf4, 0x45, 0x7a, 0x19, 0xdf, 0xee, 0x78, 0x34, 0x60}
};

// S1_box, S1_box = S3_box
static const uint8_t S1_box[16][16] =
{
    {0x55, 0xc2, 0x63, 0x71, 0x3b, 0xc8, 0x47, 0x86, 0x9f, 0x3c, 0xda, 0x5b, 0x29, 0xaa, 0xfd, 0x77},
    {0x8c, 0xc5, 0x94, 0x0c, 0xa6, 0x1a, 0x13, 0x00, 0xe3, 0xa8, 0x16, 0x72, 0x40, 0xf9, 0xf8, 0x42},
    {0x44, 0x26, 0x68, 0x96, 0x81, 0xd9, 0x45, 0x3e, 0x10, 0x76, 0xc6, 0xa7, 0x8b, 0x39, 0x43, 0xe1},
    {0x3a, 0xb5, 0x56, 0x2a, 0xc0, 0x6d, 0xb3, 0x05, 0x22, 0x66, 0xbf, 0xdc, 0x0b, 0xfa, 0x62, 0x48},
    {0xdd, 0x20, 0x11, 0x06, 0x36, 0xc9, 0xc1, 0xcf, 0xf6, 0x27, 0x52, 0xbb, 0x69, 0xf5, 0xd4, 0x87},
    {0x7f, 0x84, 0x4c, 0xd2, 0x9c, 0x57, 0xa4, 0xbc, 0x4f, 0x9a, 0xdf, 0xfe, 0xd6, 0x8d, 0x7a, 0xeb},
    {0x2b, 0x53, 0xd8, 0x5c, 0xa1, 0x14, 0x17, 0xfb, 0x23, 0xd5, 0x7d, 0x30, 0x67, 0x73, 0x08, 0x09},
    {0xee, 0xb7, 0x70, 0x3f, 0x61, 0xb2, 0x19, 0x8e, 0x4e, 0xe5, 0x4b, 0x93, 0x8f, 0x5d, 0xdb, 0xa9},
    {0xad, 0xf1, 0xae, 0x2e, 0xcb, 0x0d, 0xfc, 0xf4, 0x2d, 0x46, 0x6e, 0x1d, 0x97, 0xe8, 0xd1, 0xe9},
    {0x4d, 0x37, 0xa5, 0x75, 0x5e, 0x83, 0x9e, 0xab, 0x82, 0x9d, 0xb9, 0x1c, 0xe0, 0xcd, 0x49, 0x89},
    {0x01, 0xb6, 0xbd, 0x58, 0x24, 0xa2, 0x5f, 0x38, 0x78, 0x99, 0x15, 0x90, 0x50, 0xb8, 0x95, 0xe4},
    {0xd0, 0x91, 0xc7, 0xce, 0xed, 0x0f, 0xb4, 0x6f, 0xa0, 0xcc, 0xf0, 0x02, 0x4a, 0x79, 0xc3, 0xde},
    {0xa3, 0xef, 0xea, 0x51, 0xe6, 0x6b, 0x18, 0xec, 0x1b, 0x2c, 0x80, 0xf7, 0x74, 0xe7, 0xff, 0x21},
    {0x5a, 0x6a, 0x54, 0x1e, 0x41, 0x31, 0x92, 0x35, 0xc4, 0x33, 0x07, 0x0a, 0xba, 0x7e, 0x0e, 0x34},
    {0x88, 0xb1, 0x98, 0x7c, 0xf3, 0x3d, 0x60, 0x6c, 0x7b, 0xca, 0xd3, 0x1f, 0x32, 0x65, 0x04, 0x28},
    {0x64, 0xbe, 0x85, 0x9b, 0x2f, 0x59, 0x8a, 0xd7, 0xb0, 0x25, 0xac, 0xaf, 0x12, 0x03, 0xe2, 0xf2}
};

// Constant used to initialize LSFR S
static const std::string d[16] =
{
    "100010011010111", "010011010111100", "110001001101011", "001001101011110", "101011110001001", "011010111100010", "111000100110101", "000100110101111",
    "100110101111000", "010111100010011", "110101111000100", "001101011110001", "101111000100110", "011110001001101", "111100010011010", "100011110101100"
};

ZUC::ZUC() {};

int ZUC::hexToInt(std::string hexStr)
{
    std::stringstream stream(hexStr);
    int value;
    stream >> hex >> value;
    return value;
}

std::string ZUC::charToHex(unsigned char ch)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    return ss.str();
}

uint32_t ZUC::binToInt32(std::string bin_str)
{
    std::bitset<32> bits(bin_str);                // Create a bitset with the binary string
    return static_cast<uint32_t>(bits.to_ulong());     // Convert the bitset to an unsigned long and then to uint32_t
}

std::string ZUC::hexToBin(std::string hexStr)
{
    std::string binStr;
    for (char hexChar : hexStr) {
        switch (hexChar) {
        case '0': binStr += "0000"; break;
        case '1': binStr += "0001"; break;
        case '2': binStr += "0010"; break;
        case '3': binStr += "0011"; break;
        case '4': binStr += "0100"; break;
        case '5': binStr += "0101"; break;
        case '6': binStr += "0110"; break;
        case '7': binStr += "0111"; break;
        case '8': binStr += "1000"; break;
        case '9': binStr += "1001"; break;
        case 'A': case 'a': binStr += "1010"; break;
        case 'B': case 'b': binStr += "1011"; break;
        case 'C': case 'c': binStr += "1100"; break;
        case 'D': case 'd': binStr += "1101"; break;
        case 'E': case 'e': binStr += "1110"; break;
        case 'F': case 'f': binStr += "1111"; break;
        default:
            std::cerr << "Invalid hex character: " << hexChar << std::endl;
            return ""; // If an invalid hexadecimal character is entered, an empty string is returned
        }
    }
    return binStr;
}

std::string ZUC::binToHex(const std::string bin_str)
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

std::string ZUC::int32ToHex(uint32_t value)
{
    stringstream stream;
    stream << hex << setw(8) << setfill('0') << value;
    return stream.str();
}

uint32_t ZUC::mod_add(uint32_t a, uint32_t b)
{
    uint32_t c = a + b;
    c = (c & 0x7fffffff) + (c >> 31);

    return c;
}

uint32_t mod_2expk_mul(uint32_t x, int k)
{
    return ((x << k) | (x >> (31 - k))) & 0x7FFFFFFF;
}

void ZUC::genKey(std::string outputname)
{
    cout << "密钥生成中......" << endl;

    // Generate random key and iv
    const std::string charset = "0123456789abcdef";
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);
    std::uniform_int_distribution<int> dist1(0, 1);           // Randomly generate '0' and '1'

    // Randomly generate input parameters and CK
    for (int i = 0; i < 8; i++)
    {
        COUNT = COUNT + charset[dist(gen)];
    }

    BEARER = BEARER + charset[dist1(gen)];
    BEARER = BEARER + charset[dist(gen)];

    DIRECTION = charset[dist1(gen)];

    for (int i = 0; i < 32; i++)
    {
        CK = CK + charset[dist(gen)];
    }

    // Write the generated key to a file
    std::ofstream outputFile(outputname);
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file: " << outputname << std::endl;
    }
    outputFile << COUNT << std::endl << BEARER << std::endl << DIRECTION << std::endl << CK << std::endl;
    outputFile.close();
    cout << "生成的密钥已写入 " << outputname << " 中。" << endl;
}

void ZUC::getKey(std::string filename)
{
    std::ifstream file(filename);          // open file
    if (!file.is_open())     // If the file exists, a key file is automatically generated
    {
        std::cerr << "Failed to open file " << filename << "!" << std::endl;
    }
    else
    {   // Read the key parameters and CK directly from the file
        if (!(std::getline(file, COUNT) && std::getline(file, BEARER) && std::getline(file, DIRECTION) && std::getline(file, CK)))
        {
            std::cerr << "Failed to read " << filename << "!" << std::endl;
        }
    }
    file.close();   // closed file
}

void ZUC::lfsrWithInitialisationMode(uint32_t u)
{
    uint32_t v = LFSR_S[0];
    v = mod_add(v, mod_2expk_mul(LFSR_S[15], 15));
    v = mod_add(v, mod_2expk_mul(LFSR_S[13], 17));
    v = mod_add(v, mod_2expk_mul(LFSR_S[10], 21));
    v = mod_add(v, mod_2expk_mul(LFSR_S[4], 20));
    v = mod_add(v, mod_2expk_mul(LFSR_S[0], 8));
    LFSR_S[16] = mod_add(v,u);
    if (LFSR_S[16] == 0)
    {
        LFSR_S[16] = 0x7FFFFFFF;
    }

    for (int i = 0; i < 16; i++)
    {
        LFSR_S[i] = LFSR_S[i + 1];
    }
}

void ZUC::lfsrWithWorkMode()
{
    LFSR_S[16] = LFSR_S[0];
    LFSR_S[16] = mod_add(LFSR_S[16], mod_2expk_mul(LFSR_S[15], 15));
    LFSR_S[16] = mod_add(LFSR_S[16], mod_2expk_mul(LFSR_S[13], 17));
    LFSR_S[16] = mod_add(LFSR_S[16], mod_2expk_mul(LFSR_S[10], 21));
    LFSR_S[16] = mod_add(LFSR_S[16], mod_2expk_mul(LFSR_S[4], 20));
    LFSR_S[16] = mod_add(LFSR_S[16], mod_2expk_mul(LFSR_S[0], 8));
    if (LFSR_S[16] == 0)
    {
        LFSR_S[16] = 0x7FFFFFFF;
    }

    for (int i = 0; i < 16; i++)
    {
        LFSR_S[i] = LFSR_S[i + 1];
    }
}

void ZUC::bitReconstruction()
{
    BR_X[0] = ((LFSR_S[15] << 1) & 0xFFFF0000) | (LFSR_S[14] & 0x0000FFFF);    // Combine the high 16 bits of LFSR S[15] and the low 16 bits of LFSR S[14]
    BR_X[1] = ((LFSR_S[11] << 16) & 0xFFFF0000) | ((LFSR_S[9] >> 15) & 0x0000FFFF);
    BR_X[2] = ((LFSR_S[7] << 16) & 0xFFFF0000) | ((LFSR_S[5] >> 15) & 0x0000FFFF);
    BR_X[3] = ((LFSR_S[2] << 16) & 0xFFFF0000) | ((LFSR_S[0] >> 15) & 0x0000FFFF);
}

uint32_t ZUC::leftRotate(uint32_t num, uint8_t len)
{
    return (num << len) | (num >> (32 - len));
}

uint32_t ZUC::L1Function(uint32_t X)
{
    return X ^ leftRotate(X, 2) ^ leftRotate(X, 10) ^ leftRotate(X, 18) ^ leftRotate(X, 24);
}

uint32_t ZUC::L2Function(uint32_t X)
{
    return X ^ leftRotate(X, 8) ^ leftRotate(X, 14) ^ leftRotate(X, 22) ^ leftRotate(X, 30);
}

uint32_t ZUC::subWord(uint32_t num)
{
    uint8_t bytearr[4];
    bytearr[0] = (num >> 24) & 0xFF;
    bytearr[1] = (num >> 16) & 0xFF;
    bytearr[2] = (num >> 8) & 0xFF;
    bytearr[3] = num & 0xFF;

    int row;
    int col;
    for (int i = 0; i < 4; i++)
    {
        row = bytearr[i] / 16;
        col = bytearr[i] % 16;

        if (i == 0 || i == 2)
        {
            bytearr[i] = S0_box[row][col];
        }
        else
        {
            bytearr[i] = S1_box[row][col];
        }
    }

    uint32_t result = 0;
    result = result | static_cast<uint32_t>(bytearr[0]) << 24; // Move bytearr[0] 24 bits to the left
    result = result | static_cast<uint32_t>(bytearr[1]) << 16; // Move bytearr[1] 16 bits to the left
    result = result | static_cast<uint32_t>(bytearr[2]) << 8;  // Move bytearr[2] 8 bits to the left
    result = result | static_cast<uint32_t>(bytearr[3]);       // No need to move left

    return result;
}

uint32_t ZUC::fFunction(uint32_t X0, uint32_t X1, uint32_t X2)
{
    uint32_t W = ((X0 ^ R1) + R2) % 0x100000000;
    uint32_t W1 = (R1 + X1) % 0x100000000;
    uint32_t W2 = R2 ^ X2;
    R1 = subWord(L1Function(((W1 << 16) & 0xFFFF0000) | ((W2 >> 16) & 0x0000FFFF)));
    R2 = subWord(L2Function(((W2 << 16) & 0xFFFF0000) | ((W1 >> 16) & 0x0000FFFF)));

    return W;
}

void ZUC::initializeLFSR_S()
{
    std::string k[16];
    std::string IV[16];

    size_t pos = 0;
    for (int i = 0; i < 16; i++)
    {
        k[i] = hexToBin(CK.substr(pos, 2));
        pos = pos + 2;
    }

    pos = 0;
    for (int i = 0; i < 4; i++)
    {
        IV[i] = hexToBin(COUNT.substr(pos, 2));
        pos = pos + 2;
    }

    pos = 3;
    IV[4] = hexToBin(BEARER).substr(pos) + DIRECTION + "00";
    IV[5] = "00000000";
    IV[6] = "00000000";
    IV[7] = "00000000";

    for (int i = 0; i < 8; i = i+2)
    {
        IV[i + 8] = IV[i];
        IV[i + 9] = IV[i + 1];
    }

    for (int i = 0; i < 16; i++)
    {
        LFSR_S[i] = binToInt32(k[i] + d[i] + IV[i]);
    }
}

void ZUC::initialisationMode()
{
    initializeLFSR_S();
    R1 = 0;
    R2 = 0;
    uint32_t W;
    for (int i = 0; i < 32; i++)
    {
        bitReconstruction();
        W = fFunction(BR_X[0], BR_X[1], BR_X[2]);
        lfsrWithInitialisationMode(W >> 1);
    }
}

std::string ZUC::workMode(int L)
{
    std::string hex_str;

    bitReconstruction();
    fFunction(BR_X[0], BR_X[1], BR_X[2]);
    lfsrWithWorkMode();
    uint32_t Z;
    for (int i = 0; i < L; i++)
    {
        bitReconstruction();
        Z = fFunction(BR_X[0], BR_X[1], BR_X[2]) ^ BR_X[3];
        hex_str = hex_str + int32ToHex(Z);
        lfsrWithWorkMode();
    }

    return hexToBin(hex_str);
}

std::string ZUC::enc_dec(std::string in_hex_str)
{
    initialisationMode();

    std::string in_bin_str = hexToBin(in_hex_str);
    int L = in_bin_str.length() / 32;
    if (in_bin_str.length() % 32 != 0)
    {
        L = L + 1;
    }

    std::string k_bin_str = workMode(L);

    std::string out_bin_str;
    for (int i = 0; i < in_bin_str.length(); i++)
    {
        if (in_bin_str[i] == k_bin_str[i])
        {
            out_bin_str = out_bin_str + "0";
        }
        else
        {
            out_bin_str = out_bin_str + "1";
        }
    }

    // cout << binToHex(out_bin_str);

    return binToHex(out_bin_str);
}

bool ZUC::enc_dec_file(std::string filename, std::string outname, int model)
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

    if (model == 1)
    {
        outputFile << "\xEF\xBB\xBF";
    }

    char* buffer = new char[data_len];
    inputFile.read(buffer, data_len);

    std::string in_hex_str;
    for (int i = 0; i < data_len; i++)      // Converts the contents of a file to a binary string
    {
        in_hex_str = in_hex_str + charToHex(buffer[i]);
    }
    std::string out_hex_str = enc_dec(in_hex_str);

    std::string out_str;
    size_t pos = 0;
    for (uint64_t i = 0; i < data_len; i++)
    {
        out_str = out_str + char(hexToInt(out_hex_str.substr(pos, 2)));
        pos = pos + 2;
    }

    outputFile.write(out_str.c_str(), out_str.size());        // Writes the encrypted data to the output file

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
