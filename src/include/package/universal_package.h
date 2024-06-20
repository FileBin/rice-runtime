/*
 * file: universal_package.h
 * author: filebin
 * date: 2024-06-21 00:49:09
 */

#ifndef SRC_INCLUDE_PACKAGE_UNIVERSAL_PACKAGE_H_
#define SRC_INCLUDE_PACKAGE_UNIVERSAL_PACKAGE_H_

#include <glib.h>

typedef struct _URP_Metainf URP_Metainf;
typedef enum _ConfigFileFormat ConfigFileFormat;

enum _ConfigFileFormat {
    CONFIG_FILE_FORMAT_NONE = 0, // that file not present
    CONFIG_FILE_FORMAT_YAML = 1, // yml file format
    CONFIG_FILE_FORMAT_JSON = 2, // json file format
};

struct _URP_Metainf {
    gchar signature[4];
    guint version;
    gchar package_info_format;
    gchar metadata_format;
};

#endif