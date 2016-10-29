#include "linker.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define RTLD_DEFAULT  ((void*) 0xffffffff)
#define RTLD_NEXT     ((void*) 0xfffffffe)

typedef long unsigned int* _Unwind_Ptr;
extern "C" _Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount);
extern int dl_iterate_phdr(int (*cb)(dl_phdr_info *info, size_t size, void *data),
				void *data);

#ifdef _MSC_VER
#ifdef WINCE
void* __builtin_return_address (int iLevel)
{
	return 0;
}
#else
__declspec (naked) void* __builtin_return_address (int iLevel)
{
	__asm
	{
		push ebx;

		mov eax, ebp;
		mov ebx, DWORD PTR [esp + 8]; // iLevel

__Next:
		test ebx, ebx;
		je  __break;
		dec ebx;
		mov eax, DWORD PTR [eax];
		jmp __Next;
__break:

		mov eax, DWORD PTR [eax + 4];
		pop ebx;
		ret;
	}
}
#endif
#else
#endif
/* This file hijacks the symbols stubbed out in libdl.so. */


void* s_dlopen(const char* filename, int flags) {
	
	soinfo* result = do_dlopen(filename, flags);
	if (result == NULL) {
		debug_printf("dlopen failed\n");
		return NULL;
	}
	return result;
}

const char* s_dlerror() {
	return 0;
}

void* s_dlsym(void* handle, const char* symbol) {
	//ScopedPthreadMutexLocker locker(&gDlMutex);

	if (handle == NULL) {
		debug_printf("dlsym library handle is null\n");
		return NULL;
	}
	if (symbol == NULL) {
		debug_printf( "dlsym symbol name is null\n");
		return NULL;
	}

	soinfo* found = NULL;
	Elf32_Sym* sym = NULL;
	if (handle == RTLD_DEFAULT) {
		sym = dlsym_linear_lookup(symbol, &found, NULL);
	} else if (handle == RTLD_NEXT) {
		void* ret_addr = __builtin_return_address(0);
		soinfo* si = find_containing_library(ret_addr);

		sym = NULL;
		if (si && si->next) {
			sym = dlsym_linear_lookup(symbol, &found, si->next);
		}
	} else {
		found = reinterpret_cast<soinfo*>(handle);
		sym = dlsym_handle_lookup(found, symbol);
	}

	if (sym != NULL) {
		unsigned bind = ELF32_ST_BIND(sym->st_info);

		if (bind == STB_GLOBAL && sym->st_shndx != 0) {
			unsigned ret = sym->st_value + found->load_bias;
			return (void*) ret;
		}

		debug_printf("symbol %s found but not global\n", symbol);
		return NULL;
	} else {
		debug_printf("undefined symbol %s\n", symbol);
		return NULL;
	}
}

int s_dladdr(const void* addr, Dl_info* info) {
	//ScopedPthreadMutexLocker locker(&gDlMutex);

	// Determine if this address can be found in any library currently mapped.
	soinfo* si = find_containing_library(addr);
	if (si == NULL) {
		return 0;
	}

	memset(info, 0, sizeof(Dl_info));

	info->dli_fname = si->name;
	// Address at which the shared object is loaded.
	info->dli_fbase = (void*) si->base;

	// Determine if any symbol in the library contains the specified address.
	Elf32_Sym *sym = dladdr_find_symbol(si, addr);
	if (sym != NULL) {
		info->dli_sname = si->strtab + sym->st_name;
		info->dli_saddr = (void*) (si->load_bias + sym->st_value);
	}

	return 1;
}

int s_dlclose(void* handle) {
	//ScopedPthreadMutexLocker locker(&gDlMutex);
	return do_dlclose(reinterpret_cast<soinfo*>(handle));
}

#if defined(ANDROID_ARM_LINKER)
//   0000000 00011111 111112 22222222 2333333 3333444444444455555555556666666 6667
//   0123456 78901234 567890 12345678 9012345 6789012345678901234567890123456 7890
#define ANDROID_LIBDL_STRTAB \
    "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0__cxa_begin_cleanup\0__cxa_call_unexpected\0dl_unwind_find_exidx\0"

#elif defined(ANDROID_X86_LINKER) || defined(ANDROID_MIPS_LINKER)
//   0000000 00011111 111112 22222222 2333333 3333444444444455555555556666666 6667
//   0123456 78901234 567890 12345678 9012345 6789012345678901234567890123456 7890
#define ANDROID_LIBDL_STRTAB \
    "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0android_update_LD_LIBRARY_PATH\0dl_iterate_phdr\0"
#else
#error Unsupported architecture. Only ARM, MIPS, and x86 are presently supported.
#endif

