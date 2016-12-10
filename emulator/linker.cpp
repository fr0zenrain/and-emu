#ifndef WINCE
#include <errno.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

#ifdef _MSC_VER
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
//#include <dlfcn.h>
#include <unistd.h>
#endif

#include "elf.h"
#include "linker_phdr.h"
#include "linker.h"

extern uc_engine* g_uc;

/* Assume average path length of 64 and max 8 paths */
#define LDPATH_BUFSIZE 512
#define LDPATH_MAX 8

#define LDPRELOAD_BUFSIZE 512
#define LDPRELOAD_MAX 8

#ifdef ANDROID_ARM_LINKER
typedef long unsigned int* _Unwind_Ptr;
extern "C" _Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount);
#else

#endif
/* >>> IMPORTANT NOTE - READ ME BEFORE MODIFYING <<<
 *
 * Do NOT use malloc() and friends or pthread_*() code here.
 * Don't use printf() either; it's caused mysterious memory
 * corruption in the past.
 * The linker runs before we bring up libc and it's easiest
 * to make sure it does not depend on any complex libc features
 *
 * open issues / todo:
 *
 * - are we doing everything we should for ARM_COPY relocations?
 * - cleaner error reporting
 * - after linking, set as much stuff as possible to READONLY
 *   and NOEXEC
 */

// We can't use malloc(3) in the dynamic linker. We use a linked list of anonymous
// maps, each a single page in size. The pages are broken up into as many struct soinfo
// objects as will fit, and they're all threaded together on a free list.
#define SOINFO_PER_POOL ((PAGE_SIZE - sizeof(soinfo_pool_t*)) / sizeof(soinfo))
struct soinfo_pool_t {
	soinfo_pool_t* next;
	soinfo info[SOINFO_PER_POOL];
};
static struct soinfo_pool_t* gSoInfoPools = NULL;
static soinfo* gSoInfoFreeList = NULL;

soinfo* solist = &libdl_info;
soinfo* sonext = &libdl_info;
soinfo* somain; /* main process, always the one after libdl_info */

static char gLdPathsBuffer[LDPATH_BUFSIZE];
static const char* gLdPaths[LDPATH_MAX + 1] = { "", "/system/lib",
		"/vendor/lib", "/data/local/tmp", };

static const char* system_lib[] = { "libc.so", "libm.so", "libstdc++.so",
		"libz.so", "liblog.so", "libcutils.so", "libnativehelper.so",
		"libcorkscrew.so", "libgccdemangle.so", "libselinux.so",
		"libstlport.so", "libdvm.so", };

static char gLdPreloadsBuffer[LDPRELOAD_BUFSIZE];
static const char* gLdPreloadNames[LDPRELOAD_MAX + 1];

static soinfo* gLdPreloads[LDPRELOAD_MAX + 1];

int gLdDebugVerbosity;

enum RelocationKind {
	kRelocAbsolute = 0, kRelocRelative, kRelocCopy, kRelocSymbol, kRelocMax
};

#if STATS
struct linker_stats_t {
	int count[kRelocMax];
};

static linker_stats_t linker_stats;

static void count_relocation(RelocationKind kind) {
	++linker_stats.count[kind];
}
#else
static void count_relocation(RelocationKind) {
}
#endif

#if COUNT_PAGES
static unsigned bitmask[4096];
#define MARK(offset) \
    do { \
        bitmask[((offset) >> 12) >> 3] |= (1 << (((offset) >> 12) & 7)); \
    } while(0)
#else
#define MARK(x) do {} while (0)
#endif

// You shouldn't try to call memory-allocating functions in the dynamic linker.
// Guard against the most obvious ones.
#define DISALLOW_ALLOCATION(return_type, name, ...) \
    return_type name __VA_ARGS__ \
    { \
        const char* msg = "ERROR: " #name " called from the dynamic linker!\n"; \
        __libc_format_log(ANDROID_LOG_FATAL, "linker", "%s", msg); \
        write(2, msg, strlen(msg)); \
        abort(); \
    }
#define UNUSED __attribute__((unused))
/*DISALLOW_ALLOCATION(void*, malloc, (size_t u UNUSED));
 DISALLOW_ALLOCATION(void, free, (void* u UNUSED));
 DISALLOW_ALLOCATION(void*, realloc, (void* u1 UNUSED, size_t u2 UNUSED));
 DISALLOW_ALLOCATION(void*, calloc, (size_t u1 UNUSED, size_t u2 UNUSED));*/

static char tmp_err_buf[768];
static char __linker_dl_err_buf[768];

char* linker_get_error_buffer() {
	return &__linker_dl_err_buf[0];
}

size_t linker_get_error_buffer_size() {
	return sizeof(__linker_dl_err_buf);
}

static bool ensure_free_list_non_empty() {
	unsigned int addr = 0;
	unsigned int value = 0;
	if (gSoInfoFreeList != NULL) {
		return true;
	}

	// Allocate a new pool.
	soinfo_pool_t* pool = reinterpret_cast<soinfo_pool_t*>(s_mmap(NULL,
			sizeof(*pool), PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
			-1, 0));
	if (pool == MAP_FAILED) {
		return false;
	}

	// Add the pool to our list of pools.
	addr = (unsigned int)pool + offsetof(soinfo_pool_t,next);
	uc_err err = uc_mem_write(g_uc,(uint64_t)pool,&gSoInfoPools,4);
	//pool->next = gSoInfoPools;
	gSoInfoPools = pool;

	// Chain the entries in the new pool onto the free list.
	//gSoInfoFreeList = &pool->info[0];
	addr = (unsigned int)pool + offsetof(soinfo_pool_t,info);
	gSoInfoFreeList = (soinfo*)addr;

	soinfo* next = NULL;
	for (int i = SOINFO_PER_POOL - 1; i >= 0; --i) {

		//pool->info[i].next = next;
		//next = &pool->info[i];
		unsigned int s = (unsigned int)pool + offsetof(soinfo_pool_t,info)+i*sizeof(soinfo);
		addr = (int)s+offsetof(soinfo,next);
		err=uc_mem_write(g_uc,(uint64_t)addr,&next,4);
		err=uc_mem_read(g_uc,(uint64_t)s,&next,4);	
	}

	return true;
}

static void set_soinfo_pool_protection(int protection) {
	/*for (soinfo_pool_t* p = gSoInfoPools; p != NULL; p = p->next) {
		if (s_mprotect(p, sizeof(*p), protection) == -1) {
			abort(); // Can't happen.
		}
	}*/
}

soinfo* soinfo_alloc(const char* name) {
	if (strlen(name) >= SOINFO_NAME_LEN) {
		//loader_printf(LOG_ERR, "library name \"%s\" too long\n", name);
		return NULL;
	}

	if (!ensure_free_list_non_empty()) {
		//loader_printf(LOG_ERR, "out of memory when loading \"%s\"\n", name);
		return NULL;
	}

	// Take the head element off the free list.
	soinfo* si = gSoInfoFreeList;
	unsigned int addr = (int)gSoInfoFreeList+offsetof(soinfo,next);
	int value = 0;
	uc_err err = uc_mem_read(g_uc,(uint64_t)addr,&value,4);
	err = uc_mem_write(g_uc,(uint64_t)gSoInfoFreeList,&value,4);
	//gSoInfoFreeList = gSoInfoFreeList->next;

	// Initialize the new element.
	soinfo s;
	memset(&s, 0, sizeof(soinfo));
	err = uc_mem_write(g_uc,(uint64_t)si,&s,sizeof(soinfo));
	//memset(si, 0, sizeof(soinfo));
	addr = (int)si+offsetof(soinfo,name);

	err = uc_mem_write(g_uc,(uint64_t)addr,name,strlen(name)+1);
	//s_strlcpy(si->name, name, sizeof(si->name));

    if((int)sonext >= EMULATOR_MEMORY_START)
    {
        addr = (int)sonext+offsetof(soinfo,next);
        err = uc_mem_write(g_uc,(uint64_t)addr,&si,4);

        err = uc_mem_write(g_uc,(uint64_t)sonext,&si,4);
    }
	else
    {
        sonext->next = si;
        sonext = si;
    }

	//loader_printf( "name %s: allocated soinfo @ %p\n", name, si);
	return si;
}

