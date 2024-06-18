/*
 * file: core_initializer.h
 * author: filebin
 * date: 2024-06-18 20:49:13
 */

#ifndef EXPORT_RICE_CORE_INITIALIZER_H_
#define EXPORT_RICE_CORE_INITIALIZER_H_

#include <glib.h>

typedef struct _RiceCoreInitializer RiceCoreInitializer;

struct _RiceCoreInitializer {
    GArray (*get_packages)();
    GArray (*get_packages_search_paths)();
};

#endif