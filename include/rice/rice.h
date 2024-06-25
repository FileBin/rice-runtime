/*
 * file: Core.h
 * author: filebin
 * date: 2024-06-18 20:24:30
 */

#include <glib.h>

#include "glibconfig.h"
#include "rice_config.h"

#ifndef INCLUDE_CORE_H_
#define INCLUDE_CORE_H_

typedef struct _RicePackageManager RicePackageManager;
typedef struct _RiceCoreInitializer RiceCoreInitializer;
typedef struct _RicePackageDesc RicePackageDesc;
typedef struct _RiceVersion RiceVersion;
typedef enum _RiceVersionComparingType RiceVersionComparingType;

struct _RiceVersion {
    guint8 major;
    guint8 minor;
    guint16 patch;
};

enum _RiceVersionComparingType {
    RICE_VERSION_EQUAL = 0,
    RICE_VERSION_GREATER,
    RICE_VERSION_GREATER_OR_EQUAL,
    RICE_VERSION_LESS,
    RICE_VERSION_LESS_OR_EQUAL,
};

__RICE_DECL gboolean rice_version_verify(RiceVersion version);
__RICE_DECL gboolean rice_version_verify_exact(RiceVersion version);
__RICE_DECL gboolean rice_version_parse(gchar *str, RiceVersion *out_version);

typedef struct RiceRuntimeCore {
    RicePackageManager *package_manager;
} RiceRuntimeCore;

__RICE_DECL RiceRuntimeCore *rice_core_new();
__RICE_DECL void rice_core_run(RiceRuntimeCore *core);

struct _RiceCoreInitializer {
    rice_array_t (*get_packages)();
    rice_array_t (*get_packages_search_paths)();
};

struct _RicePackageDesc {
    gchar *name;
    RiceVersion version;
};

struct _RiceDependency {
    gchar *name;
    RiceVersionComparingType comparingType;
    RiceVersion version;
};

#endif