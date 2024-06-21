#include "rice/rice_package.h"

#include "package.h"
#include "rice_private_config.h"

ReadPackageResult read_native_package_header(FILE *file, PackageMetainf *out_metainf) {
    guint64 result = fread(out_metainf, sizeof(PackageMetainf), 1, file);
    // check if package header is valid header
    if (result != sizeof(PackageMetainf)) {
        fclose(file);
        return READ_PACKAGE_RESULT_ERR_BAD_METAINF;
    }

    if (memcmp(out_metainf->signature, RICE_NRP_SIGNATURE, 4) != 0) {
        fclose(file);
        return READ_PACKAGE_RESULT_ERR_BAD_METAINF;
    }

    if (out_metainf->version > RICE_VERSION) {
        fclose(file);
        return READ_PACKAGE_RESULT_ERR_BAD_METAINF;
    }

    return READ_PACKAGE_RESULT_OK;
}

ReadPackageResult read_package_stream(FILE *file, RicePackage *out_package) {
    // TODO add read package logic
}