/********************************
* project name: SM2
* author: wangbinbin
* completion time: 2024.4.7
********************************/

#include "SM2.h"

/*
int main()
{
    SM2 sm2;

    // sm2.genKey(PUBLIC_KEY_PATH, PRIVATE_KEY_PATH);

    // std::string m_str = "656E6372797074696F6E207374616E64617264";
    // std::string C_str = "0404ebfc718e8d1798620432268e77feb6415e2ede0e073c0f4f640ecd2e149a73e858f9d81e5430a57b36daab8f950a3c64e6ee6a63094d99283aff767e124df059983c18f809e262923c53aec295d30383b54e39d609d160afcb1908d0bd876621886ca989ca9c7d58087307ca93092d651efa";
    // std::string C_str = "04bd37cab17d367e2aa8ed26bfd54a3dc9c0d3954203e668d5cdd7ff1c6c791010f22aa15e8315d674350dcccc9c9eb10b90b95a1cc1e53e247f37d14f1ca3cbb6ca2e533a4bd2eb308aa2cc50ed60d79cfe8986828a2c3a7d3a8ac858183f43b0afac5569a807e42aebcf58b46ce7e05f9f8263";

    if (sm2.encrypt_file(PLAIN_PATH, ENCRYPTED_FILE_PATH, "pB.txt", "dA.txt"))
    {
        cout << "加密后的内容已存储到" << ENCRYPTED_FILE_PATH << "中。" << endl;
    }

    if (sm2.decrypt_file(ENCRYPTED_FILE_PATH, DECRYPTED_FILE_PATH, "dB.txt"))
    {
        cout << "解密后的内容已存储到" << DECRYPTED_FILE_PATH << "中。" << endl;
    }

    mirkill(sm2.p);     // Free the memory space occupied by the large number p
    mirkill(sm2.a);
    mirkill(sm2.b);
    mirkill(sm2.n);
    mirkill(sm2.x);
    mirkill(sm2.y);
    mirkill(sm2.xB);
    mirkill(sm2.yB);
    mirexit();     // Clear the memory used by the MIRACL library and end the use of the MIRACL library

    return 0;
}
*/

//椭圆曲线的参数
const char* p_hex_str = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
const char* a_hex_str = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
const char* b_hex_str = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
const char* n_hex_str = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";   // Order of G
const char* x_hex_str = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";   // g=(x,y)
const char* y_hex_str = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

SM3 sm3;

SM2::SM2()
{
    paraInit();   // Elliptic curve number initialization
}

void SM2::paraInit()
{
    miracl* mip = mirsys(100, 16);    // Initializes the MIRACL library
    mip->IOBASE = 16;                 // Set base to 16

    p = mirvar(0);   // Assign an initial value to the variable p
    a = mirvar(0);
    b = mirvar(0);
    n = mirvar(0);   // Order of G
    x = mirvar(0);   // g = (x,y)
    y = mirvar(0);
    cinstr(p, (char*)p_hex_str);           // Use cinstr() to assign a hexadecimal string to p
    cinstr(a, (char*)a_hex_str);
    cinstr(b, (char*)b_hex_str);
    cinstr(n, (char*)n_hex_str);
    cinstr(x, (char*)x_hex_str);
    cinstr(y, (char*)y_hex_str);

    ecurve_init(a, b, p, MR_PROJECTIVE);   // Initializes the elliptic curve

    publickey.G = epoint_init();           // Initialize point G
    epoint_set(x, y, 0, publickey.G);      // g = (x,y) is the base point G
}

void SM2::readpB(std::string filepath)
{
    std::ifstream file(filepath);          // open file
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::string line1, line2;
    if (!(std::getline(file, line1) && std::getline(file, line2)))
    {
        std::cerr << "Failed to read pB.txt!" << std::endl;
    }

    file.close();   // closed file

    miracl* mip = mirsys(100, 16);         // Initializes the MIRACL library
    mip->IOBASE = 16;                      // Set base to 16
    ecurve_init(a, b, p, MR_PROJECTIVE);   // Initializes the elliptic curve

    xB = mirvar(0);
    yB = mirvar(0);
    cinstr(xB, (char*)line1.c_str());      // Read the string into the variable xB
    cinstr(yB, (char*)line2.c_str());      // Read the string into the variable yB
    publickey.pB = epoint_init();
    epoint_set(xB, yB, 0, publickey.pB);   // Assign a value to the point pB
}

