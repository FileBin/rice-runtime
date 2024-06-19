/*
 * file: core_initializer.h
 * author: filebin
 * date: 2024-06-18 20:49:13
 */

#include "rice_config.h"

#ifndef INCLUDE_RICE_CORE_INITIALIZER_H_
#define INCLUDE_RICE_CORE_INITIALIZER_H_

#include <glib.h>

typedef struct _RiceCoreInitializer RiceCoreInitializer;

struct _RiceCoreInitializer {
    GArray (*get_packages)();
    GArray (*get_packages_search_paths)();
};

#endif