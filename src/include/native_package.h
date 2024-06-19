/*
 * file: native_package.h
 * author: filebin
 * date: 2024-06-20 00:01:44
 */

#ifndef SRC_INCLUDE_READ_PACKAGE_RESULT_H_
#define SRC_INCLUDE_READ_PACKAGE_RESULT_H_

#include <archive.h>
#include <glib.h>
typedef enum _ReadNativePackageResult ReadNativePackageResult;
typedef struct _NativePackageHeader NativePackageHeader;

enum _ReadNativePackageResult {
    READ_NATIVE_PACKAGE_RESULT_OK = 0,
    READ__ERR_FILE_NOT_FOUND = -1,
    READ_NATIVE_PACKAGE_RESULT_ERR_EOF_UNEXPECTED = -2,
    READ_NATIVE_PACKAGE_RESULT_ERR_BAD_NATIVE_HEADER = -3,
    READ_NATIVE_PACKAGE_RESULT_ERR_BAD_PACKAGE_HEADER = -4,
};

struct _NativePackageHeader {
    gchar signature[4];
    guint version;
    gint compressing_algorithm;
};

#define RETURN_NATIVE_PACKAGE_RESULT_IF_NOT_OK(result) \
    if (result != READ_NATIVE_PACKAGE_RESULT_OK) \
        return result;

#endif