#include"RSA.h"


RSA::RSA() {};

bool RSA::isPrime(bm::int1024_t n)
{
    br::mt11213b BRNG(time(nullptr));

    if (miller_rabin_test(n, 25, BRNG))
    {
        if (miller_rabin_test((n - 1) / 2, 25, BRNG))
        {
            return true;
        }
    }
    return false;
}

bm::int1024_t RSA::getPrime()
{
    bm::int1024_t prime;

    // Creates a pseudo-random number generator object and initializes it with the current time as the seed value
    br::mt19937 BRNG(time(nullptr));

    // Creates a uniformly distributed integer generator dist that generates random integers in a specified range.
    br::uniform_int_distribution<bm::int1024_t> dist(bm::int1024_t(0), bm::int1024_t(1) << SIZE);        // Generates a binary number of SIZE

    while (!isPrime(prime = dist(BRNG)));
    return prime;
}

bm::int1024_t RSA::getKey_n(bm::int1024_t prime1, bm::int1024_t prime2)
{
    return prime1 * prime2;
}

bm::int1024_t RSA::getFn(bm::int1024_t p, bm::int1024_t q)
{
    return (p - 1) * (q - 1);
}

bm::int1024_t RSA::getKey_e(bm::int1024_t fn)
{
    bm::int1024_t key_e;
    br::mt11213b BRNG(time(nullptr));
    br::uniform_int_distribution<bm::int1024_t> dist(bm::int1024_t(0), (bm::int1024_t(fn)));   // Generates a random integer between 0 and fn

    do {
        key_e = dist(BRNG);
    } while (key_e < 2 || getGcd(key_e, fn) != 1);

    return key_e;
}

void RSA::exGcd(bm::int1024_t a, bm::int1024_t b, bm::int1024_t* x, bm::int1024_t* y)
{
    if (b == 0)
    {
        *x = 1;
        *y = 0;
        return;
    }

    exGcd(b, a % b, x, y);
    bm::int1024_t tmp = *x;
    *x = *y;
    *y = tmp - a / b * (*y);
}

bm::int1024_t RSA::getKey_d(bm::int1024_t key_e, bm::int1024_t fn)
{
    bm::int1024_t x;
    bm::int1024_t y;

    exGcd(key_e, fn, &x, &y);

    return (x % fn + fn) % fn;          // Transform so that the key is equivalent to a smaller number
}

void RSA::genKeys(std::string publickey_file, std::string privatekey_file)
{
    cout << "生成密钥中......\n";

    bm::int1024_t p, q = getPrime();
    while ((p = getPrime()) == q);
    mKey.mkey_n = getKey_n(p, q);

    bm::int1024_t fn = getFn(p, q);
    mKey.mkey_e = getKey_e(fn);
    mKey.mkey_d = getKey_d(mKey.mkey_e, fn);

    // Write the generated public key to a file
    std::ofstream outputFile(publickey_file);
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file: " << publickey_file << std::endl;
    }
    outputFile << mKey.mkey_n << std::endl << mKey.mkey_e << std::endl;
    outputFile.close();

    // Write the generated private key to a file
    std::ofstream outputFile1(privatekey_file);
    if (!outputFile1.is_open())
    {
        std::cerr << "Unable to open output file: " << privatekey_file << std::endl;
    }
    outputFile1 << mKey.mkey_n << std::endl << mKey.mkey_d << std::endl;
    outputFile1.close();

    cout << "生成的公钥已写入 " << publickey_file << " 中。" << endl;
    cout << "生成的公钥已写入 " << privatekey_file << " 中。" << endl;
}

void RSA::getPublicKey(std::string filename)
{
    std::ifstream file(filename);          // open file
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filename << "!" << std::endl;
    }
    else
    {
        std::string secret_key[2];
        if (!(std::getline(file, secret_key[0]) && std::getline(file, secret_key[1])))
        {
            std::cerr << "Failed to read " << filename << "!" << std::endl;
        }
        mKey.mkey_n = bm::int1024_t(secret_key[0]);
        mKey.mkey_e = bm::int1024_t(secret_key[1]);
    }

    file.close();   // closed file
}

void RSA::getPrivateKey(std::string filename)
{
    std::ifstream file(filename);          // open file
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filename << "!" << std::endl;
    }
    else
    {
        std::string secret_key[2];
        if (!(std::getline(file, secret_key[0]) && std::getline(file, secret_key[1])))
        {
            std::cerr << "Failed to read " << filename << "!" << std::endl;
        }
        mKey.mkey_n = bm::int1024_t(secret_key[0]);
        mKey.mkey_d = bm::int1024_t(secret_key[1]);
    }

    file.close();   // closed file
}

bm::int1024_t RSA::getGcd(bm::int1024_t n1, bm::int1024_t n2)
{
    bm::int1024_t num;
    while ((num = n1 % n2))
    {
        n1 = n2;
        n2 = num;
    }
    return n2;
}

bm::int1024_t RSA::fastExp(bm::int1024_t data, bm::int1024_t key, bm::int1024_t key_n)
{
    bm::int1024_t rst = 1;
    for (; key; key >>= 1)      // key moves one digit to the right each time
    {
        if (key & 1)
        {
            rst = (rst * data) % key_n;
        }
        data = (data * data) % key_n;
    }
    return rst;
}

bool RSA::encrypt_file(const string filename, const string outname)
{
    std::ifstream inputFile(filename, ifstream::binary);
    std::ofstream outputFile(outname, ifstream::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open input file" << std::endl;
        return false;
    }

    char* buffer = new char[NUMBER];
    bm::int1024_t* bufferOut = new bm::int1024_t[NUMBER];
    while (!inputFile.eof())
    {
        inputFile.read(buffer, NUMBER);
        streamsize ret = inputFile.gcount();
        for (streamsize i = 0; i < ret; ++i)
        {
            bufferOut[i] = fastExp(buffer[i], mKey.mkey_e, mKey.mkey_n);
        }
        outputFile.write((char*)bufferOut, ret * sizeof(bm::int1024_t));
    }

    delete[] bufferOut;
    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}

bool RSA::decrypt_file(const string filename, const string outname)
{
    std::ifstream inputFile(filename, ifstream::binary);
    std::ofstream outputFile(outname, ifstream::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open input file" << std::endl;
        return false;
    }

    bm::int1024_t* buffer = new bm::int1024_t[NUMBER];    // input buffer
    char* bufferOut = new char[NUMBER];                   // output buffer
    while (!inputFile.eof())
    {
        inputFile.read((char*)buffer, NUMBER * sizeof(bm::int1024_t));
        streamsize ret = inputFile.gcount() / sizeof(bm::int1024_t);
        for (streamsize i = 0; i < ret; ++i)
        {
            bufferOut[i] = (char)fastExp(buffer[i], mKey.mkey_d, mKey.mkey_n);
        }
        outputFile.write(bufferOut, ret);
    }

    delete[] bufferOut;
    delete[] buffer;
    inputFile.close();
    outputFile.close();

    return true;
}
