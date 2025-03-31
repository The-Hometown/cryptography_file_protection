#include "hardTDEAwidget.h"
#include "ui_hardTDEAwidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

hardTDEAwidget::hardTDEAwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hardTDEAwidget)
{
    ui->setupUi(this);

    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")   // 解决中文乱码问题
    #endif

    //设置串口
    serial_port = new QSerialPort();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            port_name = serial.portName();
            serial.close();
        }
    }

    unsigned char cmd[10] = {0xA3, 0x05, 0x0A, 0x0F, 0x50, 0x55, 0x5A, 0x5F, 0xF0, 0xF5};
    //初始化命令
    des3_cmd.append(cmd[0]);
    encrypt_signal.append(cmd[1]);
    decrypt_signal.append(cmd[2]);
    recvd.append(cmd[3]);
    set_key1.append(cmd[4]);
    set_key2.append(cmd[5]);
    set_key3.append(cmd[6]);
    set_key.append(cmd[7]);
    //send_key.append(cmd[5]);
    set_pt.append(cmd[8]);
    send_pt.append(cmd[9]);

    serial_inf();

    ui->progressBar->setRange(0, 100); // 设置进度条的范围从0到100
    ui->progressBar->setValue(0);   // 初始化进度条的值为0
    //设置槽
    connect(ui->genpushButton, SIGNAL(clicked()), this, SLOT(genpushButton_click()));
    connect(ui->keypushButton, SIGNAL(clicked(bool)),this,SLOT(keypushButton_click()));
    connect(ui->encryptpushButton, SIGNAL(clicked(bool)),this,SLOT(encryptpushButton_click()));
    connect(ui->decryptpushButton, SIGNAL(clicked(bool)),this,SLOT(decryptpushButton_click()));
    connect(ui->filepushButton, SIGNAL(clicked(bool)),this,SLOT(filepushButton_click()));
    connect(ui->backpushButton, SIGNAL(clicked(bool)),this,SLOT(backpushButton_click()));
}

hardTDEAwidget::~hardTDEAwidget()
{
    delete ui;
}

void hardTDEAwidget::genpushButton_click()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/key.txt";
        QFile file(fileName);
        std::string filePathStr = fileName.toStdString();
        genKey(filePathStr);

        // 更新进度条的值
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "密钥生成成功！");
        ui->textBrowser->append("生成的密钥存储在: " + fileName);
    }
}

void hardTDEAwidget::keypushButton_click()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "Text files (*.txt)");
    if (!filePath.isEmpty())
    {
        ui->progressBar->setValue(0);
        key_path = filePath.toStdString();
        ui->keylineEdit->setText(filePath);
        getKey(key_path);
        ui->textBrowser->append("密钥文件: " + filePath);
    }
    qDebug() << "key_str:" << QString::fromStdString(key_str) <<endl;
}

void hardTDEAwidget::filepushButton_click()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "Text files (*.txt)");
    if (!filePath.isEmpty())
    {
        ui->progressBar->setValue(0);
        infile_path = filePath.toStdString();
        ui->filelineEdit->setText(filePath);
        ui->textBrowser->append("选择文件: " + filePath);
    }
}

void hardTDEAwidget::encryptpushButton_click()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/encrypted_file.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        encrypt_file(infile_path);

        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件加密成功！");
        //QMessageBox::information(this, "Success", "文件加密成功！");
        //QMessageBox::information(this, "Success", "文件加密成功！");


        string out_str = dataProcess();
        std::ofstream outputFile(outfile_path, ifstream::binary);
        outputFile.write(out_str.c_str(), out_str.size());
        outputFile.close();

        ui->textBrowser->append("加密后的文件存放路径: " + fileName);
    }
}

void hardTDEAwidget::decryptpushButton_click()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/decrypted_file.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        decrypt_file(infile_path);

        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件解密成功！");
        //QMessageBox::information(this, "Success", "文件解密成功！");
        //QMessageBox::information(this, "Success", "文件解密成功！");

        string out_str = dataProcess();
        std::ofstream outputFile(outfile_path, ifstream::binary);
        outputFile << "\xEF\xBB\xBF";
        outputFile.write(out_str.c_str(), out_str.size());
        outputFile.close();

        ui->textBrowser->append("解密后的文件存放路径: " + fileName);
    }
}

void hardTDEAwidget::backpushButton_click()
{
    emit sendsignal();
    this->close();
}

void hardTDEAwidget::serial_inf()
{
    serial_port->setPortName(port_name);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setBaudRate(QSerialPort::Baud115200);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->open(QIODevice::ReadWrite);

    connect(serial_port, &QSerialPort::readyRead, this, &hardTDEAwidget::dataRead);
}

void hardTDEAwidget::dataRead()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    readdata.append(serial_port->readAll());
}

string hardTDEAwidget::dataProcess()
{
    std::string out_hex_str;
    for(int i = 0; i < readdata.size(); i=i+8)
    {
        for(int j=i+7; j>=i; j--)
        {
            out_hex_str = out_hex_str + charToHex(readdata.at(j));
        }
    }

    qDebug() << "串口返回经过数据处理后的数据:" << QString::fromStdString(out_hex_str) << endl;

    std::string out_str;
    size_t pos = 0;
    for(uint64_t i = 0; i<(out_hex_str.length()/2); i=i+1)
    {
        out_str = out_str + char(hexToInt(out_hex_str.substr(pos,2)));
        pos = pos + 2;
    }

    return out_str;
}

