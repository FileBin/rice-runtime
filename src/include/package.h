/*
 * file: universal_package.h
 * author: filebin
 * date: 2024-06-21 00:49:09
 */

#ifndef SRC_INCLUDE_PACKAGE_H_
#define SRC_INCLUDE_PACKAGE_H_

#include <glib.h>
#include <rice/rice.h>

typedef struct _PackageMetainfHeader PackageMetainfHeader;
typedef enum _ReadPackageResult ReadPackageResult;
typedef enum _ConfigFileFormat ConfigFileFormat;
typedef enum _RicePackageFormat RicePackageFormat;
typedef struct _RicePackage RicePackage;

enum _ConfigFileFormat {
    CONFIG_FILE_FORMAT_NONE = 0, // that file not present
    CONFIG_FILE_FORMAT_YAML = 1, // yml file format
    CONFIG_FILE_FORMAT_JSON = 2, // json file format
};

struct _PackageMetainfHeader {
    gchar signature[8];
    guint version;
    // TODO add metainf metadata
};

enum _ReadPackageResult {
    READ_PACKAGE_RESULT_OK = 0,
    READ__ERR_FILE_NOT_FOUND = -1,
    READ_PACKAGE_RESULT_ERR_EOF_UNEXPECTED = -2,
    READ_PACKAGE_RESULT_ERR_BAD_ARCHIVE = -3,
    READ_PACKAGE_RESULT_ERR_BAD_METAINF = -4,
    READ_PACKAGE_RESULT_ERR_PACKAGEINFO_NOT_FOUND = -5,
    READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_STREAM = -6,
    READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_DOCUMENT = -7,
    READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT = -8,
    READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_VERSION = -9,
};

enum _RicePackageFormat {
    RICE_PACKAGE_FORMAT_NRP = 0,
    RICE_PACKAGE_FORMAT_URP = 1,
    RICE_PACKAGE_FORMAT_RRP = 2,
    RICE_PACKAGE_FORMAT_MAX,
};

struct _RicePackage {
    gchar *name;
    gchar *provides;
    RiceVersion version;
    rice_array_t dependencies;
};

void rice_package_set_name(RicePackage* package, gchar* name);

const char *get_package_format_file_extension(RicePackageFormat format);

#define RETURN_PACKAGE_RESULT_IF_NOT_OK(result) \
    if (result != READ_PACKAGE_RESULT_OK) \
    return result

#endif