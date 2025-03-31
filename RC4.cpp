/**************************************************
 * project name: RC4
 * author: wang binbin
 * completion time: 2024.3.30
 *************************************************/

#include "RC4.h"


unsigned char Sbox[256];
unsigned char Sbox2[256];           // Back up the initialized S-box

RC4::RC4() {};

void RC4::genKey(std::string outputname)
{
    cout << "生成密钥中......\n";

    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> length_dist(16, 256);
    std::uniform_int_distribution<int> char_dist(0, sizeof(charset) - 2);

    int length = length_dist(gen);       // Generates a random length
    std::string random_key;

    for (int i = 0; i < length; ++i) {
        random_key = random_key + charset[char_dist(gen)];
    }

    Key = random_key;

    // Writes the key to a file
    std::ofstream outputFile(outputname);
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file: " << outputname << std::endl;
    }
    outputFile << random_key << std::endl;
    outputFile.close();
    cout << "生成的密钥已写入 " << outputname << " 中。" << endl;
}

void RC4::getKey(std::string filename)
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

void RC4::swap(unsigned char& a, unsigned char& b)
{
    unsigned char tmp = a;
    a = b;
    b = tmp;
}

void RC4::initSbox()
{
    unsigned char Tbox[256];
    int key_len = Key.length();

    // Example Initialize the Sbox
    for (int i = 0; i < 256; i++)
    {
        Sbox[i] = i;
        Tbox[i] = Key[i % key_len];
    }

    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + Sbox[i] + Tbox[i]) % 256;
        swap(Sbox[i], Sbox[j]);
    }

    // Backup initialized S-box for decryption. It's important!
    for (int i = 0; i < 256; i++)
    {
        Sbox2[i] = Sbox[i];
    }
}

void RC4::enc_dec(char* data, long long data_len, int model)
{
    unsigned char k;
    unsigned char t;
    int i = 0;
    int j = 0;

    if (model == 0)
    {
        for (long long h = 0; h < data_len; h++)
        {
            i = (i + 1) % 256;
            j = (j + Sbox[i]) % 256;
            swap(Sbox[i], Sbox[j]);
            t = (Sbox[i] + Sbox[j]) % 256;
            k = Sbox[t];
            data[h] = data[h] ^ k;
        }
    }
    else if(model == 1)
    {
        for (long long h = 0; h < data_len; h++)
        {
            i = (i + 1) % 256;
            j = (j + Sbox2[i]) % 256;
            swap(Sbox2[i], Sbox2[j]);
            t = (Sbox2[i] + Sbox2[j]) % 256;
            k = Sbox2[t];
            data[h] = data[h] ^ k;
        }
    }
}

bool RC4::enc_dec_file(const string filename, const string outname, int model)
{
    initSbox();
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
    long long data_len = fileSize;

    if (model == 1)
    {
        outputFile << "\xEF\xBB\xBF";                    // Set the decrypted file to UTF-8. It's important!
    }
    //outputFile << "\xEF\xBB\xBF";                     // Note: Do not set the encrypted format to UTF-8, otherwise the garbled code will be decrypted

    char* buffer = new char[data_len];
    inputFile.read(buffer, data_len);
    streamsize ret = inputFile.gcount();

    enc_dec(buffer, data_len, model);
    outputFile.write(buffer, ret);        // Writes the encrypted data to the output file

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
