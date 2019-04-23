# invoke SourceDir generated makefile for udpEchoCC3X00.pem4f
udpEchoCC3X00.pem4f: .libraries,udpEchoCC3X00.pem4f
.libraries,udpEchoCC3X00.pem4f: package/cfg/udpEchoCC3X00_pem4f.xdl
	$(MAKE) -f C:\MSP432\udpEchoCC3X00_MSP_EXP432P401R_TI_MSP432P401R/src/makefile.libs

clean::
	$(MAKE) -f C:\MSP432\udpEchoCC3X00_MSP_EXP432P401R_TI_MSP432P401R/src/makefile.libs clean