// name_offset: starting index of the name in libdl_info.strtab
#define ELF32_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      reinterpret_cast<Elf32_Addr>(reinterpret_cast<void*>(value)), \
      /* st_size */ 0, \
      (shndx == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx }

extern "C" void __cxa_begin_cleanup() {
	//loader_printf(LOG_DBG, "__cxa_begin_cleanup not implemented\n");
}
extern "C" void __cxa_call_unexpected() {
	//loader_printf(LOG_DBG, "__cxa_call_unexpected not implemented\n");
}

static Elf32_Sym gLibDlSymtab[] = {
// Total length of libdl_info.strtab, including trailing 0.
// This is actually the STH_UNDEF entry. Technically, it's
// supposed to have st_name == 0, but instead, it points to an index
// in the strtab with a \0 to make iterating through the symtab easier.
		ELF32_SYM_INITIALIZER(sizeof(ANDROID_LIBDL_STRTAB) - 1, NULL, 0),
		ELF32_SYM_INITIALIZER( 0, (void*)&s_dlopen, 1),
		ELF32_SYM_INITIALIZER( 7, (void*)&s_dlclose, 1),
		ELF32_SYM_INITIALIZER(15, (void*)&s_dlsym, 1),
		ELF32_SYM_INITIALIZER(21, (void*)&s_dlerror, 1),
		ELF32_SYM_INITIALIZER(29, (void*)&s_dladdr, 1),
		ELF32_SYM_INITIALIZER(36, &__cxa_begin_cleanup, 1),
		ELF32_SYM_INITIALIZER(56, &__cxa_call_unexpected, 1),
//ELF32_SYM_INITIALIZER(36, &android_update_LD_LIBRARY_PATH, 1),
#if defined(ANDROID_ARM_LINKER)
		ELF32_SYM_INITIALIZER(78, &dl_unwind_find_exidx, 1),
#elif defined(ANDROID_X86_LINKER) || defined(ANDROID_MIPS_LINKER)
		ELF32_SYM_INITIALIZER(67, &dl_iterate_phdr, 1),
#endif
	};

// Fake out a hash table with a single bucket.
// A search of the hash table will look through
// gLibDlSymtab starting with index [1], then
// use gLibDlChains to find the next index to
// look at.  gLibDlChains should be set up to
// walk through every element in gLibDlSymtab,
// and then end with 0 (sentinel value).
//
// That is, gLibDlChains should look like
// { 0, 2, 3, ... N, 0 } where N is the number
// of actual symbols, or nelems(gLibDlSymtab)-1
// (since the first element of gLibDlSymtab is not
// a real symbol).
//
// (see soinfo_elf_lookup())
//
// Note that adding any new symbols here requires
// stubbing them out in libdl.
static unsigned gLibDlBuckets[1] = { 1 };
static unsigned gLibDlChains[12] = { 0, 2, 3, 4, 5, 6, 7, 8, 0 };

// This is used by the dynamic linker. Every process gets these symbols for free.
#ifdef _MSC_VER
soinfo libdl_info = {
	"libdl.so",
	0,0,
	0,0,0,
	0,0,0,0,
	0,
	1,
	ANDROID_LIBDL_STRTAB,
	gLibDlSymtab,
	1,
	10,
	gLibDlBuckets,
	gLibDlChains,
	0,0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,
	0xffffff,
	{ 0, 0, 0, 0, 0,},
	false,
	0,
	false,
	true
};
#else
soinfo libdl_info = {
	"libdl.so",

	phdr: 0, phnum: 0,
	entry: 0, base: 0, size: 0,
	unused1: 0, dynamic: 0, unused2: 0, unused3: 0,
	next: 0,

	flags: FLAG_LINKED,

	strtab: ANDROID_LIBDL_STRTAB,
	symtab: gLibDlSymtab,

	nbucket: 1,
	nchain: 10,
	bucket: gLibDlBuckets,
	chain: gLibDlChains,

	plt_got: 0, plt_rel: 0, plt_rel_count: 0, rel: 0, rel_count: 0,
	preinit_array: 0, preinit_array_count: 0, init_array: 0, init_array_count: 0,
	fini_array: 0, fini_array_count: 0, init_func: 0, fini_func: 0,

#if defined(ANDROID_ARM_LINKER)
	ARM_exidx: 0, ARM_exidx_count: 0,
#elif defined(ANDROID_MIPS_LINKER)
	mips_symtabno: 0, mips_local_gotno: 0, mips_gotsym: 0,
#endif

	ref_count: 0xffffff,
	{	l_addr: 0, l_name: 0, l_ld: 0, l_next: 0, l_prev: 0,},
	constructors_called: false,
	load_bias: 0,
	has_text_relocations: false,
	has_DT_SYMBOLIC: true,
};
#endif