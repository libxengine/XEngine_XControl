CC = g++ -Wall -std=c++17
RELEASE = 0
UNICODE = 0  
LOADBIN = -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad -L ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_OPenSsl -lHelpComponents_XLog -lXEngine_ProcSdk -lXEngine_SystemApi -lRfcComponents_EmailClient -lXEngine_Download -lNetHelp_APIHelp -lNetHelp_BackManage
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = BackManage_Handle.o BackManage_Net.o BackManage_Config.o XEngine_ManageServiceApp.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_BackServiceApp $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
BackManage_Handle.o:./BackManage_Handle.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./BackManage_Handle.cpp 
BackManage_Net.o:./BackManage_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./BackManage_Net.cpp
BackManage_Config.o:./BackManage_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./BackManage_Config.cpp
XEngine_ManageServiceApp.o:./XEngine_ManageServiceApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_ManageServiceApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_BackServiceApp
	cp -f ./XEngine_BackServiceApp ./XEngine_ManageServiceApp/XEngine_BackServiceApp
InstallSo:
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib/libXEngine_BaseLib.so ./XEngine_ManageServiceApp/libXEngine_BaseLib.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib/libXEngine_Algorithm.so ./XEngine_ManageServiceApp/libXEngine_Algorithm.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core/libXEngine_OPenSsl.so ./XEngine_ManageServiceApp/libXEngine_OPenSsl.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents/libHelpComponents_XLog.so ./XEngine_ManageServiceApp/libHelpComponents_XLog.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk/libXEngine_ProcSdk.so ./XEngine_ManageServiceApp/libXEngine_ProcSdk.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk/libXEngine_SystemApi.so ./XEngine_ManageServiceApp/libXEngine_SystemApi.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents/libRfcComponents_EmailClient.so ./XEngine_ManageServiceApp/libRfcComponents_EmailClient.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad/libXEngine_Download.so ./XEngine_ManageServiceApp/libXEngine_Download.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp/libNetHelp_BackManage.so ./XEngine_ManageServiceApp/libNetHelp_BackManage.so
	cp -f ../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp/libNetHelp_APIHelp.so ./XEngine_ManageServiceApp/libNetHelp_APIHelp.so
	
CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_BackServiceApp
	       