static void soinfo_free(soinfo* si) {
	if (si == NULL) {
		return;
	}

	soinfo *prev = NULL, *trav;

	//loader_printf( "name %s: freeing soinfo @ %p\n", si->name, si);

	for (trav = solist; trav != NULL; trav = trav->next) {
		if (trav == si)
			break;
		prev = trav;
	}
	if (trav == NULL) {
		/* si was not in solist */
		//loader_printf( "name \"%s\" is not in solist!\n", si->name);
		return;
	}

	/* prev will never be NULL, because the first entry in solist is
	 always the static libdl_info.
	 */
	prev->next = si->next;
	if (si == sonext) {
		sonext = prev;
	}
	si->next = gSoInfoFreeList;
	gSoInfoFreeList = si;
}

#ifdef ANDROID_ARM_LINKER

/* For a given PC, find the .so that it belongs to.
 * Returns the base address of the .ARM.exidx section
 * for that .so, and the number of 8-byte entries
 * in that section (via *pcount).
 *
 * Intended to be called by libc's __gnu_Unwind_Find_exidx().
 *
 * This function is exposed via dlfcn.cpp and libdl.so.
 */
extern "C" _Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int *pcount)
{
	soinfo *si;
	unsigned addr = (unsigned)pc;

	for (si = solist; si != 0; si = si->next) {
		if ((addr >= si->base) && (addr < (si->base + si->size))) {
			*pcount = (int)si->ARM_exidx_count;
			return (_Unwind_Ptr)si->ARM_exidx;
		}
	}
	*pcount = 0;
	return NULL;
}

#elif defined(ANDROID_X86_LINKER) || defined(ANDROID_MIPS_LINKER)

/* Here, we only have to provide a callback to iterate across all the
 * loaded libraries. gcc_eh does the rest. */
int
dl_iterate_phdr(int (*cb)(dl_phdr_info *info, size_t size, void *data),
		void *data)
{
	int rv = 0;
	for (soinfo* si = solist; si != NULL; si = si->next) {
		dl_phdr_info dl_info;
		dl_info.dlpi_addr = si->link_map.l_addr;
		dl_info.dlpi_name = si->link_map.l_name;
		dl_info.dlpi_phdr = si->phdr;
		dl_info.dlpi_phnum = si->phnum;
		rv = cb(&dl_info, sizeof(dl_phdr_info), data);
		if (rv != 0) {
			break;
		}
	}
	return rv;
}

#endif

static Elf32_Sym* soinfo_elf_lookup(soinfo* si, unsigned hash,const char* name) 
{
	Elf32_Sym* s = 0;

	if(si->emu)
	{
		return ((emulator*)si->emulator)->get_symbols(name,hash);
	}

	if(si->base)	
	{
		for (unsigned n = si->tmp_bucket[hash % si->nbucket]; n != 0; n =
				si->tmp_chain[n]) {
			s = si->tmp_symtab + n;
			if (strcmp((char*)si->tmp_strtab + s->st_name, name))
				continue;

			/* only concern ourselves with global and weak symbol definitions */
			switch (ELF32_ST_BIND(s->st_info)) {
			case STB_GLOBAL:
			case STB_WEAK:
				if (s->st_shndx == SHN_UNDEF) {
					continue;
				}

				debug_printf("FOUND %s in %s (%08x) %d\n", name, si->name,
						s->st_value, s->st_size);

				return s;
			}
		}
	}
	else
	{
		Elf32_Sym* symtab = si->symtab;
		const char* strtab = si->strtab;

		for (unsigned n = si->bucket[hash % si->nbucket]; n != 0; n =
		si->chain[n]) {
			Elf32_Sym* s = symtab + n;
			if (strcmp(strtab + s->st_name, name))
				continue;

			/* only concern ourselves with global and weak symbol definitions */
			switch (ELF32_ST_BIND(s->st_info)) {
		case STB_GLOBAL:
		case STB_WEAK:
			if (s->st_shndx == SHN_UNDEF) {
				continue;
			}

			debug_printf("FOUND %s in %s (%08x) %d\n", name, si->name,
				s->st_value, s->st_size);
			return s;
			}
		}

	}

	return NULL;
}

static unsigned elfhash(const char* _name) {
	const unsigned char* name = (const unsigned char*) _name;
	unsigned h = 0, g;

	while (*name) {
		h = (h << 4) + *name++;
		g = h & 0xf0000000;
		h ^= g;
		h ^= g >> 24;
	}
	return h;
}

static Elf32_Sym* soinfo_do_lookup(soinfo* si, const char* name, soinfo** lsi,
		soinfo* needed[]) {
	unsigned elf_hash = elfhash(name);
	Elf32_Sym* s = NULL;

	if (si != NULL && somain != NULL) {

		/*
		 * Local scope is executable scope. Just start looking into it right away
		 * for the shortcut.
		 */

		if (si == somain) {
			s = soinfo_elf_lookup(si, elf_hash, name);
			if (s != NULL) {
				*lsi = si;
				goto done;
			}
		} else {
			/* Order of symbol lookup is controlled by DT_SYMBOLIC flag */

			/*
			 * If this object was built with symbolic relocations disabled, the
			 * first place to look to resolve external references is the main
			 * executable.
			 */

			if (!si->has_DT_SYMBOLIC) {
				debug_printf("%s: looking up %s in executable %s\n",
						si->name, name, somain->name);
				s = soinfo_elf_lookup(somain, elf_hash, name);
				if (s != NULL) {
					*lsi = somain;
					goto done;
				}
			}

			/* Look for symbols in the local scope (the object who is
			 * searching). This happens with C++ templates on i386 for some
			 * reason.
			 *
			 * Notes on weak symbols:
			 * The ELF specs are ambiguous about treatment of weak definitions in
			 * dynamic linking.  Some systems return the first definition found
			 * and some the first non-weak definition.   This is system dependent.
			 * Here we return the first definition found for simplicity.  */

			s = soinfo_elf_lookup(si, elf_hash, name);
			if (s != NULL) {
				*lsi = si;
				goto done;
			}

			/*
			 * If this object was built with -Bsymbolic and symbol is not found
			 * in the local scope, try to find the symbol in the main executable.
			 */

			if (si->has_DT_SYMBOLIC) {
				debug_printf(
						"%s: looking up %s in executable %s after local scope\n",
						si->name, name, somain->name);
				s = soinfo_elf_lookup(somain, elf_hash, name);
				if (s != NULL) {
					*lsi = somain;
					goto done;
				}
			}
		}
	}

	/* Next, look for it in the preloads list */
	for (int i = 0; gLdPreloads[i] != NULL; i++) {
		s = soinfo_elf_lookup(gLdPreloads[i], elf_hash, name);
		if (s != NULL) {
			*lsi = gLdPreloads[i];
			goto done;
		}
	}
	debug_printf(
			"si->base = 0x%08x, si->strtab = %p, si->symtab = %p,si->bucket = %p,si->chain = %p\n",
			si->base, si->strtab, si->symtab, si->bucket, si->chain);
	for (int i = 0; needed[i] != NULL; i++) {
		debug_printf( "%s: looking up %s in %s\n", si->name, name,
				needed[i]->name);
		s = soinfo_elf_lookup(needed[i], elf_hash, name);
		if (s != NULL) {
			*lsi = needed[i];
			goto done;
		}
	}

	done:
	if (s != NULL) 
	{
		/*debug_printf("si %s sym %s s->st_value = 0x%08x, " "found in %s, base = 0x%08x, load bias = 0x%08x\n",
				si->name, name, s->st_value, (*lsi)->name, (*lsi)->base,
				(*lsi)->load_bias);*/
		return s;
	}

	return NULL;
}

