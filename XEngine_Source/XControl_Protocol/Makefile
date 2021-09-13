CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_Core
LIB = -lXEngine_OPenSsl -ljsoncpp
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=,--disable-new-dtags
LIBINCLUDE = Protocol_Packet.o Protocol_Parse.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXControl_Protocol.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)


Protocol_Packet.o:./Protocol_Packet/Protocol_Packet.cpp                         
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Packet/Protocol_Packet.cpp
Protocol_Parse.o:./Protocol_Parse/Protocol_Parse.cpp                         
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Protocol_Parse/Protocol_Parse.cpp
	
pch.o:./pch.cpp                          
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXControl_Protocol.so
	cp ./libXControl_Protocol.so ../../XEngine_Release/libXControl_Protocol.so

CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.so                                         