void hardTDEAwidget::genKey(std::string outputname)
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

    key_str = key;
    key1_str = key.substr(0, 16);
    key2_str = key.substr(16, 32);
    key3_str = key.substr(32);

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


void hardTDEAwidget::getKey(std::string filename)
{
    std::ifstream file(filename);          // open file
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filename << "!" << std::endl;
    }
    else
    {
        if (!(std::getline(file, key_str)))
        {
            std::cerr << "Failed to read " << filename << "!" << std::endl;
        }
        key1_str = key_str.substr(0, 16);
        key2_str = key_str.substr(16, 32);
        key3_str = key_str.substr(32);
    }

    file.close();   // closed file
}

void hardTDEAwidget::encrypt(std::string in_hex_str)
{
    serial_port->write(des3_cmd);
    serial_port->write(encrypt_signal);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key1_str));
    serial_port->write(set_key1);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key2_str));
    serial_port->write(set_key2);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key3_str));
    serial_port->write(set_key3);
    serial_port->write(set_key);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(in_hex_str));
    //serial_port->write(set_key);
    //serial_port->write(send_key);
    serial_port->write(set_pt);
    serial_port->write(send_pt);
}

void hardTDEAwidget::decrypt(std::string in_hex_str)
{
    serial_port->write(des3_cmd);
    serial_port->write(decrypt_signal);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key1_str));
    serial_port->write(set_key1);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key2_str));
    serial_port->write(set_key2);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key3_str));
    serial_port->write(set_key3);
    //serial_port->write(stringtoarray(key_str));
    serial_port->write(set_key);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(in_hex_str));
    //serial_port->write(send_key);
    serial_port->write(set_pt);
    serial_port->write(send_pt);
}

bool hardTDEAwidget::encrypt_file(const string filename)
{
    readdata.clear();

    std::ifstream inputFile(filename, ifstream::binary);
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

    string temp_str;
    for (uint64_t i = 0; i < data_len + n; i = i + 8)
    {
        string p_hex_str;
        for(uint64_t j=i; j<i+8; j++)
        {
            p_hex_str = p_hex_str + intToHex(uint8_t(in_str[(j)]));
        }
        temp_str = temp_str + "A3050F"+key1_str+"500F"+key2_str+"550F"+key3_str+"5A5F0F"+p_hex_str+"F0F5";

        encrypt(p_hex_str);
    }
    qDebug() << "发送给串口的待加密的数据: " << QString::fromStdString(temp_str) << endl;

    delete[] buffer;
    inputFile.close();

    return true;
}

bool hardTDEAwidget::decrypt_file(const string filename)
{
    readdata.clear();

    std::ifstream inputFile(filename, ifstream::binary);
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

    string temp_str = "";
    for (uint64_t i = 0; i < data_len; i = i + 8)
    {
        string c_hex_str;
        for(uint64_t j=i; j<i+8; j++)
        {
            c_hex_str = c_hex_str + intToHex(uint8_t(in_str[(j)]));
        }
        temp_str = temp_str + "A30A0F"+key1_str+"500F"+key2_str+"550F"+key3_str+"5A5F0F"+c_hex_str+"F0F5";

        decrypt(c_hex_str);
    }
    qDebug() << "发送给串口的加密后的数据: " << QString::fromStdString(temp_str) << endl;

    delete[] buffer;
    inputFile.close();

    return true;
}

std::string hardTDEAwidget::charToHex(unsigned char ch)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    return ss.str();
}

QByteArray hardTDEAwidget::stringtoarray(string s)
{
    QByteArray array;
    unsigned char a,b,c;
    string r,t;

    for(unsigned long long i=0;i<s.length();)
    {
        t = s[i];
        r = s[i+1];
        a = stringtochar(t);
        b = stringtochar(r);
        c = (a<<4) | b;
        array.append(c);
        i=i+2;
    }
    return array;
}

unsigned char hardTDEAwidget::stringtochar(string s)
{
     unsigned char ch;
     if(s=="a") ch=0x0a;
     else if(s=="b") ch=0x0b;
     else if(s=="c") ch=0x0c;
     else if(s=="d") ch=0x0d;
     else if(s=="e") ch=0x0e;
     else if(s=="f") ch=0x0f;
     else if(s=="0") ch=0x00;
     else if(s=="1") ch=0x01;
     else if(s=="2") ch=0x02;
     else if(s=="3") ch=0x03;
     else if(s=="4") ch=0x04;
     else if(s=="5") ch=0x05;
     else if(s=="6") ch=0x06;
     else if(s=="7") ch=0x07;
     else if(s=="8") ch=0x08;
     else if(s=="9") ch=0x09;

     return ch;
}

std::string hardTDEAwidget::intToHex(int value)
{
    stringstream stream;
    stream << hex << setw(2) << setfill('0') << value;
    return stream.str();
}

int hardTDEAwidget::hexToInt(std::string hexStr)
{
    std::stringstream stream(hexStr);
    int value;
    stream >> hex >> value;
    return value;
}
