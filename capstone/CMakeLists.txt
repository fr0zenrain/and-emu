
include_directories(include)

set(SOURCEFILES
        arch/ARM/ARMDisassembler.c
        arch/ARM/ARMInstPrinter.c
        arch/ARM/ARMMapping.c
        arch/ARM/ARMModule.c
        cs.c
        MCInst.c
        MCInstrDesc.c
        MCRegisterInfo.c
        SStream.c
        utils.c
        )

add_library(capstone STATIC ${SOURCEFILES})