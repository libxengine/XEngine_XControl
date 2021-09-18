# XContral

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
8. support user writer xrpc protocol to process events

#### Installation

#### Requirements  
support system above windows 7sp1 and linux(ubuntu20.04,centos8)  
XEngine need V7.20 or above  
vcpkg need 2021.05.11 or above  

#### Windows
use vs2019 x86(debug or release) x64(only release) open and complie  
You need to configure the environment in the following way, otherwise you may need to set the library directory in the project yourself  

##### install Dependent library
vcpkg:https://github.com/microsoft/vcpkg   
vcpkg.exe install jsoncpp  
vcpkg integrate install  
you can not use vcpkg and config jsoncpp development for youself   

##### XEngine
download xengine through git.read xengine project readme file to configure your env

#### Linux
Linux use Makefile to complie  
supproted ubuntu x64 or centos x64     

##### install Dependent library
install jsoncpp to your system  
ubuntu20.04  
sudo apt install libjsoncpp-devel  
Centos8.x  
sudo dnf install jsoncpp-devel  

##### XEngine Install
you can install xengine env to your system by shell   
like this:sudo XEngine_LINEnv.sh -i 3  
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
5.  change to your config file and email config Manage_EMail.ini  
6.  -c parament create a encrypto email info file  
7.  run..  

## directory struct
- XEngine_Docment   docment directory
- XEngine_Release   install directory
- XEngine_Source    code    directory
- XEngine_APPClient example client

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
user custom protocol start  

## Clean up statement
the software will not generate any files to other directories in your system.  
the software can achieve complete cleanup by directly delete the directory.  
the software will not modify any of your system configuration.  

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## post issues

if you have eny quest.post issues...
