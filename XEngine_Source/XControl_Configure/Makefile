CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = 
LIB = -ljsoncpp
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=,--disable-new-dtags
LIBINCLUDE = Configure_Json.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXControl_Configure.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)


Configure_Json.o:./Configure_Json/Configure_Json.cpp                         
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Configure_Json/Configure_Json.cpp

pch.o:./pch.cpp                          
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXControl_Configure.so
	cp ./libXControl_Configure.so ../../XEngine_Release/libXControl_Configure.so


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.so                                         
