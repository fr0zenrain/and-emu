
include_directories(include)

set(SOURCEFILES
        arch/X86/X86ATTInstPrinter.c
        arch/X86/X86Disassembler.c
        arch/X86/X86DisassemblerDecoder.c
        arch/X86/X86IntelInstPrinter.c
        arch/X86/X86Mapping.c
        arch/X86/X86Module.c
        cs.c
        MCInst.c
        MCInstrDesc.c
        MCRegisterInfo.c
        SStream.c
        utils.c
        )

add_library(capstone ${SOURCEFILES})