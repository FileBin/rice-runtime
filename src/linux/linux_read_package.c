#include <archive.h>
#include <elf.h>
#include <glib.h>
#include <stdio.h>
#include <time.h>

#include "glibconfig.h"
#include "rice/rice_package.h"

#if defined(__LP64__)
#define ElfW(type) Elf64_##type
#else
#define ElfW(type) Elf32_##type
#endif

#include "native_package.h"

ReadNativePackageResult read_native_package_header(FILE *file, NativePackageHeader *out_native_header);
ReadNativePackageResult read_package_stream(FILE *file, RicePackage *out_package);

ReadNativePackageResult PLATFORM_read_native_package(gchar *path, RicePackage *out_package) {
    ElfW(Ehdr) header;
    NativePackageHeader native_package_header;

    ReadNativePackageResult result = READ_NATIVE_PACKAGE_RESULT_OK;

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return READ_NATIVE_PACKAGE_RESULT_ERR_EOF_UNEXPECTED;
    }
    fread(&header, sizeof(header), 1, file);

    if (memcmp(header.e_ident, ELFMAG, SELFMAG) != 0) {
        fclose(file);
        return READ_NATIVE_PACKAGE_RESULT_ERR_BAD_NATIVE_HEADER;
    }

    guint64 elf_end = header.e_shoff + header.e_shentsize * header.e_shnum;

    fseek(file, elf_end, SEEK_SET);

    result = read_native_package_header(file, &native_package_header);
    RETURN_NATIVE_PACKAGE_RESULT_IF_NOT_OK(result)

    result = read_package_stream(file, out_package);
    RETURN_NATIVE_PACKAGE_RESULT_IF_NOT_OK(result)

    fclose(file);

    return result;
}