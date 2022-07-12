CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./ -I ../XEngine_ThirdPart/jsoncpp
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_Core -L ../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_OPenSsl -ljsoncpp
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = Protocol_Packet.o Protocol_Parse.o pch.o

ifeq ($(shell uname),Darwin)
	SHAREDLL = dylib
	SHAREFLAG = -dynamiclib
	LOADSO = 
else
	SHAREDLL = so
	SHAREFLAG = -shared
endif

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
DEBUG = 
else
FLAGS = -c
DEBUG = -g
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

ifeq ($(FILECENTOS), $(wildcard $(FILECENTOS)))
	UNICODE = -D _CENTOS
else
	UNICODE = -D _UBUNTU
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXControl_Protocol.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)


Protocol_Packet.o:./Protocol_Packet/Protocol_Packet.cpp                         
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Packet/Protocol_Packet.cpp
Protocol_Parse.o:./Protocol_Parse/Protocol_Parse.cpp                         
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Parse/Protocol_Parse.cpp
	
pch.o:./pch.cpp                          
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXControl_Protocol.$(SHAREDLL)
	cp ./libXControl_Protocol.$(SHAREDLL) ../../XEngine_Release/libXControl_Protocol.$(SHAREDLL)

CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.$(SHAREDLL)                                         
