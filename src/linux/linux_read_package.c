#include <archive.h>
#include <elf.h>
#include <glib.h>

#include "rice/rice_package.h"

#if defined(__LP64__)
#define ElfW(type) Elf64_##type
#else
#define ElfW(type) Elf32_##type
#endif

void PLATFORM_read_package(gchar *path, RicePackage *out_package) {}