/* This is used by dlsym(3).  It performs symbol lookup only within the
 specified soinfo object and not in any of its dependencies.

 TODO: Only looking in the specified soinfo seems wrong. dlsym(3) says
 that it should do a breadth first search through the dependency
 tree. This agrees with the ELF spec (aka System V Application
 Binary Interface) where in Chapter 5 it discuss resolving "Shared
 Object Dependencies" in breadth first search order.
 */
Elf32_Sym* dlsym_handle_lookup(soinfo* si, const char* name) {
	return soinfo_elf_lookup(si, elfhash(name), name);
}

/* This is used by dlsym(3) to performs a global symbol lookup. If the
 start value is null (for RTLD_DEFAULT), the search starts at the
 beginning of the global solist. Otherwise the search starts at the
 specified soinfo (for RTLD_NEXT).
 */
Elf32_Sym* dlsym_linear_lookup(const char* name, soinfo** found,
		soinfo* start) {
	unsigned elf_hash = elfhash(name);

	if (start == NULL) {
		start = solist;
	}

	Elf32_Sym* s = NULL;
	for (soinfo* si = start; (s == NULL) && (si != NULL); si = si->next) {
		s = soinfo_elf_lookup(si, elf_hash, name);
		if (s != NULL) {
			*found = si;
			break;
		}
	}

	if (s != NULL) {
		debug_printf( "%s s->st_value = 0x%08x, found->base = 0x%08x",
				name, s->st_value, (*found)->base);
	}

	return s;
}

soinfo* find_containing_library(const void* p) {
	Elf32_Addr address = reinterpret_cast<Elf32_Addr>(p);
	for (soinfo* si = solist; si != NULL; si = si->next) {
		if (address >= si->base && address - si->base < si->size) {
			return si;
		}
	}
	return NULL;
}

Elf32_Sym* dladdr_find_symbol(soinfo* si, const void* addr) {
	Elf32_Addr soaddr = reinterpret_cast<Elf32_Addr>(addr) - si->base;

	// Search the library's symbol table for any defined symbol which
	// contains this address.
	for (size_t i = 0; i < si->nchain; ++i) {
		Elf32_Sym* sym = &si->symtab[i];
		if (sym->st_shndx != SHN_UNDEF && soaddr >= sym->st_value
				&& soaddr < sym->st_value + sym->st_size) {
			return sym;
		}
	}

	return NULL;
}

void dump(soinfo* si) {
	Elf32_Sym* s = si->symtab;
	for (unsigned n = 0; n < si->nchain; n++) {
		debug_printf( "%04d> %08x: %02x %04x %08x %08x %s", n, s,
				s->st_info, s->st_shndx, s->st_value, s->st_size,
				si->strtab + s->st_name);
		s++;
	}
}

static int open_library_on_path(const char* name, const char* const paths[]) {
	char buf[512];
	for (size_t i = 0; paths[i] != NULL; ++i) {
		if (strcmp(paths[i], "") == 0) {
#ifdef _MSC_VER
			int fd = s_open(name, O_RDONLY| _O_BINARY);
#else
			int fd = s_open(name, O_RDONLY | O_CLOEXEC);
#endif 
			if (fd != -1) {
				return fd;
			}

		}
#ifdef _MSC_VER
		int n = _snprintf(buf, sizeof(buf), "%s/%s", paths[i], name);
#else
		int n = snprintf(buf, sizeof(buf), "%s/%s", paths[i], name);
#endif
		if (n < 0 || n >= static_cast<int>(sizeof(buf))) {
			debug_printf("Warning: ignoring very long library path: %s/%s\n",
					paths[i], name);
			continue;
		}
#ifdef _MSC_VER
			int fd = s_open(name, O_RDONLY| _O_BINARY);
#else
			int fd = s_open(name, O_RDONLY | O_CLOEXEC);
#endif 
		if (fd != -1) {
			return fd;
		}
	}
	return -1;
}

int open_library(const char* name) {

	// If the name contains a slash, we should attempt to open it directly and not search the paths.
	if (strchr(name, '/') != NULL) {
#ifdef _MSC_VER
			int fd = s_open(name, O_RDONLY| _O_BINARY);
#else
			int fd = s_open(name, O_RDONLY | O_CLOEXEC);
#endif 
		if (fd != -1) {
			return fd;
		}
		// ...but nvidia binary blobs (at least) rely on this behavior, so fall through for now.
	}

	// Otherwise we try LD_LIBRARY_PATH first, and fall back to the built-in well known paths.
	int fd = open_library_on_path(name, gLdPaths);

	return fd;
}

static soinfo* load_library(const char* name) {
	// Open the file.
	debug_printf("open %s\n", name);

	int fd = open_library(name);
	if (fd == -1) {
		debug_printf("library \"%s\" not found\n", name);
		return NULL;
	}

	// Read the ELF header and load the segments.
	ElfReader elf_reader(name, fd, 0);
	if (!elf_reader.Load()) {
		return NULL;
	}

	const char* bname = strrchr(name, '/');
	soinfo* si = soinfo_alloc(bname ? bname + 1 : name);
	if (si == NULL) {
		return NULL;
	}
    if((int)si >= EMULATOR_MEMORY_START)
    {
        unsigned int addr = (int)si + offsetof(soinfo, base);
        unsigned int value = elf_reader.load_start();
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, size);
        value = elf_reader.load_size();
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, load_bias);
        value = elf_reader.load_bias();
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, flags);
        value = 0;
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, entry);
        value = 0;
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, dynamic);
        value = 0;
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, phnum);
        value = elf_reader.phdr_count();
        uc_mem_write(g_uc,addr,&value, 4);

        addr = (int)si + offsetof(soinfo, phdr);
        value = (unsigned int)elf_reader.loaded_phdr();
        uc_mem_write(g_uc,addr,&value, 4);
    }
    else
    {
        si->base = elf_reader.load_start();
        si->size = elf_reader.load_size();
        si->load_bias = elf_reader.load_bias();
        si->flags = 0;
        si->entry = 0;
        si->dynamic = NULL;
        si->phnum = elf_reader.phdr_count();
        si->phdr = elf_reader.loaded_phdr();
    }
	return si;
}

