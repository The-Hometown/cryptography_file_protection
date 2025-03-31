#include "hardAESwidget.h"
#include "ui_hardAESwidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

hardAESwidget::hardAESwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hardAESwidget)
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

    unsigned char cmd[8] = {0x03, 0x05, 0x0A, 0x50, 0x55, 0x5A, 0xA0, 0xA5};
    //初始化命令
    aes_cmd.append(cmd[0]);
    encrypt_signal.append(cmd[1]);
    decrypt_signal.append(cmd[2]);
    recvd.append(cmd[3]);
    set_key.append(cmd[4]);
    send_key.append(cmd[5]);
    set_pt.append(cmd[6]);
    send_pt.append(cmd[7]);  

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

hardAESwidget::~hardAESwidget()
{
    delete ui;
}

void hardAESwidget::genpushButton_click()
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

void hardAESwidget::keypushButton_click()
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

void hardAESwidget::filepushButton_click()
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

void hardAESwidget::encryptpushButton_click()
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

        string out_str = dataProcess();
        std::ofstream outputFile(outfile_path, ifstream::binary);
        outputFile.write(out_str.c_str(), out_str.size());
        outputFile.close();

        ui->textBrowser->append("加密后的文件存放路径: " + fileName);
    }
}

void hardAESwidget::decryptpushButton_click()
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

        string out_str = dataProcess();
        std::ofstream outputFile(outfile_path, ifstream::binary);
        outputFile << "\xEF\xBB\xBF";
        outputFile.write(out_str.c_str(), out_str.size());
        outputFile.close();

        ui->textBrowser->append("解密后的文件存放路径: " + fileName);
    }
}

void hardAESwidget::backpushButton_click()
{
    emit sendsignal();
    this->close();
}

void hardAESwidget::serial_inf()
{
    serial_port->setPortName(port_name);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setBaudRate(QSerialPort::Baud115200);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->open(QIODevice::ReadWrite);

    connect(serial_port, &QSerialPort::readyRead, this, &hardAESwidget::dataRead);
}

void hardAESwidget::dataRead()
{
    readdata.append(serial_port->readAll());
}

string hardAESwidget::dataProcess()
{
    std::string out_hex_str;
    for(int i = 0; i < readdata.size(); i=i+16)
    {
        for(int j=i+15; j>=i; j--)
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

/*
string hardAESwidget::dataProcess()
{
    std::string out_hex_str;
    for(int i = readdata.size()-1; i>=0; i--)
    {
        out_hex_str = out_hex_str + charToHex(readdata.at(i));
    }

    qDebug() << "receive data:" << QString::fromStdString(out_hex_str) << endl;

    std::string out_str;
    size_t pos = 0;
    for(uint64_t i = 0; i<(out_hex_str.length()/2); i=i+1)
    {
        out_str = out_str + char(hexToInt(out_hex_str.substr(pos,2)));
        pos = pos + 2;
    }

    return out_str;
}
*/

void hardAESwidget::genKey(std::string outputname)
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

    key_str = key;

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

void hardAESwidget::getKey(std::string filename)
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
    }

    file.close();   // closed file
}

void hardAESwidget::encrypt(std::string in_hex_str)
{
    serial_port->write(aes_cmd);
    serial_port->write(encrypt_signal);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key_str));
    serial_port->write(set_key);
    serial_port->write(send_key);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(in_hex_str));
    serial_port->write(set_pt);
    serial_port->write(send_pt);
}

void hardAESwidget::decrypt(std::string in_hex_str)
{
    serial_port->write(aes_cmd);
    serial_port->write(decrypt_signal);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(key_str));
    serial_port->write(set_key);
    serial_port->write(send_key);
    serial_port->write(recvd);
    serial_port->write(stringtoarray(in_hex_str));
    serial_port->write(set_pt);
    serial_port->write(send_pt);
}

bool hardAESwidget::encrypt_file(const string filename)
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
    int n = 16 - data_len % 16;
    std::string space_str(n, ' ');
    in_str = in_str + space_str;

    string temp_str;
    for (uint64_t i = 0; i < data_len + n; i = i + 16)
    {
        string p_hex_str;
        for(uint64_t j = i; j<i+16; j++)
        {
            p_hex_str = p_hex_str + intToHex(uint8_t(in_str[(j)]));
        }
        temp_str = temp_str + "030550"+key_str+"555A50"+p_hex_str+"A0A5";

        encrypt(p_hex_str);
    }
    qDebug() << "发送给串口的待加密的数据: " << QString::fromStdString(temp_str) << endl;

    delete[] buffer;
    inputFile.close();

    return true;
}

bool hardAESwidget::decrypt_file(const string filename)
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
    for (uint64_t i = 0; i < data_len; i = i + 16)
    {
        string c_hex_str;
        for(uint64_t j=i; j<i+16; j++)
        {
            c_hex_str = c_hex_str + intToHex(uint8_t(in_str[(j)]));
        }
        temp_str = temp_str + "030A50"+key_str+"555A50"+c_hex_str+"A0A5";

        decrypt(c_hex_str);
    }
    qDebug() << "发送给串口的加密后的数据: " << QString::fromStdString(temp_str) << endl;

    delete[] buffer;
    inputFile.close();

    return true;
}

std::string hardAESwidget::charToHex(unsigned char ch)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    return ss.str();
}

QByteArray hardAESwidget::stringtoarray(string s)
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

unsigned char hardAESwidget::stringtochar(string s)
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

std::string hardAESwidget::intToHex(int value)
{
    stringstream stream;
    stream << hex << setw(2) << setfill('0') << value;
    return stream.str();
}

int hardAESwidget::hexToInt(std::string hexStr)
{
    std::stringstream stream(hexStr);
    int value;
    stream >> hex >> value;
    return value;
}
