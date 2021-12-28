# XContral

#### 开发语言
c c++  

#### 介绍
计算机进程守护服务.远程控制软件  
轻量级后台服务管理工具,通过HTTP或者TCP协议来控制你的计算机

#### 软件特性
1. 支持收集计算机信息并且发送到你的邮件  
2. 支持HTTP请求指定地址任务    
3. 支持多种多样的任务列表和自定义任务  
4. 支持一些高级特性  
5. 支持HTTP任务接受处理  
6. 支持TCP,UDP连接协议控制  
7. 支持守护本地服务和程序以及策略控制  
8. 支持用户编写PRC来处理事件  

## 安装教程

#### 版本需求
支持WINDOWS 7SP1和LINUX(UBUNT20.04,CENTOS8)以上系统  
XEngine版本需要V7.21或者以上版本  
vcpkg 需要2021.05.11以上版本  

#### Windows
使用VS2019 x86(debug release) x64(release)打开并且编译  
你需要按照下面的方式配置环境,不然你可能需要自己在项目中设置库目录  

##### 三方库环境
需要使用VCPKG安装环境.代码地址:https://github.com/microsoft/vcpkg  
安装好后你可能需要把它集成到你的VS中  
然后命令安装环境: vcpkg.exe install jsoncpp   
集成VS环境: vcpkg integrate install  
vcpkg 主要为了方便安装jsoncpp,如果你想自己配置jsoncpp的环境,可以不使用vcpkg...  

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  

##### 三方库环境
基于不同系统,可能命令不同,比如在Ubuntu下面  
sudo apt install libjsoncpp-devel  
Centos8.x  
sudo dnf install jsoncpp-devel  

#### XEngine环境
XEngine通过GIT下载,参考XEngine项目的readme配置环境  
##### 编译命令
在XEngine_Source目录下执行命令
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

#### 使用说明

1.  切换到MASTER分支
2.  下载代码
3.  编译
4.  安装
5.  修改配置文件和电子邮件配置XControl_EMail.ini  
6.  -c 参数创建加密邮件信息  
7.  运行  

## 目录结构
- XEngine_Docment   文档目录结构  
- XEngine_Release   安装目录结构  
- XEngine_Source    源代目录结构  
- XEngine_APPClient 示例客户端

## 系统任务列表
没有需要执行的任务  
下载并且运行一个程序  
删除指定文件  
删除指定目录  
上传一个文件到指定FTP  
请求获取文件夹中的文件列表  
执行指定程序  
弹出指定消息  
结束指定进程  
远程关闭计算机  
执行命令  
连接  
用户自定义开始  

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request  

## 提交问题

如果你有问题,可以在issues中提交  