soinfo *find_loaded_library(const char *name) {
	soinfo *si;
	const char *bname;
    char buf[128] = {0};
    unsigned int addr = 0;

	// TODO: don't use basename only for determining libraries
	// http://code.google.com/p/android/issues/detail?id=6670

	bname = strrchr(name, '/');
	bname = bname ? bname + 1 : name;

	for (si = solist; si != NULL; ) {
        if((int)si >= EMULATOR_MEMORY_START)
        {
            addr = (int)si + offsetof(soinfo,name);
            uc_mem_read(g_uc,addr,buf,128);
        }
        else
        {
            memcpy(buf,si->name,128);
        }

		if (!strcmp(bname, buf)) {
			return si;
		}
        if((int)si >= EMULATOR_MEMORY_START) {
            addr = (int) si + offsetof(soinfo, next);
            uc_mem_read(g_uc, addr, &si, 4);
        }
        else
        {
            si = si->next;
        }
	}
	return NULL;
}

static soinfo* find_library_internal(const char* name) {
	if (name == NULL) {
		return somain;
	}

	soinfo* si = find_loaded_library(name);
	if (si != NULL) {
		if (si->flags & FLAG_LINKED) {
			return si;
		}
		debug_printf("OOPS: recursive link to \"%s\"\n", si->name);
		return NULL;
	}

	debug_printf("[ '%s' has not been loaded yet.  Locating...]\n",
			name);
	si = load_library(name);
	if (si == NULL) {
		return NULL;
	}

	// At this point we know that whatever is loaded @ base is a valid ELF
	// shared library whose segments are properly mapped in.
	//debug_printf("[ init_library base=0x%08x sz=0x%08x name='%s' ]\n",
	//		si->base, si->size, si->name);

	if (!soinfo_link_image(si, true, 0)) {
		s_munmap(reinterpret_cast<void*>(si->base), si->size);
		soinfo_free(si);
		return NULL;
	}

	return si;
}

static soinfo* find_library(const char* name) {
    int ref = 0;
	soinfo* si = find_library_internal(name);
	if (si != NULL) {
        unsigned int addr = (unsigned int)si + offsetof(soinfo,ref_count);
        uc_mem_read(g_uc,addr,&ref,4);
        ref++;
        uc_mem_write(g_uc,addr,&ref,4);
		//si->ref_count++;
	}
	return si;
}

static int soinfo_unload(soinfo* si) {
	if (si->ref_count == 1) {
		debug_printf( "unloading '%s'\n", si->name);
		if (!strcmp(si->name, "libc.so") || !strcmp(si->name, "libstdc++.so")
				|| !strcmp(si->name, "libm.so") || !strcmp(si->name, "libz.so")
				|| !strcmp(si->name, "libdl.so")
				|| !strcmp(si->name, "liblog.so"))
			;
		else
			si->CallDestructors();

		for (Elf32_Dyn* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
			if (d->d_tag == DT_NEEDED) {
				const char* library_name = si->strtab + d->d_un.d_val;
				debug_printf( "%s needs to unload %s\n", si->name,
						library_name);
				soinfo_unload(find_loaded_library(library_name));
			}
		}

		s_munmap(reinterpret_cast<void*>(si->base), si->size);
		//notify_gdb_of_unload(si);
		soinfo_free(si);
		si->ref_count = 0;
	} else {
		si->ref_count--;
		debug_printf(
				"not unloading '%s', decrementing ref_count to %d\n", si->name,
				si->ref_count);
	}
	return 0;
}

soinfo* do_dlopen(const char* name, int flags) {
    char tmp[128] = {0};
	if ((flags & ~(RTLD_NOW | RTLD_LAZY | RTLD_LOCAL | RTLD_GLOBAL)) != 0) {
		debug_printf( "invalid flags to dlopen: %x", flags);
		return NULL;
	}
	set_soinfo_pool_protection(PROT_READ | PROT_WRITE);
	soinfo* si = find_library(name);
    uc_mem_read(g_uc,(unsigned int)si,tmp,128);
	if (si != NULL) {
		if (!strcmp(tmp, "libc.so") || !strcmp(tmp, "libstdc++.so")
				|| !strcmp(tmp, "libz.so") || !strcmp(tmp, "libm.so")
				|| !strcmp(tmp, "liblog.so"))
			;
		else {
			si->CallConstructors();
		}
	}
	set_soinfo_pool_protection (PROT_READ);
	return si;
}

int do_dlclose(soinfo* si) {
	set_soinfo_pool_protection(PROT_READ | PROT_WRITE);
	int result = soinfo_unload(si);
	set_soinfo_pool_protection (PROT_READ);
	return result;
}

/* TODO: don't use unsigned for addrs below. It works, but is not
 * ideal. They should probably be either uint32_t, Elf32_Addr, or unsigned
 * long.
 */
