CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_DownLoad -L ../XContral_Infomation -L ../XContral_Configure
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_OPenSsl -lXClient_Socket -lHelpComponents_XLog -lXEngine_ProcFile -lXEngine_SystemApi -lRfcComponents_EmailClient -lNetHelp_APIHelp -lXEngine_Download -lXContral_Infomation -lXContral_Configure -ljsoncpp
LIBEX = -static-libgcc -ldl -lrt -lpthread 
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = XContral_Config.o XContral_Handle.o XContral_Task.o XContral_Network.o XContral_Process.o XEngine_XContral.o

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

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_XContralApp $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
XContral_Config.o:./XContral_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_Config.cpp 
XContral_Handle.o:./XContral_Handle.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_Handle.cpp
XContral_Task.o:./XContral_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_Task.cpp
XContral_Network.o:./XContral_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_Network.cpp
XContral_Process.o:./XContral_Process.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_Process.cpp
	
XEngine_XContral.o:./XEngine_XContral.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_XContral.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_XContralApp
	cp -f ./XEngine_XContralApp ../../XEngine_Release/XEngine_XContralApp
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_XContralApp
