#include "glib.h"
#include "rice/rice.h"

#include "macros.h"
#include "package.h"
#include "rice_private_config.h"

#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

NODISCARD ReadPackageResult read_package_info_yml(struct archive *archive, RicePackage *out_package);

NODISCARD ReadPackageResult read_package_metainf(FILE *file, PackageMetainfHeader *out_metainf) {
    guint64 result = fread(out_metainf, sizeof(PackageMetainfHeader), 1, file);
    // check if package header is valid header
    if (result != sizeof(PackageMetainfHeader)) {
        fclose(file);
        return READ_PACKAGE_RESULT_ERR_BAD_METAINF;
    }

    if (memcmp(out_metainf->signature, RICE_METAINF_SIGNATURE, RICE_METAINF_SIGNATURE_SIZE) != 0) {
        fclose(file);
        return READ_PACKAGE_RESULT_ERR_BAD_METAINF;
    }

    if (out_metainf->version > RICE_VERSION) {
        fclose(file);
        return READ_PACKAGE_RESULT_ERR_BAD_METAINF;
    }

    return READ_PACKAGE_RESULT_OK;
}

NODISCARD ReadPackageResult read_package_stream(FILE *file, RicePackage *out_package) {
    struct archive *archive;
    struct archive_entry *entry;
    ReadPackageResult package_result = READ_PACKAGE_RESULT_ERR_PACKAGEINFO_NOT_FOUND;
    int result;

    archive = archive_read_new();
    archive_read_support_filter_all(archive);
    archive_read_support_format_all(archive);

    result = archive_read_open_FILE(archive, file);

    if (result != ARCHIVE_OK)
        return READ_PACKAGE_RESULT_ERR_BAD_ARCHIVE;

    size_t buffer_size = RICE_ARCHIVE_BLOCK_SIZE_INITIAL;
    char *buff = g_malloc_n(buffer_size, sizeof(char));

    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        int64_t entry_size = archive_entry_size(entry);

        if (entry_size <= 0)
            continue;

        const char *entry_pathname = archive_entry_pathname(entry);

        if (memcmp(entry_pathname, RICE_ARCHIVE_ENTRY_PACKAGE_INFO, RICE_ARCHIVE_ENTRY_PACKAGE_INFO_SIZE) != 0)
            continue;

        const char *entry_ext = entry_pathname + RICE_ARCHIVE_ENTRY_PACKAGE_INFO_SIZE;

        const char *supported_file_formats[] = {".yaml", ".yml", ".json"};

        for (guint8 i = 0; i < SIZEOF_ARR(supported_file_formats); i++) {
            const char* fmt = supported_file_formats[i];
            if (strcmp(entry_ext, fmt) == 0) {
                package_result = read_package_info_yml(archive, out_package);
                break;
            }
        }
        archive_entry_free(entry);
    }
    free(buff);
    result = archive_read_free(archive);
    if (result != ARCHIVE_OK)
        return READ_PACKAGE_RESULT_ERR_EOF_UNEXPECTED;

    return READ_PACKAGE_RESULT_OK;
}