static int soinfo_relocate(soinfo* si, Elf32_Rel* rel_addr, unsigned count,
		soinfo* needed[]) {
	uc_err err;
	Elf32_Sym* s;
	Elf32_Rel* start = rel_addr;
	Elf32_Rel* trel = (Elf32_Rel*)malloc(count*sizeof(Elf32_Rel));
	Elf32_Sym* symtab = si->tmp_symtab;
	const char* strtab = si->tmp_strtab;
	soinfo* lsi;
	
	err = uc_mem_read(g_uc,(uint64_t)rel_addr,trel,count*sizeof(Elf32_Rel));

	Elf32_Rel* rel = trel;
	for (size_t idx = 0; idx < count; ++idx, ++rel) {
		unsigned type = ELF32_R_TYPE(rel->r_info);
		unsigned sym = ELF32_R_SYM(rel->r_info);
		Elf32_Addr reloc =
				static_cast<Elf32_Addr>(rel->r_offset + si->load_bias);
		Elf32_Addr sym_addr = 0;
		char* sym_name = NULL;

		debug_printf("Processing '%s' relocation at index %d\n",
				si->name, idx);
		if (type == 0) { // R_*_NONE
			continue;
		}
		if (sym != 0) {
			sym_name = (char *) (strtab + symtab[sym].st_name);
			s = soinfo_do_lookup(si, sym_name, &lsi, needed);
			if (s == NULL) {
				/* We only allow an undefined symbol if this is a weak
				 reference..   */
				s = &symtab[sym];
				if (ELF32_ST_BIND(s->st_info) != STB_WEAK) {
					debug_printf(
							"cannot locate symbol \"%s\" referenced by \"%s\"...\n",
							sym_name, si->name);
					//free(rel);
					//free(symtab);
					//free((void*)strtab);
					return -1;
				}

				/* IHI0044C AAELF 4.5.1.1:

				 Libraries are not searched to resolve weak references.
				 It is not an error for a weak reference to remain
				 unsatisfied.

				 During linking, the value of an undefined weak reference is:
				 - Zero if the relocation type is absolute
				 - The address of the place if the relocation is pc-relative
				 - The address of nominal base address if the relocation
				 type is base-relative.
				 */

				switch (type) {
#if defined(ANDROID_ARM_LINKER)
				case R_ARM_JUMP_SLOT:
				case R_ARM_GLOB_DAT:
				case R_ARM_ABS32:
				case R_ARM_RELATIVE: /* Don't care. */
#elif defined(ANDROID_X86_LINKER)
				case R_386_JMP_SLOT:
				case R_386_GLOB_DAT:
				case R_386_32:
				case R_386_RELATIVE: /* Dont' care. */
#endif /* ANDROID_*_LINKER */
				/* sym_addr was initialized to be zero above or relocation
				 code below does not care about value of sym_addr.
				 No need to do anything.  */
				break;

#if defined(ANDROID_X86_LINKER)
				case R_386_PC32:
				sym_addr = reloc;
				break;
#endif /* ANDROID_X86_LINKER */

#if defined(ANDROID_ARM_LINKER)
				case R_ARM_COPY:
				/* Fall through.  Can't really copy if weak symbol is
				 not found in run-time.  */
#endif /* ANDROID_ARM_LINKER */
			default:
				debug_printf("unknown weak reloc type %d @ %p (%d)\n",
						type, rel, (int) (rel - start));
				return -1;
				}
			} else {
				/* We got a definition.  */
#if 0
				if ((base == 0) && (si->base != 0)) {
					/* linking from libraries to main image is bad */
					DL_ERR("cannot locate \"%s\"...",
							strtab + symtab[sym].st_name);
					return -1;
				}
#endif
				sym_addr =
						static_cast<Elf32_Addr>(s->st_value + lsi->load_bias);
			}
			count_relocation(kRelocSymbol);
		} else {
			s = NULL;
		}

		/* TODO: This is ugly. Split up the relocations by arch into
		 * different files.
		 */
		switch (type) {
#if defined(ANDROID_ARM_LINKER)
		case R_ARM_JUMP_SLOT:
		count_relocation(kRelocAbsolute);
		MARK(rel->r_offset);
		debug_printf("RELO JMP_SLOT %08x <- %08x %s\n", reloc, sym_addr, sym_name);
		//uc_mem_write(g_uc,reloc,&sym_addr,4);
		err=uc_mem_write(g_uc,reloc,&sym_addr,4);
        if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
		//*reinterpret_cast<Elf32_Addr*>(reloc) = sym_addr;
		break;
		case R_ARM_GLOB_DAT:
		count_relocation(kRelocAbsolute);
		MARK(rel->r_offset);
		debug_printf("RELO GLOB_DAT %08x <- %08x %s\n", reloc, sym_addr, sym_name);
		//uc_mem_write(g_uc,reloc,&sym_addr,4);
		//uc_mem_write(g_uc,reloc,&svc,4);
		err=uc_mem_write(g_uc,reloc,&sym_addr,4);
        if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
		//*reinterpret_cast<Elf32_Addr*>(reloc) = sym_addr;
		break;
		case R_ARM_ABS32:
		count_relocation(kRelocAbsolute);
		MARK(rel->r_offset);
		debug_printf("RELO ABS %08x <- %08x %s\n", reloc, sym_addr, sym_name);
		//uc_mem_write(g_uc,reloc,&sym_addr,4);
		//uc_mem_write(g_uc,reloc,&svc,4);
		err=uc_mem_write(g_uc,reloc,&sym_addr,4);
        if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
		//*reinterpret_cast<Elf32_Addr*>(reloc) += sym_addr;
		break;
		case R_ARM_REL32:
		count_relocation(kRelocRelative);
		MARK(rel->r_offset);
		debug_printf("RELO REL32 %08x <- %08x - %08x %s\n",
				reloc, sym_addr, rel->r_offset, sym_name);
		*reinterpret_cast<Elf32_Addr*>(reloc) += sym_addr - rel->r_offset;
		break;
#elif defined(ANDROID_X86_LINKER)
		case R_386_JMP_SLOT:
		count_relocation(kRelocAbsolute);
		MARK(rel->r_offset);
		debug_printf("RELO JMP_SLOT %08x <- %08x %s", reloc, sym_addr, sym_name);
		*reinterpret_cast<Elf32_Addr*>(reloc) = sym_addr;
		break;
		case R_386_GLOB_DAT:
		count_relocation(kRelocAbsolute);
		MARK(rel->r_offset);
		debug_printf("RELO GLOB_DAT %08x <- %08x %s", reloc, sym_addr, sym_name);
		*reinterpret_cast<Elf32_Addr*>(reloc) = sym_addr;
		break;
#elif defined(ANDROID_MIPS_LINKER)
		case R_MIPS_REL32:
		count_relocation(kRelocAbsolute);
		MARK(rel->r_offset);
		TRACE_TYPE(RELO, "RELO REL32 %08x <- %08x %s",
				reloc, sym_addr, (sym_name) ? sym_name : "*SECTIONHDR*");
		if (s) {
			*reinterpret_cast<Elf32_Addr*>(reloc) += sym_addr;
		} else {
			*reinterpret_cast<Elf32_Addr*>(reloc) += si->base;
		}
		break;
#endif /* ANDROID_*_LINKER */

#if defined(ANDROID_ARM_LINKER)
		case R_ARM_RELATIVE:
#elif defined(ANDROID_X86_LINKER)
		case R_386_RELATIVE:
#endif /* ANDROID_*_LINKER */
		count_relocation(kRelocRelative);
		MARK(rel->r_offset);
		if (sym) {
			debug_printf("odd RELATIVE form...");
			return -1;
		}
		debug_printf( "RELO RELATIVE %08x <- +%08x\n", reloc,
				si->base);
		err=uc_mem_read(g_uc,reloc,&sym_addr,4);
        if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
		sym_addr +=si->base;
		//uc_mem_write(g_uc,reloc,&sym_addr,4);
		err=uc_mem_write(g_uc,reloc,&sym_addr,4);
        if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
		//*reinterpret_cast<Elf32_Addr*>(reloc) += si->base;
		break;

#if defined(ANDROID_X86_LINKER)
		case R_386_32:
		count_relocation(kRelocRelative);
		MARK(rel->r_offset);

		debug_printf("RELO R_386_32 %08x <- +%08x %s\n", reloc, sym_addr, sym_name);
		*reinterpret_cast<Elf32_Addr*>(reloc) += sym_addr;
		break;

		case R_386_PC32:
		count_relocation(kRelocRelative);
		MARK(rel->r_offset);
		debug_printf("RELO R_386_PC32 %08x <- +%08x (%08x - %08x) %s\n",
				reloc, (sym_addr - reloc), sym_addr, reloc, sym_name);
		*reinterpret_cast<Elf32_Addr*>(reloc) += (sym_addr - reloc);
		break;
#endif /* ANDROID_X86_LINKER */

#ifdef ANDROID_ARM_LINKER
		case R_ARM_COPY:
		if ((si->flags & FLAG_EXE) == 0) {
			/*
			 * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0044d/IHI0044D_aaelf.pdf
			 *
			 * Section 4.7.1.10 "Dynamic relocations"
			 * R_ARM_COPY may only appear in executable objects where e_type is
			 * set to ET_EXEC.
			 *
			 * TODO: FLAG_EXE is set for both ET_DYN and ET_EXEC executables.
			 * We should explicitly disallow ET_DYN executables from having
			 * R_ARM_COPY relocations.
			 */
			debug_printf("%s R_ARM_COPY relocations only supported for ET_EXEC", si->name);
			return -1;
		}
		count_relocation(kRelocCopy);
		MARK(rel->r_offset);
		debug_printf("RELO %08x <- %d @ %08x %s", reloc, s->st_size, sym_addr, sym_name);
		if (reloc == sym_addr) {
			Elf32_Sym *src = soinfo_do_lookup(NULL, sym_name, &lsi, needed);

			if (src == NULL) {
				debug_printf("%s R_ARM_COPY relocation source cannot be resolved", si->name);
				return -1;
			}
			if (lsi->has_DT_SYMBOLIC) {
				debug_printf("%s invalid R_ARM_COPY relocation against DT_SYMBOLIC shared "
						"library %s (built with -Bsymbolic?)", si->name, lsi->name);
				return -1;
			}
			if (s->st_size < src->st_size) {
				debug_printf("%s R_ARM_COPY relocation size mismatch (%d < %d)",
						si->name, s->st_size, src->st_size);
				return -1;
			}
			memcpy((void*)reloc, (void*)(src->st_value + lsi->load_bias), src->st_size);
		} else {
			debug_printf("%s R_ARM_COPY relocation target cannot be resolved", si->name);
			return -1;
		}
		break;
#endif /* ANDROID_ARM_LINKER */

	default:
		debug_printf( "unknown reloc type %d @ %p (%d)\n", type, rel,
				(int) (rel - start));
		return -1;
		}
	}

	free(trel);

	return 0;
}

