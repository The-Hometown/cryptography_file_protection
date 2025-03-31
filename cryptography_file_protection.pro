#----------------------------------------
#
# Project created by QtCreator 2024-4-17
#
#----------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Additional include directory
INCLUDEPATH += "F:\Projects\software and hardware encryption platform\cryptography file protection\boost_1_58_0"
INCLUDEPATH += "F:\Projects\software and hardware encryption platform\cryptography file protection\MIRACL-master"

# 链接库
#LIBS += -L"F:\Projects\software and hardware encryption platform\miracl\miracl.lib" -lmiracl
#LIBS += -L"F:\Projects\software and hardware encryption platform\cryptography file protection\miracl\miracl.lib" -lmiracl


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AES.cpp \
    AESwidget.cpp \
    DES.cpp \
    DESwidget.cpp \
    RC4.cpp \
    RC4widget.cpp \
    RSA.cpp \
    RSAwidget.cpp \
    SM3.cpp \
    SM3widget.cpp \
    SM4.cpp \
    SM4widget.cpp \
    TDEA.cpp \
    TDEAwidget.cpp \
    ZUC.cpp \
    ZUCwidget.cpp \
    hardAESwidget.cpp \
    hardDESwidget.cpp \
    hardTDEAwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AES.h \
    AESwidget.h \
    DES.h \
    DESwidget.h \
    RC4.h \
    RC4widget.h \
    RSA.h \
    RSAwidget.h \
    SM3.h \
    SM3widget.h \
    SM4.h \
    SM4widget.h \
    TDEA.h \
    TDEAwidget.h \
    ZUC.h \
    ZUCwidget.h \
    hardAESwidget.h \
    hardDESwidget.h \
    hardTDEAwidget.h \
    mainwindow.h \
    miracl.h \
    mirdef.h

FORMS += \
    AESwidget.ui \
    DESwidget.ui \
    RC4widget.ui \
    RSAwidget.ui \
    SM2widget.ui \
    SM3widget.ui \
    SM4widget.ui \
    TDEAwidget.ui \
    ZUCwidget.ui \
    hardAESwidget.ui \
    hardDESwidget.ui \
    hardTDEAwidget.ui \
    mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    miracl.lib \
    miracl.pdb
