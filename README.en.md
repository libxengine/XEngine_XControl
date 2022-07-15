[中文](README.md) ||  [English](README.en.md)  
# XEngine_XControl
This repository has a development and master branch. If you want to use it, please use the master branch

#### Development language
c c++  

#### Description
Computer process deamon service. Remote control software  
Lightweight background service management tool,control your computer with HTTP or TCP protocol  

#### Software feature
1. Support the collection of computer information and send it to your email  
2. Support HTTP request to specify the address task  
3. Support a variety of task lists and custom tasks  
4. Support some advanced features  
5. Support HTTP task acceptance processing  
6. support connect protocol for tcp and udp  
7. support deamon local service and process and Strategy control

#### Installation

#### XEngine Evn
you must install XEngine,need V7.38 or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

##### fast to deployment 
git clone https://gitee.com/xyry/libxengine.git or git clone https://github.com/libxengine/xengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### Windows
use vs open and complie,support windows 7sp1 and above   
Just Run it.

#### Linux
Linux use Makefile to complie  
supproted ubuntu x64 or centos x64  

#### MacOS
Linux use Makefile to complie,terminal exection,Requires mac 12 and above  

##### complie
execute command in XEngine_Source path
make complie  
make FLAGS=InstallAll install  
make FLAGS=CleanAll clear  

#### use

1.  Switch to the MASTER branch
2.  download code
3.  complie
4.  install
5.  run..  

## directory struct
- XEngine_Docment   docment directory
- XEngine_Release   install directory
- XEngine_Source    code    directory

## system task list
nothing to do...   
delete file  
delete dir  
upload a file to ftp  
Request a list of files in the folder  
execute program  
pop message  
terminal process  
remote shutdown computre  
execute command  
connenct  
information report
user custom protocol start  

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## post issues

if you have eny question.post issues...
