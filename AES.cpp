/**************************************************
 * project name: AES
 * author: wang binbin
 * completion time: 2024.3.26
 *************************************************/

#include "AES.h"


int S[16][16];         // S_box
int S2[16][16];        // S_box_inverse
int word[4][44];

static const int ColM[4][4] =                // Matrix used by MixColumn
{
    2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2
};

static const int ColM2[4][4] =              // Matrix used by inverse MixColumn
{
    0x0e, 0x0b, 0x0d, 0x09,
    0x09, 0x0e, 0x0b, 0x0d,
    0x0d, 0x09, 0x0e, 0x0b,
    0x0b, 0x0d, 0x09, 0x0e
};

static const int Rcon[10] =                 //KeyExpansion wheel constant
{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

static const int Cbox[8][8] =               //Matrix used by generating S_box
{
    1,0,0,0,1,1,1,1,
    1,1,0,0,0,1,1,1,
    1,1,1,0,0,0,1,1,
    1,1,1,1,0,0,0,1,
    1,1,1,1,1,0,0,0,
    0,1,1,1,1,1,0,0,
    0,0,1,1,1,1,1,0,
    0,0,0,1,1,1,1,1
};

static const int Cbox2[8][8] =              //Matrix used by generating inverse S_box
{
    0,0,1,0,0,1,0,1,
    1,0,0,1,0,0,1,0,
    0,1,0,0,1,0,0,1,
    1,0,1,0,0,1,0,0,
    0,1,0,1,0,0,1,0,
    0,0,1,0,1,0,0,1,
    1,0,0,1,0,1,0,0,
    0,1,0,0,1,0,1,0
};

static const int XORc[8] = { 1,1,0,0,0,1,1,0 };         // Xor constant for S-box generation
static const int XORc2[8] = { 1,0,1,0,0,0,0,0 };        // Xor constant for inverse S-box generation

/**************** Function definition ********************/
AES::AES()
{
    genSbox(S);
    genSbox2(S2);
};

void AES::genKey(std::string outputname)
{
    cout << "生成密钥中......" << endl;

    std::string key;

    const std::string charset = "0123456789abcdef";
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);

    for (int i = 0; i < 32; i++)
    {
        key = key + charset[dist(gen)];
    }

    Key = key;

    // Write the generated key and iv to a file
    std::ofstream outputFile(outputname);
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file: " << outputname << std::endl;
    }
    outputFile << key << std::endl;
    outputFile.close();
    cout << "生成的密钥已写入 " << outputname << " 中。" << endl;
}

void AES::getKey(std::string filename)
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
    keyExpansion(word);
}

int AES::gfMul(int n, int m)
{
    bitset<8> a(n);
    bitset<8> b(m);

    int nrr[8];                  // store n in binary

    int brr[8];                  // store binary before m shift
    int arr[8] = { 0 };          // store m-shifted binary
    int temp[8];

    int c[8] = { 0,0,0,1,1,0,1,1 };
    int result = 0;

    for (int i = 0; i < 8; i++)
    {
        brr[7 - i] = b[i];
        nrr[7 - i] = a[i];
    }

    for (int i = 0; i < 7; i++)
    {
        int cnt = 0;

        for (int i = 0; i < 8; i++)
        {
            temp[i] = brr[i];
        }

        if (nrr[i] == 1)
        {
            cnt = 7 - i;
        }

        while (cnt--)
        {
            int flag = temp[0];

            for (int i = 1; i < 8; i++)
            {
                temp[i - 1] = temp[i];
            }
            temp[7] = 0;

            if (flag)
            {
                for (int i = 0; i < 8; i++)
                {
                    temp[i] = temp[i] ^ c[i];
                }
            }
        }

        if (nrr[i] == 1)
        {
            for (int i = 0; i < 8; i++)
            {
                arr[i] = arr[i] ^ temp[i];
            }
        }

    }

    if (nrr[7] == 1)
    {
        for (int i = 0; i < 8; i++)
        {
            arr[i] = arr[i] ^ brr[i];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (arr[i] == 1)
        {
            result = result + pow(2, 7 - i);
        }
    }

    return result;
}

int AES::invElement(int m)
{
    int x = 0;
    for (int i = 1; i < 256; i++)
    {
        if (gfMul(i, m) == 1)
        {
            x = i;
            break;
        }
    }

    return x;
}

void AES::genSbox(int box[16][16])
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            box[i][j] = i * 16 + j;
        }
    }


    for (int j = 1; j < 16; j++)
    {
        box[0][j] = invElement(box[0][j]);
    }

    for (int i = 1; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            box[i][j] = invElement(box[i][j]);
        }
    }

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            int result = 0;
            bitset<8> b(box[i][j]);
            int brr[8] = { 0 };

            for (int i = 0; i < 8; i++)
            {
                for (int k = 0; k < 8; k++)
                {
                    brr[i] = brr[i] ^ (Cbox[i][k] * b[k]);
                }
            }

            for (int i = 0; i < 8; i++)
            {
                brr[i] = brr[i] ^ XORc[i];
            }

            for (int i = 0; i < 8; i++)
            {
                if (brr[i] == 1)
                {
                    result = result + pow(2, i);
                }
            }

            box[i][j] = result;
        }
    }
}

