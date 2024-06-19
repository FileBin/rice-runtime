#include "native_package.h"
#include "rice/rice_package.h"
#include "rice_private_config.h"

ReadNativePackageResult read_native_package_header(FILE *file, NativePackageHeader *out_package_header) {
    guint64 result = fread(out_package_header, sizeof(NativePackageHeader), 1, file);
    // check if package header is valid header
    if (result != sizeof(NativePackageHeader)) {
        fclose(file);
        return READ_NATIVE_PACKAGE_RESULT_ERR_BAD_PACKAGE_HEADER;
    }

    if (memcmp(out_package_header->signature, RICE_HEADER_SIGNATURE, 4) != 0) {
        fclose(file);
        return READ_NATIVE_PACKAGE_RESULT_ERR_BAD_PACKAGE_HEADER;
    }

    if (out_package_header->version > RICE_VERSION) {
        fclose(file);
        return READ_NATIVE_PACKAGE_RESULT_ERR_BAD_PACKAGE_HEADER;
    }
}

ReadNativePackageResult read_package_stream(FILE *file, RicePackage *out_package) {
    // TODO add read package logic
}