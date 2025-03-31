/****************************************
* project name: SM4
* author: wang binbin
* completion time: 2024.3.31
*****************************************/

#include "SM4.h"


SM4::SM4() {};

std::string SM4::intToHex(int value)
{
    stringstream stream;
    stream << hex << setw(2) << setfill('0') << value;
    return stream.str();
}

int SM4::hexToInt(const std::string& hexStr)
{
    std::stringstream stream(hexStr);
    int value;
    stream >> hex >> value;
    return value;
}

void SM4::genKey(std:: string outputname)
{
    cout << "生成密钥中......" << endl;

    std::string key;

    // Generate random key
    const std::string charset = "0123456789abcdef";
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);

    for (int i = 0; i < 32; i++)
    {
        key = key + charset[dist(gen)];
    }

    Key = key;

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

void SM4::getKey(std::string filename)
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

    file.close();   // closed file
}

void SM4::splitIntoBytes(uint32_t num, uint8_t& byte1, uint8_t& byte2, uint8_t& byte3, uint8_t& byte4)
{
    byte1 = (num >> 24) & 0xFF; // Fetch highest byte
    byte2 = (num >> 16) & 0xFF; // Extract the second-highest byte
    byte3 = (num >> 8) & 0xFF;  // Extract the second lowest byte
    byte4 = num & 0xFF;         // Fetch minimum byte
}

uint32_t SM4::joinIntegers(uint8_t data[4])
{
    uint32_t rst = 0;

    // Concatenate four 8-bit integers into one 32-bit integer
    rst = rst | static_cast<uint32_t>(data[0]) << 24; // Move 24 bits to the left
    rst = rst | static_cast<uint32_t>(data[1]) << 16; // Move 16 bits to the left
    rst = rst | static_cast<uint32_t>(data[2]) << 8;  // Move 8 bits to the left
    rst = rst | static_cast<uint32_t>(data[3]);       // No need to move left

    return rst;
}

uint32_t SM4::taoFunction(uint32_t num)
{
    uint8_t data[4];
    splitIntoBytes(num, data[0], data[1], data[2], data[3]);

    for (int i = 0; i < 4; i++)
    {
        int row;
        int col;

        row = data[i] / 16;
        col = data[i] % 16;

        data[i] = Sbox[row][col];
    }

    return joinIntegers(data);
}

uint32_t SM4::leftRotate(uint32_t num, uint8_t len)
{
    return (num << len) | (num >> (32 - len));
}

uint32_t SM4::LFunction(uint32_t num)
{
    return num ^ leftRotate(num, 2) ^ leftRotate(num, 10) ^ leftRotate(num, 18) ^ leftRotate(num, 24);
}

uint32_t SM4::roundFunction(uint32_t X0, uint32_t X1, uint32_t X2, uint32_t X3, uint32_t rk)
{
    uint32_t num = X1 ^ X2 ^ X3 ^ rk;
    num = taoFunction(num);
    num = LFunction(num);

    return X0 ^ num;
}

void SM4::keyExpansion(uint32_t round_keys[32])
{
    // Decompose a string of length 32 into four strings of length 8
    std::vector<std::string> chunks;
    size_t pos = 0;
    while (pos < Key.length()) {
        chunks.push_back(Key.substr(pos, 8));
        pos = pos + 8;
    }

    uint32_t MK0 = std::stoul(chunks[0], nullptr, 16);  // Converts a hexadecimal string of length 8 to an unsigned 32-bit integer
    uint32_t MK1 = std::stoul(chunks[1], nullptr, 16);
    uint32_t MK2 = std::stoul(chunks[2], nullptr, 16);
    uint32_t MK3 = std::stoul(chunks[3], nullptr, 16);

    uint32_t K[36];
    K[0] = MK0 ^ FK[0];
    K[1] = MK1 ^ FK[1];
    K[2] = MK2 ^ FK[2];
    K[3] = MK3 ^ FK[3];

    for (int i = 0; i < 32; i++)
    {
        uint32_t num = K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i];
        num = taoFunction(num);
        num = num ^ leftRotate(num, 13) ^ leftRotate(num, 23);
        K[i + 4] = K[i] ^ num;
        round_keys[i] = K[i + 4];
    }
}

void SM4::enc_dec(uint8_t text[4][4], int model)
{
    uint32_t round_keys[32];
    keyExpansion(round_keys);

    uint32_t X[36];
    X[0] = joinIntegers(text[0]);
    X[1] = joinIntegers(text[1]);
    X[2] = joinIntegers(text[2]);
    X[3] = joinIntegers(text[3]);

    if (model == 0)     // model=0, perform encryption
    {
        for (int i = 0; i < 32; i++)
        {
            X[i + 4] = roundFunction(X[i], X[i + 1], X[i + 2], X[i + 3], round_keys[i]);
        }
    }
    else if (model == 1)      // model=1, perform decryption
    {
        for (int i = 0; i < 32; i++)
        {
            X[i + 4] = roundFunction(X[i], X[i + 1], X[i + 2], X[i + 3], round_keys[31 - i]);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        // Decomposes a 32 - bit integer into four 8 - bit integers
        splitIntoBytes(X[35-i], text[i][0], text[i][1], text[i][2], text[i][3]);
    }
}

bool SM4::encrypt_file(const string filename, const string outname)
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
    int n = 16 - data_len % 16;
    std::string space_str(n, ' ');
    in_str = in_str + space_str;

    for (int i = 0; i < data_len + n; i = i + 16)
    {
        uint8_t ptext[4][4];
        for (int j = 0; j < 4; j++)                // read by row
        {
            for (int k = 0; k < 4; k++)
            {
                ptext[j][k] = in_str[(i + j * 4 + k)];
            }
        }

        enc_dec(ptext, 0);

        std::string out_str;
        for (int j = 0; j < 4; j++)             // read by row
        {
            for (int k = 0; k < 4; k++)
            {
                out_str = out_str + char(ptext[j][k]);
            }
        }
        outputFile.write(out_str.c_str(), out_str.size());
    }

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}

bool SM4::decrypt_file(const string filename, const string outname)
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

    for (int i = 0; i < data_len; i = i + 16)
    {
        uint8_t ctext[4][4];
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ctext[j][k] = buffer[(i + j * 4 + k)];
            }
        }

        enc_dec(ctext, 1);

        std::string out_str;
        for (int j = 0; j < 4; j++)             // read by row
        {
            for (int k = 0; k < 4; k++)
            {
                out_str = out_str + char(ctext[j][k]);
            }
        }
        outputFile.write(out_str.c_str(), out_str.size());
    }

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
