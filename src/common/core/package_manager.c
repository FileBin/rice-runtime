/*
 * file: package-manager.c
 * author: filebin
 * date: 2024-06-18 20:40:49
 */
#include "package_manager.h"
#include "rice_config.h"
#include <glib.h>
#include <string.h>

#include "rice/rice_package.h"

void PLATFORM_read_package(gchar *path, RicePackage *out_package);

gint compare_desc_with_package_by_name(const RicePackage *ricePackage, const RicePackageDesc *ricePackageDesc) {
    return strcmp(ricePackage->info.name, ricePackageDesc->name);
}

RicePackage *load_package(RicePackageManager *packageManager, const RicePackageDesc *desc) {
    guint n = packageManager->search_paths.len;
    for (guint i = 0; i < n; i++) {
        gchar *directory = g_array_index(&packageManager->search_paths, gchar *, i);
        gchar path[RICE_PATH_MAX];

        strcpy(path, directory);
        strcat(path, desc->name);

        if (g_file_test(path, G_FILE_TEST_EXISTS)) {
            RicePackage *pkg = g_malloc(sizeof(RicePackage));
        }
    }
}

RicePackage *find_package(RicePackageManager *packageManager, const RicePackageDesc *desc) {
    GArray *loaded_packages = &packageManager->loaded_packages;
    guint package_index;
    gboolean found =
        g_array_binary_search(loaded_packages, desc, (GCompareFunc)compare_desc_with_package_by_name, &package_index);

    return &g_array_index(loaded_packages, RicePackage, package_index);
}

void rice_package_manager_load_package(RicePackageManager *packageManager, const RicePackageDesc *desc) {
    find_package(packageManager, desc);
}