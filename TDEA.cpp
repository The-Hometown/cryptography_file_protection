/**************************************************
 * project name: 3DES
 * author: wang binbin
 * completion time: 2024.4.16
 *************************************************/


#include "TDEA.h"


DES des;   // Generate a global DES object

TDEA::TDEA() {};

std::string TDEA::charToHex(unsigned char ch)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    return ss.str();
}

int TDEA::hexToInt(string hex_str)
{
    std::stringstream stream(hex_str);
    int value;
    stream >> hex >> value;
    return value;
}

void TDEA::genKey(std::string outputname)
{
    cout << "生成密钥中......" << endl;

    std::string key;

    const std::string charset = "0123456789abcdef";
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);

    for (int i = 0; i < 48; i++)            // Generate random key
    {
        key = key + charset[dist(gen)];
    }

    Key = key;
    key1 = key.substr(0, 16);
    key2 = key.substr(16, 32);
    key3 = key.substr(32);

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

void TDEA::getKey(std::string filename)
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
        key1 = Key.substr(0, 16);
        key2 = Key.substr(16, 32);
        key3 = Key.substr(32);
    }

    file.close();   // closed file
}

bool TDEA::encrypt_file(const string filename, const string outname)
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
            in_hex_str = in_hex_str + charToHex(uint8_t(in_str[(i + j)]));
        }

        // Encrypt: C = E(key3, D(key2, E(key1,P)))
        des.setKey(key1);
        out_hex_str = des.enc_dec(in_hex_str, 0);
        des.setKey(key2);
        out_hex_str = des.enc_dec(out_hex_str, 1);
        des.setKey(key3);
        out_hex_str = des.enc_dec(out_hex_str, 0);

        std::string out_str;
        size_t pos = 0;
        for (int j = 0; j < 8; j++)
        {
            out_str = out_str + char(hexToInt(out_hex_str.substr(pos, 2)));
            pos = pos + 2;
        }
        outputFile.write(out_str.c_str(), out_str.size());        // Write the ciphertext to a file
    }

    delete[] buffer;    // Free the dynamic pointer space
    inputFile.close();
    outputFile.close();

    return true;
}

bool TDEA::decrypt_file(const string filename, const string outname)
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

        // Decrypt: P = D(key1, E(key2, D(key3,C)))
        des.setKey(key3);
        out_hex_str = des.enc_dec(in_hex_str, 1);
        des.setKey(key2);
        out_hex_str = des.enc_dec(out_hex_str, 0);
        des.setKey(key1);
        out_hex_str = des.enc_dec(out_hex_str, 1);

        std::string out_str;
        size_t pos = 0;
        for (int j = 0; j < 8; j++)
        {
            out_str = out_str + char(hexToInt(out_hex_str.substr(pos, 2)));
            pos = pos + 2;
        }
        outputFile.write(out_str.c_str(), out_str.size());   // Writes the decrypted plaintext to the file
    }

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