void AES::genSbox2(int box[16][16])
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            box[i][j] = i * 16 + j;
        }
    }

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            int result = 0;
            bitset<8> b(box[i][j]);
            int brr[8] = { 0 };

            for (int i = 0; i < 8; i++)
            {
                for (int k = 0; k < 8; k++)
                {
                    brr[i] = brr[i] ^ (Cbox2[i][k] * b[k]);
                }
            }

            for (int i = 0; i < 8; i++)
            {
                brr[i] = brr[i] ^ XORc2[i];
            }

            for (int i = 0; i < 8; i++)
            {
                if (brr[i] == 1)
                {
                    result = result + pow(2, i);
                }
            }

            box[i][j] = invElement(result);
        }
    }
}


void AES::leftByte(int word[4])
{
    int arr[4];

    for (int i = 0; i < 4; i++)
    {
        arr[i] = word[i];
    }

    for (int i = 0; i < 4; i++)
    {
        word[i] = arr[(i + 1) % 4];
    }
}

void AES::gFunction(int word[4], int round)
{
    leftByte(word);

    int row;
    int col;

    for (int i = 0; i < 4; i++)
    {
        row = word[i] / 16;
        col = word[i] % 16;

        word[i] = S[row][col];
    }

    word[0] = word[0] ^ Rcon[round];
}

void AES::keyExpansion(int word[4][44])
{
    int key[4][4];

    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::string str;
            str = str + Key[cnt];
            str = str + Key[cnt + 1];
            cnt = cnt + 2;
            key[i][j] = hexToInt(str);
        }
    }


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            word[i][j] = key[i][j];
        }
    }

    for (int i = 4, j = 0; i < 44; i++)
    {
        if (i % 4 == 0)
        {
            int arr[4];

            for (int k = 0; k < 4; k++)
            {
                arr[k] = word[k][i - 1];
            }

            gFunction(arr, j);

            for (int k = 0; k < 4; k++)
            {
                word[k][i] = word[k][i - 4] ^ arr[k];
            }

            j++;
        }
        else
        {
            for (int k = 0; k < 4; k++)
            {
                word[k][i] = word[k][i - 4] ^ word[k][i - 1];
            }
        }
    }
}

void AES::subBytes(int m[4][4])
{
    int row;
    int col;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            row = m[i][j] / 16;
            col = m[i][j] % 16;

            m[i][j] = S[row][col];
        }
    }
}

void AES::subBytes2(int m[4][4])
{
    int row;
    int col;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            row = m[i][j] / 16;
            col = m[i][j] % 16;

            m[i][j] = S2[row][col];
        }
    }
}

void AES::shiftRow(int m[4][4])
{
    int p[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[i][j] = m[i][j];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = p[i][(j + i) % 4];
        }
    }
}

void AES::shiftRow2(int m[4][4])
{
    int p[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[i][j] = m[i][j];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int temp = j - i;
            if (temp < 0)
            {
                temp = temp + 4;
            }
            m[i][j] = p[i][temp];
        }
    }

}

