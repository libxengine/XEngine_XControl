[中文](README.md) ||  [English](README.en.md)  
# XEngine_XControl
本仓库有开发和主分支,如果要使用,请使用master分支下的代码

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

## 安装教程

#### XEngine环境
必须安装XEngine,版本需要V7.38或者以上版本,安装XEngine可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

##### 快速部署
git clone https://gitee.com/xyry/libxengine.git 或者 git clone https://github.com/libxengine/xengine.git  
window执行XEngine_WINEnv.bat 脚本.  
Linux执行:sudo ./XEngine_LINEnv.sh -i 3  
macos执行:./XEngine_LINEnv.sh -i 3  

#### Windows
使用VS打开并且编译,支持WINDOWS 7SP1以上系统  
直接运行即可

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  
在控制台运行

#### MacOS
使用makefile编译,控制台运行,需要mac 12以及以上版本  
在控制台运行

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
5.  运行  

## 目录结构
- XEngine_Docment   文档目录结构  
- XEngine_Release   安装目录结构  
- XEngine_Source    源代目录结构  

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
信息报告  
用户自定义开始  

## 开发计划
远程录音录像  
远程桌面,摄像头监控  
优化代码  
起始序列号设置  

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request  

## 提交问题

如果你有问题,可以在issues中提交  