void soinfo::CallArray(const char* array_name,linker_function_t* functions, size_t count, bool reverse) 
{
	if (functions == NULL) {
		return;
	}

	debug_printf("[ Calling %s (size %d) @ %p for '%s' ]\n",
			array_name, count, functions, name);

	int begin = reverse ? (count - 1) : 0;
	int end = reverse ? -1 : count;
	int step = reverse ? -1 : 1;

	for (int i = begin; i != end; i += step) {
		debug_printf( "[ %s[%d] == %p ]\n", array_name, i,
				functions[i]);
		
		CallFunction("function",functions[i]);
	}

	debug_printf("[ Done calling %s for '%s' ]\n", array_name, name);
}

void soinfo::CallFunction(const char* function_name,
		linker_function_t function) {
	if (function == NULL
			|| reinterpret_cast<unsigned int>(function)
					== static_cast<unsigned int>(-1)) {
		return;
	}

	debug_printf("[ Calling %s @ %p for '%s' ]\n", function_name,
			function, name);


    unsigned int pc = (int)function;
    pc = pc&1?pc-1:pc;

    emulator::get_emulator()->start_emulator(pc,this);

	debug_printf("[ Done calling %s @ %p for '%s' ]\n", function_name,function, name);

	// The function may have called dlopen(3) or dlclose(3), so we need to ensure our data structures
	// are still writable. This happens with our debug malloc (see http://b/7941716).
	set_soinfo_pool_protection(PROT_READ | PROT_WRITE);
}

void soinfo::CallPreInitConstructors() {
	// DT_PREINIT_ARRAY functions are called before any other constructors for executables,
	// but ignored in a shared library.
	CallArray("DT_PREINIT_ARRAY", preinit_array, preinit_array_count, false);
}

void soinfo::CallConstructors() {
    int con_called = 0;
    int flag = 0;
    int prein_array;
	int dyn;
    unsigned int tmp_dyn;
    uc_mem_read(g_uc,(unsigned int)&constructors_called,&con_called,4);
    uc_mem_read(g_uc,(unsigned int)&flags,&flag,4);
    uc_mem_read(g_uc,(unsigned int)&preinit_array,&prein_array,4);
    uc_mem_read(g_uc,(unsigned int)&dynamic,&dyn,4);

	if (con_called) {
		return;
	}

	// We set constructors_called before actually calling the constructors, otherwise it doesn't
	// protect against recursive constructor calls. One simple example of constructor recursion
	// is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:
	// 1. The program depends on libc, so libc's constructor is called here.
	// 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
	// 3. dlopen() calls the constructors on the newly created
	//    soinfo for libc_malloc_debug_leak.so.
	// 4. The debug .so depends on libc, so CallConstructors is
	//    called again with the libc soinfo. If it doesn't trigger the early-
	//    out above, the libc constructor will be called again (recursively!).
    con_called = true;

	if ((flag & FLAG_EXE) == 0 && prein_array != NULL) {
		// The GNU dynamic linker silently ignores these, but we warn the developer.
		//debug_printf("\"%s\": ignoring %d-entry DT_PREINIT_ARRAY in shared library!\n", name, preinit_array_count);
	}

    uc_mem_read(g_uc,(unsigned int)&tmp_dynamic,&tmp_dyn,4);
	if (dyn != NULL) {
		for (Elf32_Dyn* d = (Elf32_Dyn*)tmp_dyn; d->d_tag != DT_NULL; ++d) {
			if (d->d_tag == DT_NEEDED) {
				const char* library_name = tmp_strtab + d->d_un.d_val;
				//debug_printf("\"%s\": calling constructors in DT_NEEDED \"%s\"\n", name, library_name);
				//find_loaded_library(library_name)->CallConstructors();
			}
		}
	}

	//debug_printf("\"%s\": calling constructors\n", name);

	// DT_INIT should be called before DT_INIT_ARRAY if both are present.
	CallFunction("DT_INIT", init_func);
	linker_function_t* f = (linker_function_t*)malloc(init_array_count*4);
	uc_mem_read(g_uc,(uint64_t)init_array,f,init_array_count*4);
	CallArray("DT_INIT_ARRAY", f, init_array_count, false);
	free(f);
}

void soinfo::CallDestructors() {
	debug_printf("\"%s\": calling destructors\n", name);

	// DT_FINI_ARRAY must be parsed in reverse order.
	CallArray("DT_FINI_ARRAY", fini_array, fini_array_count, true);

	// DT_FINI should be called after DT_FINI_ARRAY if both are present.
	CallFunction("DT_FINI", fini_func);
}

int get_strtab_size(soinfo* si)
{
	int size = 0;
	int index ;
	si->tmp_symtab = (Elf32_Sym*)malloc(si->nchain*sizeof(Elf32_Sym));
	uc_err err = uc_mem_read(g_uc,(uint64_t)si->symtab,si->tmp_symtab,si->nchain*sizeof(Elf32_Sym));
	if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
	for(int i = 0; i < si->nchain ; i++)
	{
		if(si->tmp_symtab[i].st_name > si->strtab_size)
		{
			size = si->tmp_symtab[i].st_name;
			index = i;
		}
	}
	return size; 
}

