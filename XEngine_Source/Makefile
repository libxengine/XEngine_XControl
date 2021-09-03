RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

XCONTRAL_INFO_PATH = ./XContral_Infomation
XCONTRAL_CONFIG_PATH = ./XContral_Configure

XCONTRAL_APP_PATH = ./XEngine_XContral

NETENGINE_LIB = libXContral_Infomation.so libXContral_Configure.so XEngine_XContral.exe

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)
	
libXContral_Infomation.so:
	make -C $(XCONTRAL_INFO_PATH) $(FLAGS)
libXContral_Configure.so:
	make -C $(XCONTRAL_CONFIG_PATH) $(FLAGS)
	
XEngine_XContral.exe:
	make -C $(XCONTRAL_APP_PATH) $(FLAGS)


SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 

SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB)