void SM2::readdA(std::string filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::string line;
    if (!(std::getline(file, line)))
    {
        std::cerr << "Failed to read dA.txt!" << std::endl;
    }

    file.close();

    miracl* mip = mirsys(100, 16);
    mip->IOBASE = 16;
    ecurve_init(a, b, p, MR_PROJECTIVE);

    k = mirvar(0);
    cinstr(k, (char*)line.c_str());
}

void SM2::readdB(std::string filepath, big& dB)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::string line;
    if (!(std::getline(file, line)))
    {
        std::cerr << "Failed to read dA.txt!" << std::endl;
    }

    file.close();

    miracl* mip = mirsys(100, 16);
    mip->IOBASE = 16;
    ecurve_init(a, b, p, MR_PROJECTIVE);

    cinstr(dB, (char*)line.c_str());
}

bool SM2::genKeys(std::string publickey_file, std::string privatekey_file)
{
    cout << "密钥生成中......" << endl;

    miracl* mip = mirsys(100, 16);
    mip->IOBASE = 16;
    ecurve_init(a, b, p, MR_PROJECTIVE);

    big private_key = mirvar(0);
    epoint* public_key;
    big x = mirvar(0);
    big y = mirvar(0);
    big max_val = mirvar(0);

    irand(time(NULL) + SEED_CONST);   // Initialize seed

    // Generate a random number ranging from 1 to  n-2
    decr(n, 2, max_val);            // max_val = n - 2
    bigrand(max_val, private_key);  // Generate a random number ranging from 0 to n-3
    incr(private_key,1,private_key);     // Add 1 to the generated random number to get a random number ranging from 1 to n-2

    public_key = epoint_init();
    ecurve_mult(private_key, publickey.G, public_key);  // public_key = [private_key]G
    epoint_get(public_key, x, y);   // Assigns the coordinates of the public key to x and y

    std::string private_key_hex_str = bigToHex(private_key);  // Converts the private key to a hexadecimal string
    std::string x_hex_str = bigToHex(x);
    std::string y_hex_str = bigToHex(y);

    // Write the generated public key to a file
    std::ofstream outputFile(publickey_file);
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file: " << publickey_file << std::endl;
        return false;
    }
    outputFile << x_hex_str << std::endl << y_hex_str << std::endl;
    outputFile.close();

    // Write the generated private key to a file
    std::ofstream outputFile1(privatekey_file);
    if (!outputFile1.is_open())
    {
        std::cerr << "Unable to open output file: " << privatekey_file << std::endl;
        return false;
    }
    outputFile1 << private_key_hex_str << std::endl;
    outputFile1.close();

    cout << "生成的公钥已写入 " << publickey_file << " 中。" << endl;
    cout << "生成的私钥已写入 " << privatekey_file << " 中。" << endl;

    mirkill(private_key);
    mirkill(x);
    mirkill(y);
    mirkill(max_val);
    epoint_free(public_key);

    return true;
}

void SM2::getPublicKey(std::string filename)
{
    std::ifstream file(filename);          // open file
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::string line1, line2;
    if (!(std::getline(file, line1) && std::getline(file, line2)))
    {
        std::cerr << "Failed to read pB.txt!" << std::endl;
    }

    file.close();   // closed file

    miracl* mip = mirsys(100, 16);         // Initializes the MIRACL library
    mip->IOBASE = 16;                      // Set base to 16
    ecurve_init(a, b, p, MR_PROJECTIVE);   // Initializes the elliptic curve

    xB = mirvar(0);
    yB = mirvar(0);
    cinstr(xB, (char*)line1.c_str());      // Read the string into the variable xB
    cinstr(yB, (char*)line2.c_str());      // Read the string into the variable yB
    publickey.pB = epoint_init();
    epoint_set(xB, yB, 0, publickey.pB);   // Assign a value to the point pB
}

void SM2::getPrivateKey(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::string line;
    if (!(std::getline(file, line)))
    {
        std::cerr << "Failed to read dA.txt!" << std::endl;
    }

    file.close();

    miracl* mip = mirsys(100, 16);
    mip->IOBASE = 16;
    ecurve_init(a, b, p, MR_PROJECTIVE);

    k = mirvar(0);
    cinstr(k, (char*)line.c_str());
}

int SM2::hexToInt(std::string hexStr)
{
    std::stringstream stream(hexStr);
    int value;
    stream >> hex >> value;
    return value;
}

