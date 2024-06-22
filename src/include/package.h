/*
 * file: universal_package.h
 * author: filebin
 * date: 2024-06-21 00:49:09
 */

#ifndef SRC_INCLUDE_PACKAGE_H_
#define SRC_INCLUDE_PACKAGE_H_

#include <glib.h>

typedef struct _PackageMetainfHeader PackageMetainfHeader;
typedef enum _ReadPackageResult ReadPackageResult;
typedef enum _ConfigFileFormat ConfigFileFormat;

enum _ConfigFileFormat {
    CONFIG_FILE_FORMAT_NONE = 0, // that file not present
    CONFIG_FILE_FORMAT_YAML = 1, // yml file format
    CONFIG_FILE_FORMAT_JSON = 2, // json file format
};

struct _PackageMetainfHeader {
    gchar signature[4];
    guint version;
    gchar package_info_format;
    gchar reflection_info_format;
};

enum _ReadPackageResult {
    READ_PACKAGE_RESULT_OK = 0,
    READ__ERR_FILE_NOT_FOUND = -1,
    READ_PACKAGE_RESULT_ERR_EOF_UNEXPECTED = -2,
    READ_PACKAGE_RESULT_ERR_BAD_NATIVE_HEADER = -3,
    READ_PACKAGE_RESULT_ERR_BAD_METAINF = -4,
};

#define RETURN_PACKAGE_RESULT_IF_NOT_OK(result) \
    if (result != READ_PACKAGE_RESULT_OK) \
    return result

#endif