void AES::mixColumn(int m[4][4])
{
    int p[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[i][j] = m[i][j];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                m[i][j] = m[i][j] ^ gfMul(ColM[i][k], p[k][j]);
            }

        }
    }
}

void AES::mixColumn2(int m[4][4])
{
    int p[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[i][j] = m[i][j];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                m[i][j] = m[i][j] ^ gfMul(ColM2[i][k], p[k][j]);
            }

        }
    }
}

void AES::addRoundKey(int m[4][4], int key[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = m[i][j] ^ key[i][j];
        }
    }

}

std::string AES::intToHex(int value)
{
    stringstream stream;
    stream << hex << setw(2) << setfill('0') << value;
    return stream.str();
}

int AES::hexToInt(const std::string& hexStr)
{
    std::stringstream stream(hexStr);
    int value;
    stream >> hex >> value;
    return value;
}

void AES::encrypt(int ptext[4][4])
{
    int KeyRound[4][4];
    for (int j = 0; j < 4; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            KeyRound[k][j] = word[k][j];
        }
    }

    addRoundKey(ptext, KeyRound);

    for (int r = 1; r <= 9; r++)
    {
        subBytes(ptext);
        shiftRow(ptext);
        mixColumn(ptext);

        for (int j = r * 4, l = 0; j < r * 4 + 4; j++, l++)
        {
            for (int k = 0; k < 4; k++)
            {
                KeyRound[k][l] = word[k][j];
            }
        }

        addRoundKey(ptext, KeyRound);
    }

    subBytes(ptext);
    shiftRow(ptext);
    for (int j = 10 * 4, l = 0; j < 10 * 4 + 4; j++, l++)
    {
        for (int k = 0; k < 4; k++)
        {
            KeyRound[k][l] = word[k][j];
        }
    }
    addRoundKey(ptext, KeyRound);
}

void AES::decrypt(int ctext[4][4])
{
    int KeyRound[4][4];
    for (int j = 10 * 4, l = 0; j < 10 * 4 + 4; j++, l++)
    {
        for (int k = 0; k < 4; k++)
        {
            KeyRound[k][l] = word[k][j];
        }
    }
    addRoundKey(ctext, KeyRound);

    for (int r = 9; r >= 1; r--)
    {
        subBytes2(ctext);
        shiftRow2(ctext);
        mixColumn2(ctext);

        for (int j = r * 4, l = 0; j < r * 4 + 4; j++, l++)
        {
            for (int k = 0; k < 4; k++)
            {
                KeyRound[k][l] = word[k][j];
            }
        }

        mixColumn2(KeyRound);
        addRoundKey(ctext, KeyRound);
    }

    subBytes2(ctext);
    shiftRow2(ctext);
    for (int j = 0; j < 4; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            KeyRound[k][j] = word[k][j];
        }
    }
    addRoundKey(ctext, KeyRound);
}

bool AES::encrypt_file(const string filename, const string outname)
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
        int ptext[4][4];
        for (int j = 0; j < 4; j++)                // read by row
        {
            for (int k = 0; k < 4; k++)
            {
                ptext[k][j] = uint8_t(in_str[(i + j * 4 + k)]);
            }
        }

        encrypt(ptext);

        std::string out_str;
        for (int j = 0; j < 4; j++)             // read by row
        {
            for (int k = 0; k < 4; k++)
            {
                out_str = out_str + char(ptext[k][j]);
            }
        }
        outputFile.write(out_str.c_str(), out_str.size());
    }

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}

bool AES::decrypt_file(const string filename, const string outname)
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

    for (int i = 0; i < data_len; i = i + 16)
    {
        int ctext[4][4];
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ctext[k][j] = uint8_t(in_str[(i + j * 4 + k)]);
            }
        }

        decrypt(ctext);

        std::string out_str;
        for (int j = 0; j < 4; j++)             // read by row
        {
            for (int k = 0; k < 4; k++)
            {
                out_str = out_str + char(ctext[k][j]);
            }
        }
        outputFile.write(out_str.c_str(), out_str.size());
    }

    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