std::string SM2::hexToBin(std::string hexStr)
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

std::string SM2::binToHex(std::string binary_str)
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

std::string SM2::charToHex(unsigned char ch)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    return ss.str();
}

std::string SM2::bigToBin(big x)
{
    char bytearr[32]; // An array of characters used to store the result
    big_to_bytes(32, x, bytearr, TRUE); // Convert x2 to a string of bytes, stored in bytearr

    std::string rst;
    for (int i = 0; i < 32; ++i)
    {
        rst = rst + charToHex(bytearr[i]);
    }

    return hexToBin(rst);
}

std::string SM2::bigToHex(big x)
{
    char bytearr[32];
    big_to_bytes(32, x, bytearr, TRUE);

    std::string rst;
    for (int i = 0; i < 32; ++i)
    {
        rst = rst + charToHex(bytearr[i]);
    }

    return rst;
}

std::string SM2::epointToBin(epoint* P)
{
    big x = mirvar(0);
    big y = mirvar(0);
    epoint_get(P, x, y);

    std::string x_byte_str;
    std::string y_byte_str;
    x_byte_str = bigToBin(x);
    y_byte_str = bigToBin(y);

    return x_byte_str + y_byte_str;
}

std::string SM2::intToBin(uint32_t n, int k)       // k is the width of the binary bit string
{
    std::string binaryString;

    // Iterate over each bit of the 64-bit integer
    for (int i = k - 1; i >= 0; i--)
    {
        uint32_t bit = (n >> i) & 1;            // Check if the i-th bit is set (1) or not (0)
        binaryString += std::to_string(bit);    // Append the bit to the binary string
    }
    return binaryString;
}

std::string SM2::kdfFunction(std::string Z, int klen)
{
    uint32_t cnt = 0x00000001;    // Initialize counter
    vector<std::string> Ha;
    Ha.push_back("00");    // Fill in any value at Ha[0]

    uint32_t n;
    if (klen % 256 == 0)  // n equals klen rounded up over 256
    {
        n = klen / 256;
    }
    else
    {
        n = klen / 256 + 1;
    }

    for (uint32_t i = 1; i <= n; i++)
    {
        Ha.push_back(sm3.hashCalculate(Z + intToBin(cnt, 32)));
        cnt = cnt + 1;
    }

    std::string Ha_;
    if (klen % 256 == 0)   // If klen divides 256, assign the value of Ha[n] to Ha_
    {
        Ha_ = hexToBin(Ha[n]);
    }
    else    // Otherwise, assign the value of the leftmost klen-(v*(n-1)) bit of Ha[n] to Ha_
    {
        Ha_ = hexToBin(Ha[n]).substr(0, klen - (256 * (n - 1)));
    }

    std::string K;
    for (int i = 1; i < n; i++)
    {
        K = K + Ha[i];
    }

    return hexToBin(K) + Ha_;
}

std::string SM2::encrypt(std::string m_str)
{
    miracl* mip = mirsys(100, 16);
    mip->IOBASE = 16;
    ecurve_init(a, b, p, MR_PROJECTIVE);

    std::string m_bin_str = hexToBin(m_str);
    uint64_t klen = m_bin_str.length();

    //readpB(pB_path);  // Obtain the public key of user B
    //readdA(dA_path);  // Obtain the private key of user B

    epoint* C1;
    big x1 = mirvar(0);
    big y1 = mirvar(0);


    C1 = epoint_init();
    ecurve_mult(k, publickey.G, C1);
    epoint_get(C1, x1, y1);

    std::string C1_str = "00000100" + epointToBin(C1);      // Use uncompressed mode

    epoint* C2;
    big x2 = mirvar(0);
    big y2 = mirvar(0);
    C2 = epoint_init();
    ecurve_mult(k, publickey.pB, C2);
    epoint_get(C2, x2, y2);
    std::string C2_str = epointToBin(C2);

    std::string t_bin_str = kdfFunction(C2_str, klen);
    std::string C2_xor_str;
    for (int i = 0; i < klen; i++)        // Xor m and t
    {
        if (m_bin_str[i] == t_bin_str[i])
        {
            C2_xor_str = C2_xor_str + "0";
        }
        else
        {
            C2_xor_str = C2_xor_str + "1";
        }
    }

    std::string C3_str = sm3.hashCalculate(bigToBin(x2) + m_bin_str + bigToBin(y2));

    cout << "C3_str = ";
    cout << C3_str << endl;
    // cout << "C = ";
    // cout << binToHex(C1_str) + C3_str + binToHex(C2_xor_str) << endl;

    mirkill(k);
    mirkill(x1);
    mirkill(y1);
    mirkill(x2);
    mirkill(y2);
    epoint_free(C1);
    epoint_free(C2);

    return binToHex(C1_str) + C3_str + binToHex(C2_xor_str);
}