bool soinfo_link_image(soinfo* si1, bool breloc, ElfReader* reader) {
	/* "base" might wrap around UINT32_MAX. */
    soinfo ss;
    uc_mem_read(g_uc,(unsigned int)si1,&ss,sizeof(soinfo));
    soinfo* si = &ss;
	Elf32_Addr base = si->load_bias;
	const Elf32_Phdr *phdr = si->phdr;
	int phnum = si->phnum;
	bool relocating_linker = (si->flags & FLAG_LINKER) != 0;

	/* We can't debug anything until the linker is relocated */
	if (!relocating_linker) {
		debug_printf("[ linking %s ]\n", si->name);
		debug_printf("si->base = 0x%08x si->flags = 0x%08x\n",
				si->base, si->flags);
	}

	/* Extract dynamic section */
	size_t dynamic_count;
	Elf32_Word dynamic_flags;
	Elf32_Phdr* phdr1 = (Elf32_Phdr*)malloc(0x1000-0x34);
	uc_mem_read(g_uc,base+0x34,phdr1,0x1000-0x34);
	phdr_table_get_dynamic_section(phdr1, phnum, base, &si->dynamic,
			&dynamic_count, &dynamic_flags, reader);
	if (si->dynamic == NULL) {
		if (!relocating_linker) {
			debug_printf( "missing PT_DYNAMIC in \"%s\"\n", si->name);
		}
		return false;
	} else {
		if (!relocating_linker) {
			debug_printf( "dynamic = %p\n", si->dynamic);
		}
	}

#ifdef ANDROID_ARM_LINKER
	(void) phdr_table_get_arm_exidx(phdr1, phnum, base,
			&si->ARM_exidx, (size_t*)&si->ARM_exidx_count,reader);
#endif

	free(phdr1);

	// Extract useful information from dynamic section.
	uint32_t needed_count = 0;
	uint64_t addr = (uint64_t)si->dynamic;
	si->tmp_dynamic = (Elf32_Dyn*)malloc(dynamic_count*sizeof(Elf32_Dyn));
	uc_err err=uc_mem_read(g_uc,addr,si->tmp_dynamic,dynamic_count*sizeof(Elf32_Dyn));
    if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
	for (Elf32_Dyn* d = si->tmp_dynamic; d->d_tag != DT_NULL; ++d) {
		debug_printf("d = %p, d[0](tag) = 0x%08x d[1](val) = 0x%08x\n", d, d->d_tag,
				d->d_un.d_val);
		switch (d->d_tag) {
		case DT_HASH:
			addr = base + d->d_un.d_ptr;
			unsigned int buf[16];
			uc_mem_read(g_uc,(uint64_t)addr,buf,16);
			si->nbucket = buf[0];
			si->nchain = buf[1];
			si->bucket = (unsigned*)((int)base + d->d_un.d_ptr + 8);
			si->chain = (unsigned*)(base +d->d_un.d_ptr + 8 + si->nbucket * 4);
			break;
		case DT_STRTAB:
			si->strtab = (const char *) (base + d->d_un.d_ptr);
			break;
		case DT_SYMTAB:
			si->symtab = (Elf32_Sym *) (base + d->d_un.d_ptr);
			break;
		case DT_PLTREL:
			if (d->d_un.d_val != DT_REL) {
				debug_printf("unsupported DT_RELA in \"%s\"",si->name);
				return false;
			}
			break;
		case DT_JMPREL:
			si->plt_rel = (Elf32_Rel*) (base + d->d_un.d_ptr);
			break;
		case DT_PLTRELSZ:
			si->plt_rel_count = d->d_un.d_val / sizeof(Elf32_Rel);
			break;
		case DT_REL:
			si->rel = (Elf32_Rel*) (base + d->d_un.d_ptr);
			break;
		case DT_RELSZ:
			si->rel_count = d->d_un.d_val / sizeof(Elf32_Rel);
			break;
		case DT_PLTGOT:
			/* Save this in case we decide to do lazy binding. We don't yet. */
			si->plt_got = (unsigned *) (base + d->d_un.d_ptr);
			break;
			/*case DT_DEBUG:
			 // Set the DT_DEBUG entry to the address of _r_debug for GDB
			 // if the dynamic table is writable
			 if ((dynamic_flags & PF_W) != 0) {
			 d->d_un.d_val = (int) &_r_debug;
			 }
			 break;*/
		case DT_RELA:
			debug_printf("unsupported DT_RELA in \"%s\"", si->name);
			return false;
		case DT_INIT:
			si->init_func = reinterpret_cast<linker_function_t>(base
					+ d->d_un.d_ptr);
			debug_printf("%s constructors (DT_INIT) found at %p",
					si->name, si->init_func);
			break;
		case DT_FINI:
			si->fini_func = reinterpret_cast<linker_function_t>(base
					+ d->d_un.d_ptr);
			debug_printf("%s destructors (DT_FINI) found at %p",
					si->name, si->fini_func);
			break;
		case DT_INIT_ARRAY:
			si->init_array = reinterpret_cast<linker_function_t*>(base
					+ d->d_un.d_ptr);
			debug_printf("%s constructors (DT_INIT_ARRAY) found at %p", si->name,
					si->init_array);
			break;
		case DT_INIT_ARRAYSZ:
			si->init_array_count = ((unsigned) d->d_un.d_val)
					/ sizeof(Elf32_Addr);
			break;
		case DT_FINI_ARRAY:
			si->fini_array = reinterpret_cast<linker_function_t*>(base
					+ d->d_un.d_ptr);
			debug_printf( "%s destructors (DT_FINI_ARRAY) found at %p",
					si->name, si->fini_array);
			break;
		case DT_FINI_ARRAYSZ:
			si->fini_array_count = ((unsigned) d->d_un.d_val)
					/ sizeof(Elf32_Addr);
			break;
		case DT_PREINIT_ARRAY:
			si->preinit_array = reinterpret_cast<linker_function_t*>(base
					+ d->d_un.d_ptr);
			debug_printf("%s constructors (DT_PREINIT_ARRAY) found at %p", si->name,
					si->preinit_array);
			break;
		case DT_PREINIT_ARRAYSZ:
			si->preinit_array_count = ((unsigned) d->d_un.d_val)
					/ sizeof(Elf32_Addr);
			break;
		case DT_TEXTREL:
			si->has_text_relocations = true;
			break;
		case DT_SYMBOLIC:
			si->has_DT_SYMBOLIC = true;
			break;
		case DT_NEEDED:
			++needed_count;
			break;
#if defined DT_FLAGS
			// TODO: why is DT_FLAGS not defined?
			case DT_FLAGS:
			if (d->d_un.d_val & DF_TEXTREL) {
				si->has_text_relocations = true;
			}
			if (d->d_un.d_val & DF_SYMBOLIC) {
				si->has_DT_SYMBOLIC = true;
			}
			break;
#endif
		}
	}
	si->strtab_size = get_strtab_size(si);

	si->tmp_strtab = (char*)malloc(si->strtab_size+10);
	si->tmp_bucket = (unsigned int*)malloc(si->nbucket*4);
	si->tmp_chain = (unsigned int*)malloc(si->nchain*4);
	err = uc_mem_read(g_uc,(uint64_t)si->strtab,(void*)si->tmp_strtab,si->strtab_size);
    if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
	err = uc_mem_read(g_uc,(uint64_t)si->bucket,si->tmp_bucket,si->nbucket*4);
    if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
	err = uc_mem_read(g_uc,(uint64_t)si->chain,si->tmp_chain,si->nchain*4);
    if(err != UC_ERR_OK) { printf("uc error %d\n",err);}

	//debug_printf("si->base = 0x%08x, si->strtab = %p, si->symtab = %p,si->bucket = %p,si->chain = %p\n",
	//		si->base, si->strtab, si->symtab, si->bucket, si->chain);

	// Sanity checks.
	if (relocating_linker && needed_count != 0) {
		debug_printf("linker cannot have DT_NEEDED dependencies on other libraries\n");
		return false;
	}
	if (si->nbucket == 0) {
		debug_printf("empty/missing DT_HASH in \"%s\" (built with --hash-style=gnu?)\n",
				si->name);
		return false;
	}
	if (si->strtab == 0) {
		debug_printf("empty/missing DT_STRTAB in \"%s\"\n", si->name);
		return false;
	}
	if (si->symtab == 0) {
		debug_printf( "empty/missing DT_SYMTAB in \"%s\"\n", si->name);
		return false;
	}

	// If this is the main executable, then load all of the libraries from LD_PRELOAD now.
	if (si->flags & FLAG_EXE) {
		memset(gLdPreloads, 0, sizeof(gLdPreloads));
		size_t preload_count = 0;
		for (size_t i = 0; gLdPreloadNames[i] != NULL; i++) {
			soinfo* lsi = find_library(gLdPreloadNames[i]);
			if (lsi != NULL) {
				gLdPreloads[preload_count++] = lsi;
			} else {
				// As with glibc, failure to load an LD_PRELOAD library is just a warning.
				debug_printf(
						"could not load library \"%s\" from LD_PRELOAD for \"%s\"; caused by %s\n",
						gLdPreloadNames[i], si->name,
						linker_get_error_buffer());
			}
		}
	}

	si->tmp_needed = (soinfo**)malloc((1 + needed_count) * sizeof(soinfo*));
	soinfo** pneeded = si->tmp_needed;

	for (Elf32_Dyn* d = si->tmp_dynamic; d->d_tag != DT_NULL; ++d) {
		if (d->d_tag == DT_NEEDED) {
			char library_name[32]={0};
			const char* lib_name = si->strtab + d->d_un.d_val;
			uc_mem_read(g_uc,(uint64_t)lib_name,library_name,32);
			debug_printf( "%s needs %s\n", si->name, library_name);
			soinfo* lsi = find_library(library_name);
			if (lsi == NULL) {
				s_strlcpy(tmp_err_buf, linker_get_error_buffer(),
						sizeof(tmp_err_buf));
				debug_printf(
						"could not load library \"%s\" needed by \"%s\"; caused by %s\n",
						library_name, si->name, tmp_err_buf);
				return false;
			}
			*pneeded++ = lsi;
		}
	}
	*pneeded = NULL;

	if (si->has_text_relocations) {
		/* Unprotect the segments, i.e. make them writable, to allow
		 * text relocations to work properly. We will later call
		 * phdr_table_protect_segments() after all of them are applied
		 * and all constructors are run.
		 */
		debug_printf(
				"%s has text relocations. This is wasting memory and is "
						"a security risk. Please fix.", si->name);
		if (phdr_table_unprotect_segments(si->phdr, si->phnum, si->load_bias)
				< 0) {
			debug_printf(
					"can't unprotect loadable segments for \"%s\": %s",
					si->name, strerror(errno));
			return false;
		}
	}

	if(somain == 0 && !si->emulator)
	{
		if(strcmp(si->name,"libz.so") && strcmp(si->name,"liblog.so") &&
			strcmp(si->name,"libm.so") && strcmp(si->name,"libstdc++.so") &&
			  strcmp(si->name,"libhelper.so"))
		{
			somain = si;
		}
	}

	if (si->plt_rel != NULL && breloc) {
		debug_printf("[ relocating %s plt ]\n", si->name);
		if (soinfo_relocate(si, si->plt_rel, si->plt_rel_count, si->tmp_needed)) {
			return false;
		}
	}
	if (si->rel != NULL && breloc) {
		debug_printf("[ relocating %s ]\n", si->name);
		if (soinfo_relocate(si, si->rel, si->rel_count, si->tmp_needed)) {
			return false;
		}
	}

#ifdef ANDROID_MIPS_LINKER
	if (!mips_relocate_got(si, si->tmp_needed)) {
		return false;
	}
#endif

	si->flags |= FLAG_LINKED;
	debug_printf("[ finished linking %s ]\n", si->name);

	if (si->has_text_relocations) {
		/* All relocations are done, we can protect our segments back to
		 * read-only. */
		if (phdr_table_protect_segments(si->phdr, si->phnum, si->load_bias)
				< 0) {
			debug_printf("can't protect segments for \"%s\": %s\n",
					si->name, strerror(errno));
			return false;
		}
	}

	/* We can also turn on GNU RELRO protection */
	if (phdr_table_protect_gnu_relro(si->phdr, si->phnum, si->load_bias) < 0) {
		debug_printf("can't enable GNU RELRO protection for \"%s\": %s\n", si->name,
				strerror(errno));
		return false;
	}

    err = uc_mem_write(g_uc,(unsigned int)si1,si,sizeof(soinfo));
	return true;
}

