# 文件软硬件加解密平台

#### 介绍
本平台为用户提供密码软硬件的文件加解密人机交互。  在软件文件加解密中，实现对称密码 AES、 DES、 3DES、国密 SM4 密码算法， 实现流密码 RC4、 ZUC 密码算法，实现杂凑算法 SM3 以及非对称密码算法 RSA 和国密算法 SM2。在硬件（FPGA）实现对称密码 AES、DES 和 3DES 算法密码核，基于串口通信将硬件加密集成于平台，实现硬件文件加密与用户交互。

#### 软件架构
软件架构说明
软件首先进行了密码算法后端代码的开发，开发的软件密码算法包括AES、DES，3DES、RSA、RC4、ZUC、SM2、SM3、SM4。硬件密码核包括AES、DES、3DES。
接着在Qt Creator进行前端设计。


#### 软硬件支持

1.  下载Visual Studio 2019
2.  下载Qt Creator 5.14.2
3.  下载Vivado 2021 （如需使用硬件加解密需要下载，不使用则无需下载）
4.  准备一块BASYS 3的FPGA开发板。（如需使用硬件加解密需要准备，不使用则无需准备）

#### 使用说明

1.  将仓库中的所有文件下载到一个文件夹中，路径中不要包含中文
2.  下载boost_1_58_0
3.  双击.pro文件在Qt Creator中打开项目，在项目中添加如下两行代码：

```
QT       += serialport
INCLUDEPATH += boost_1_58_0的路径，路径加双引号
```
4. 利用编译器x86-windows-msvc2019-pe-64bit进行构建即可
注意：若遇到编译器x86-windows-msvc2019-pe-64bit不可用，解决方法：配置构建套件（kit），双击Desktop（x86-windows-msvc2019-pe-64bit），在Qt version中选择Qt 5.14.2 MSVC201764bit。即可使用编译器进行构建。

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