std::string SM2::decrypt(std::string C_str)
{
    miracl* mip = mirsys(100, 16);
    mip->IOBASE = 16;
    ecurve_init(a, b, p, MR_PROJECTIVE);

    big dB = mirvar(0);
    //readdB(dB_path, dB);

    epoint* C1;
    C1 = epoint_init();
    big x1 = mirvar(0);
    big y1 = mirvar(0);
    if (C_str.substr(0, 2) == "04")    // Obtain C1 of the ciphertext
    {
        std::string temp;
        temp = C_str.substr(2, 64);
        cinstr(x1, (char*)temp.c_str());
        temp = C_str.substr(66, 64);
        cinstr(y1, (char*)temp.c_str());
    }
    epoint_set(x1, y1, 0, C1);

    epoint* P;      // Calculation process point
    big x2 = mirvar(0);
    big y2 = mirvar(0);
    P = epoint_init();
    ecurve_mult(dB, C1, P);
    epoint_get(P, x2, y2);

    int klen = (C_str.length() - 194) * 4;   // Gets the bit length of the plaintext
    std::string P_str = epointToBin(P);
    std::string t_bin_str = kdfFunction(P_str, klen);

    std::string C2_bin_str = hexToBin(C_str.substr(194));  // Obtain C2 of the ciphertext
    std::string m_bin_str;
    for (int i = 0; i < klen; i++)          // Xor C2 and t
    {
        if (C2_bin_str[i] == t_bin_str[i])
        {
            m_bin_str = m_bin_str + "0";
        }
        else
        {
            m_bin_str = m_bin_str + "1";
        }
    }

    std::string u = sm3.hashCalculate(bigToBin(x2) + m_bin_str + bigToBin(y2));

    // cout << "m = " << binToHex(m_bin_str) << endl;
    cout << "u = " << u << endl;

    mirkill(dB);
    mirkill(x1);
    mirkill(y1);
    mirkill(x2);
    mirkill(y2);
    epoint_free(C1);
    epoint_free(P);

    return binToHex(m_bin_str);
}

bool SM2::encrypt_file(std::string inputname, std::string outputname)
{
    std::ifstream inputFile(inputname, ifstream::binary);
    std::ofstream outputFile(outputname, ifstream::binary);
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

    // Converts text content to a hexadecimal string
    std::string in_hex_str;
    for (int i = 0; i < str_len; i++)
    {
        in_hex_str = in_hex_str + charToHex(buffer[i]);
    }
    std::string out_hex_str = encrypt(in_hex_str);

    std::string out_str;
    size_t pos = 0;
    for (uint64_t i = 0; i < out_hex_str.length()/2; i++)   // The boundary here is out_hex_str.length()/2, not strlen, because the plaintext and ciphertext have different lengths
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

bool SM2::decrypt_file(std::string inputname, std::string outputname)
{
    std::ifstream inputFile(inputname, ifstream::binary);
    std::ofstream outputFile(outputname, ifstream::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open input file" << std::endl;
        return false;
    }

    inputFile.seekg(0, std::ifstream::end);          // Move the file pointer to the end of the file
    std::streampos fileSize = inputFile.tellg();     // Gets the current position of the file pointer, that is, the size of the file
    inputFile.seekg(0, std::ifstream::beg);          // Move the file pointer back to the beginning of the file
    uint64_t str_len = fileSize;

    outputFile << "\xEF\xBB\xBF";                    // Set the decrypted file to UTF-8. It's important!

    char* buffer = new char[str_len];
    inputFile.read(buffer, str_len);

    // Converts text content to a hexadecimal string
    std::string in_hex_str;
    for (uint64_t i = 0; i < str_len; i++)
    {
        in_hex_str = in_hex_str + charToHex(buffer[i]);
    }
    std::string out_hex_str = decrypt(in_hex_str);

    std::string out_str;
    size_t pos = 0;
    for (uint64_t i = 0; i < out_hex_str.length()/2; i++)
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
