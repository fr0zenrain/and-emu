#ifndef _LINKER_H_
#define _LINKER_H_
#include "elf.h"
#include <stdarg.h>
#include "runtime/runtime.h"

#define ANDROID_ARM_LINKER

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#ifndef PAGE_MASK
#define PAGE_MASK (~(PAGE_SIZE - 1))
#endif


#ifdef _MSC_VER

// win32
#define O_RDONLY 00000000
#define _O_BINARY 0x8000

#define MAP_SHARED 0x001
#define MAP_PRIVATE 0x002
#define MAP_TYPE 0x00f
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAP_FIXED 0x010


#define MAP_FAILED ((void *)-1)

#define PROT_NONE 0x00
#define PROT_READ 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PROT_WRITE 0x02
#define PROT_EXEC 0x04
#define PROT_SEM 0x10

#define MS_SYNC		4
#define SYS_NMLN  65

#define	ICACHE	(1<<0)	
#define	DCACHE	(1<<1)	

#if defined(_MSC_VER) || defined(__MACH__)
#define MAP_ANONYMOUS 0x0800
#define MAP_POPULATE 0x8000
#endif


#endif

#ifdef _MSC_VER
#else
#include <unistd.h>
//#include <sys/types.h>
#endif

#define page
// Returns the address of the page containing address 'x'.
#define PAGE_START(x)  ((x) & PAGE_MASK)

// Returns the offset of address 'x' in its page.
#define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)

// Returns the address of the next page after address 'x', unless 'x' is
// itself at the start of a page.
#define PAGE_END(x)    PAGE_START((x) + (PAGE_SIZE-1))


/* bionic is currently only 32-bit. */
#define ElfW(type) Elf32_##type

typedef struct {
	const char *dli_fname; /* Pathname of shared object that                       contains address */
	void *dli_fbase; /* Address at which shared object                  is loaded */
	const char *dli_sname; /* Name of nearest symbol with address                         lower than addr */
	void *dli_saddr; /* Exact address of symbol named              in dli_sname */
} Dl_info;

enum {
#if defined(__LP64__)
	RTLD_NOW  = 2,
#else
	RTLD_NOW  = 0,
#endif
	RTLD_LAZY = 1,

	RTLD_LOCAL  = 0,
#if defined(__LP64__)
	RTLD_GLOBAL = 0x00100,
#else
	RTLD_GLOBAL = 2,
#endif
	RTLD_NOLOAD = 4,
	RTLD_NODELETE = 0x01000,
};


struct dl_phdr_info {
	ElfW(Addr) dlpi_addr;
	const char* dlpi_name;
	const ElfW(Phdr)* dlpi_phdr;
	ElfW(Half) dlpi_phnum;
};

// Magic shared structures that GDB knows about.

struct link_map_t {
	unsigned int l_addr;
	char* l_name;
	unsigned int l_ld;
	link_map_t* l_next;
	link_map_t* l_prev;
};

// Values for r_debug->state
enum {
	RT_CONSISTENT, RT_ADD, RT_DELETE
};

struct r_debug {
	int r_version;
	link_map_t* r_map;
	void (*r_brk)(void);
	int r_state;
	unsigned int r_ldbase;
};

#define FLAG_LINKED     0x00000001
#define FLAG_EXE        0x00000004 // The main executable
#define FLAG_LINKER     0x00000010 // The linker itself

#define SOINFO_NAME_LEN 128

typedef void (*linker_function_t)();

struct soinfo {
public:
	char name[SOINFO_NAME_LEN];
	const Elf32_Phdr* phdr;
	size_t phnum;
	Elf32_Addr entry;
	Elf32_Addr base;
	unsigned size;

	uint32_t unused1;  // DO NOT USE, maintained for compatibility.

	Elf32_Dyn* dynamic;

	uint32_t unused2; // DO NOT USE, maintained for compatibility
	uint32_t unused3; // DO NOT USE, maintained for compatibility

	soinfo* next;
	unsigned flags;

	const char* strtab;
	Elf32_Sym* symtab;

	size_t nbucket;
	size_t nchain;
	unsigned* bucket;
	unsigned* chain;

	unsigned* plt_got;

	Elf32_Rel* plt_rel;
	size_t plt_rel_count;

	Elf32_Rel* rel;
	size_t rel_count;

	linker_function_t* preinit_array;
	size_t preinit_array_count;

	linker_function_t* init_array;
	size_t init_array_count;
	linker_function_t* fini_array;
	size_t fini_array_count;

	linker_function_t init_func;
	linker_function_t fini_func;

#if defined(ANDROID_ARM_LINKER)
	// ARM EABI section used for stack unwinding.
	unsigned* ARM_exidx;
	unsigned * ARM_exidx_count;
#elif defined(ANDROID_MIPS_LINKER)
	unsigned mips_symtabno;
	unsigned mips_local_gotno;
	unsigned mips_gotsym;
#endif

	size_t ref_count;
	link_map_t link_map;

	bool constructors_called;

	// When you read a virtual address from the ELF file, add this
	// value to get the corresponding address in the process' address space.
	Elf32_Addr load_bias;

	bool has_text_relocations;
	bool has_DT_SYMBOLIC;
	size_t strtab_size;
	Elf32_Dyn* tmp_dynamic;
	Elf32_Phdr* tmp_phdr;
	Elf32_Sym* tmp_symtab;
	char* tmp_strtab;
	unsigned int* tmp_bucket;
	unsigned int* tmp_chain;
	int emu;
	void* runtime;

	void CallConstructors();
	void CallDestructors();
	void CallPreInitConstructors();

private:
	void CallArray(const char* array_name, linker_function_t* functions, size_t count, bool reverse);
	void CallFunction(const char* function_name, linker_function_t function);
};

extern soinfo libdl_info;

// These aren't defined in <sys/exec_elf.h>.
#ifndef DT_PREINIT_ARRAY
#define DT_PREINIT_ARRAY   32
#endif
#ifndef DT_PREINIT_ARRAYSZ
#define DT_PREINIT_ARRAYSZ 33
#endif

void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path);
soinfo* do_dlopen(const char* name, int flags);
int do_dlclose(soinfo* si);

Elf32_Sym* dlsym_linear_lookup(const char* name, soinfo** found, soinfo* start);
soinfo* find_containing_library(const void* addr);

Elf32_Sym* dladdr_find_symbol(soinfo* si, const void* addr);
Elf32_Sym* dlsym_handle_lookup(soinfo* si, const char* name);

char* linker_get_error_buffer();
size_t linker_get_error_buffer_size();
class ElfReader;
int open_library(const char* name);
soinfo* soinfo_alloc(const char* name);
bool soinfo_link_image(soinfo* si, bool breloc, ElfReader* reader);


void *s_mmap(void *start, size_t length, int prot, int flags,int fd, int offset);
int s_mprotect( void *start, size_t len, int prot);
int s_munmap(void *start, size_t length);

#define debug_printf printf


#endif
