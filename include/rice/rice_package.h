/*
 * file: package.h
 * author: filebin
 * date: 2024-06-18 21:01:11
 */

#include "rice_config.h"

#ifndef INCLUDE_RICE_PACKAGE_H_
#define INCLUDE_RICE_PACKAGE_H_

#include <glib.h>

typedef struct _RicePackage RicePackage;
typedef struct _RicePackageDesc RicePackageDesc;
typedef enum _RicePlatform RicePlatform;
typedef enum _RiceArchitecture RiceArchitecture;

enum _RiceArchitecture {
    RICE_ARCHITECTURE_ANY = 0xffffffff,
    RICE_ARCHITECTURE_X86 = 1 << 0,
    RICE_ARCHITECTURE_X86_64 = 1 << 1,
    RICE_ARCHITECTURE_ARM32 = 1 << 2,
    RICE_ARCHITECTURE_ARM64 = 1 << 3,
};

enum _RicePlatform {
    RICE_PLATFORM_ANY = 0xffffffff,
    RICE_PLATFORM_WINDOWS = 1 << 0,
    RICE_PLATFORM_LINUX = 1 << 1,
    RICE_PLATFORM_MAC = 1 << 2,
    RICE_PLATFORM_ANDROID = 1 << 3,
};

struct _RicePackageDesc {
    gchar *name;
    gchar *version;
};

struct _RicePackage {
    RicePackageDesc info;
    GArray dependencies;
    RicePlatform platform;
    RiceArchitecture architecture;
};

#endif