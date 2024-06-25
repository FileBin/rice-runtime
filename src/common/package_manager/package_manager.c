/*
 * file: package-manager.c
 * author: filebin
 * date: 2024-06-18 20:40:49
 */

#include <glib.h>
#include <string.h>
#include <time.h>

#include "rice/rice_package.h"

#include "package.h"
#include "package_manager.h"
#include "rice_private_config.h"

gboolean PLATFORM_read_native_package(gchar *path, RicePackage *out_package);

gint compare_desc_with_package_by_name(const RicePackage *ricePackage, const RicePackageDesc *ricePackageDesc) {
    return strcmp(ricePackage->info.name, ricePackageDesc->name);
}

RicePackage *load_package(RicePackageManager *packageManager, const RicePackageDesc *desc) {
    guint n = packageManager->search_paths.len;
    for (guint i = 0; i < n; i++) {
        gchar *directory = g_array_index(&packageManager->search_paths, gchar *, i);
        gchar path[RICE_PATH_MAX];

        for (RicePackageFormat fmt = RICE_PACKAGE_FORMAT_NRP; fmt < RICE_PACKAGE_FORMAT_MAX; fmt++) {
            strcpy(path, directory);
            strcat(path, desc->name);

            const char *ext = get_package_format_file_extension(fmt);
            strcat(path, ext);

            if (g_file_test(path, G_FILE_TEST_IS_REGULAR)) {
                continue;
            }

            RicePackage *pkg = g_malloc(sizeof(RicePackage));
            ReadPackageResult result = PLATFORM_read_native_package(path, pkg);

            if (result != READ_PACKAGE_RESULT_OK) {
                // TODO: add error handling
            }

            return pkg;
        }
    }
    return NULL;
}

RicePackage *find_package(RicePackageManager *packageManager, const RicePackageDesc *desc) {
    GArray *loaded_packages = &packageManager->loaded_packages;
    guint package_index;
    gboolean found =
        g_array_binary_search(loaded_packages, desc, (GCompareFunc)compare_desc_with_package_by_name, &package_index);

    if (!found)
        return NULL;

    return &g_array_index(loaded_packages, RicePackage, package_index);
}

void rice_package_manager_load_package(RicePackageManager *packageManager, const RicePackageDesc *desc) {
    if (!find_package(packageManager, desc)) {
        RicePackage *package = load_package(packageManager, desc);

        rice_package_manager_add_package(packageManager, package);
    }
}