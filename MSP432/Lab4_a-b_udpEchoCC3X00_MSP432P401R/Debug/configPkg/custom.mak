## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em4f linker.cmd package/cfg/udpEchoCC3X00_pem4f.oem4f

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/udpEchoCC3X00_pem4f.xdl
	$(SED) 's"^\"\(package/cfg/udpEchoCC3X00_pem4fcfg.cmd\)\"$""\"C:/MSP432/udpEchoCC3X00_MSP_EXP432P401R_TI_MSP432P401R/Debug/configPkg/\1\""' package/cfg/udpEchoCC3X00_pem4f.xdl > $@
	-$(SETDATE) -r:max package/cfg/udpEchoCC3X00_pem4f.h compiler.opt compiler.opt.defs
