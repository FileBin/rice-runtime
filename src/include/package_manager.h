/*
 * file: package_manager.h
 * author: filebin
 * date: 2024-06-18 20:37:37
 */

#ifndef INCLUDE_PACKAGE_MANAGER_H_
#define INCLUDE_PACKAGE_MANAGER_H_

#include "rice/rice_package.h"
#include <glib.h>

typedef struct _RicePackageManager RicePackageManager;

struct _RicePackageManager {
    GArray loaded_packages;
    GArray search_paths;
};

void rice_package_manager_load_package(RicePackageManager *packageManager, const RicePackageDesc *desc);
void rice_package_manager_add_package(RicePackageManager *packageManager, RicePackage *package);

#endif