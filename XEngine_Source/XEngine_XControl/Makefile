CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
INCLUDE = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_DownLoad \
          -L ../XControl_Infomation -L ../XControl_Configure -L ../XControl_Protocol \
		  -L ../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_OPenSsl -lXClient_Socket -lHelpComponents_XLog -lXEngine_ProcFile -lXEngine_SystemApi -lRfcComponents_EmailClient -lNetHelp_APIHelp -lXEngine_Download \
      -lXControl_Infomation -lXControl_Configure -lXControl_Protocol \
	  -ljsoncpp
LIBEX = -static-libgcc -ldl -lrt -lpthread 
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = XControl_Config.o XControl_Handle.o XControl_Task.o XControl_Process.o XEngine_XControl.o

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
  
XControl_Config.o:./XControl_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XControl_Config.cpp 
XControl_Handle.o:./XControl_Handle.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XControl_Handle.cpp
XControl_Task.o:./XControl_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XControl_Task.cpp
XControl_Process.o:./XControl_Process.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XControl_Process.cpp
	
XEngine_XControl.o:./XEngine_XControl.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XEngine_XControl.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_XControlApp
	cp -f ./XEngine_XControlApp ../../XEngine_Release/XEngine_XControlApp
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_XControlApp