/*
 * This code is called after the linker has linked itself and
 * fixed it's own GOT. It is safe to make references to externs
 * and other non-local data at this point.
 */
/* Compute the load-bias of an existing executable. This shall only
 * be used to compute the load bias of an executable or shared library
 * that was loaded by the kernel itself.
 *
 * Input:
 *    elf    -> address of ELF header, assumed to be at the start of the file.
 * Return:
 *    load bias, i.e. add the value of any p_vaddr in the file to get
 *    the corresponding address in memory.
 */
/*
static Elf32_Addr get_elf_exec_load_bias(const Elf32_Ehdr* elf) {
	Elf32_Addr offset = elf->e_phoff;
	const Elf32_Phdr* phdr_table = (const Elf32_Phdr*) ((char*) elf + offset);
	const Elf32_Phdr* phdr_end = phdr_table + elf->e_phnum;

	for (const Elf32_Phdr* phdr = phdr_table; phdr < phdr_end; phdr++) {
		if (phdr->p_type == PT_LOAD) {
			return reinterpret_cast<Elf32_Addr>(elf) + phdr->p_offset
					- phdr->p_vaddr;
		}
	}
	return 0;
}*/

void show_loaded_library() {
	soinfo *si;
	for (si = solist; si != NULL; si = si->next) {
		debug_printf("%s base %x size %x\n", si->name, si->base,
				si->size);
	}
}
/*
int get_loaded_soinfo(RootEngine* engine,const char *name) {
	int fd;
	int __base, __size;
	size_t dynamic_count;
	Elf32_Word dynamic_flags;
	char path[512] = { 0 };

	__base = get_module_base(name, path);
	__size = get_module_size(name, path);

	fd = open_library("/proc/self/mem");

	if (fd == -1) {
		return 0;
	}

	//set file pointer to current modules
	engine->s_lseek(fd, __base, SEEK_SET);
	ElfReader elf_reader(name, fd, __base, __size, false);
	if (!elf_reader.Load()) {
		engine->s_close(fd);
		return NULL;
	}

	const char* bname = strrchr(name, '/');

	soinfo* si = soinfo_alloc(engine,bname ? bname + 1 : name);

	if (si == NULL) {
		return NULL;
	}

	si->base = elf_reader.load_start();
	si->size = elf_reader.load_size();
	si->load_bias = elf_reader.load_bias();
	si->flags |= FLAG_LINKED;
	si->entry = 0;
	si->dynamic = NULL;
	si->phnum = elf_reader.phdr_count();
	si->phdr = elf_reader.loaded_phdr();
	si->ref_count = 0xffffff;

	soinfo_link_image(si, false, &elf_reader);

	printf(
	 "si->base = 0x%08x, si->strtab = %p, si->symtab = %p,si->bucket = %p,si->chain = %p,si->dynamic = %p,si->name %s\n", si->base,
	 si->strtab, si->symtab,si->bucket,si->chain,si->dynamic,si->name);
}*/
#ifdef _MSC_VER
void linker_init() 
#else
__attribute__((constructor)) void linker_init() 
#endif
{	

	
	/*	get_loaded_soinfo("libc.so");
	 get_loaded_soinfo("libm.so");
	 get_loaded_soinfo("libstdc++.so");
	 get_loaded_soinfo("libz.so");
	 get_loaded_soinfo("liblog.so");
	 get_loaded_soinfo("libbacktrace.so");
	 get_loaded_soinfo("libcutils.so");
	 get_loaded_soinfo("libnativehelper.so");
	 get_loaded_soinfo("libcorkscrew.so");
	 get_loaded_soinfo("libgccdemangle.so");
	 get_loaded_soinfo("libselinux.so");
	 get_loaded_soinfo("libutils.so");
	 get_loaded_soinfo("libstlport.so");
	 get_loaded_soinfo("libdvm.so");
	for (int i = 0; i < sizeof(system_lib) / 4; i++)
		get_loaded_soinfo(system_lib[i]);*/
}

#ifdef _MSC_VER
void  linker_uninit() 
#else
void __attribute__((destructor)) linker_uninit()
#endif
{
	//dlclose(dvm);
}
