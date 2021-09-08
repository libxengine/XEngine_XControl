RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

XCONTROL_INFO_PATH = ./XControl_Infomation
XCONTROL_CONFIG_PATH = ./XControl_Configure
XCONTROL_PROTOCOL_PATH = ./XControl_Protocol

XCONTROL_APP_PATH = ./XEngine_XControl

NETENGINE_LIB = libXControl_Infomation.so libXControl_Configure.so libXControl_Protocol.so XEngine_XControl.exe

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)
	
libXControl_Infomation.so:
	make -C $(XCONTROL_INFO_PATH) $(FLAGS)
libXControl_Configure.so:
	make -C $(XCONTROL_CONFIG_PATH) $(FLAGS)
libXControl_Protocol.so:
	make -C $(XCONTROL_PROTOCOL_PATH) $(FLAGS)
	
XEngine_XControl.exe:
	make -C $(XCONTROL_APP_PATH) $(FLAGS)


SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 

SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB)
