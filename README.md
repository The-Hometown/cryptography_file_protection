# 文件软硬件加解密平台

#### 介绍
本平台为用户提供了密码软硬件结合的文件加解密人机交互。下述所有密码算法的实现均未调用密码算法第三方库。均为原创自主编写。在软件文件加解密中，实现了对称密码 AES、DES、3DES、国密SM4密码算法，实现了流密码 RC4、ZUC 密码算法，实现了杂凑算法 SM3 以及非对称密码算法 RSA 和国密算法 SM2。此外，在硬件（FPGA）实现了对称密码 AES、 DES 和 3DES 算法密码核，基于串口通信将硬件加密集成于本平台，实现硬件文件加密与用户交互。用户可选择软硬件不同的方法对文件实现加解密。平台集成多种经典密码算法， 可以有效的保护文件的机密性和完整性，同时操作简单，便于用户的使用。

#### 软件架构
软件架构说明 软件首先进行了密码算法后端代码的开发，开发的软件密码算法包括 AES、DES，3DES、RSA、RC4、ZUC、SM2、SM3、SM4。硬件密码核包括 AES、DES、3DES。 接着在 Qt Creator 进行前端设计。


#### 软硬件支持

1. 依赖开发环境 Visual Studio 2019（需要配置组件".NET 桌面开发"、"使用C++的桌面开发"）
2. 依赖开发环境 Qt Creator 5.14.2
3. 依赖开发环境 Vivado 2021 （如需使用硬件加解密需要下载，不使用则无需下载）
4. 准备一块 BASYS 3 的 FPGA 开发板。（如需使用硬件加解密需要准备，不使用则无需准备）

#### 使用说明

1. 将仓库中的所有文件下载到一个文件夹中，路径中不要包含中文
2. 下载 boost_1_58_0
3. 双击.pro文件在 Qt Creator 中打开项目，在项目中添加如下两行代码：

```
QT       += serialport
INCLUDEPATH += boost_1_58_0的路径，路径加双引号
```

4. 打开 Qt Creator，点击“文件”，打开文件或项目，选中 .pro.user 文件即可添加项目。

5. 配置构建套件，点击 Qt Creator 左边索引栏的“项目”，点击 Manage Kits，点击默认的那个套件，然后选择编译器，在Compiler 中选择 C：Microsoft Visual C++ Compiler (x86)，C++：Microsoft Visual C++ Compiler (x86)，应用保存。在项目 Build & Run 中，添加当前的默认套件，点击构建运行即可。

本人目前构建套件的情况如下，可供大家参考：

第一种：点击 Qt Creator 左边索引栏的“项目”，点击 Manage Kits，点击 Desktop Qt 5.14.2 MSVC2017 32bit，在 Compiler 中选择 C：Microsoft Visual C++ Compiler (x86)，C++：Microsoft Visual C++ Compiler (x86)，应用保存。在项目 Build & Run 中，添加 Desktop Qt 5.14.2 MSVC2017 32bit，点击构建运行即可。

第二种：点击 Qt Creator 左边索引栏的“项目”，点击 Manage Kits，点击 Desktop（x86-windows-msvc2019-pe-64bit），在 Compiler 中选择 C：Microsoft Visual C++ Compiler (x86)，C++：Microsoft Visual C++ Compiler (x86)，应用保存。在项目 Build & Run 中，添加 Desktop（x86-windows-msvc2019-pe-64bit），点击构建运行即可。注意：若遇到套件 Desktop（x86-windows-msvc2019-pe-64bit） 不可用，解决方法：配置构建套件（kit），双击 Desktop（x86-windows-msvc2019-pe-64bit），在 Qt version 中选择 Qt 5.14.2 MSVC2017 64bit。即可使用编译器Desktop（x86-windows-msvc2019-pe-64bit）进行构建。

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
