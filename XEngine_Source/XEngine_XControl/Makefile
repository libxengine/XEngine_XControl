CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_DownLoad -L ../XControl_Infomation -L ../XControl_Configure -L ../XControl_Protocol
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lXClient_Socket -lHelpComponents_XLog -lXEngine_ProcFile -lXEngine_SystemApi -lRfcComponents_HttpServer -lRfcComponents_EmailClient -lNetHelp_APIHelp -lXEngine_Download -lXControl_Infomation -lXControl_Configure -lXControl_Protocol -ljsoncpp
LIBEX = -static-libgcc -ldl -lrt -lpthread 
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = XContral_Config.o XContral_Handle.o XContral_Task.o XContral_Network.o XContral_Process.o XContral_RPCTask.o RPCTask_CalMethod.o XEngine_XControl.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_XControlApp $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
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
XContral_RPCTask.o:./XContral_RPCTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_RPCTask.cpp

RPCTask_CalMethod.o:./XContral_RPCTask/RPCTask_CalMethod.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XContral_RPCTask/RPCTask_CalMethod.cpp

XEngine_XControl.o:./XEngine_XControl.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_XControl.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_XControlApp
	cp -f ./XEngine_XControlApp ../../XEngine_Release/XEngine_XControlApp
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_XContralApp
