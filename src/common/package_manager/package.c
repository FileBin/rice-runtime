#include <package.h>
#include <time.h>

const char *get_package_format_file_extension(RicePackageFormat format) {
    switch (format) {
    case RICE_PACKAGE_FORMAT_NRP:
        return ".nrp";
    case RICE_PACKAGE_FORMAT_URP:
        return ".urp";
    case RICE_PACKAGE_FORMAT_RRP:
        return ".rrp";
    case RICE_PACKAGE_FORMAT_MAX:
        break;
    }
    return NULL;
}

void rice_package_set_name(RicePackage *package, gchar *name) {
    char hasProvider = package->provides != 0 && package->provides != package->name;
    package->name = name;
    if (!hasProvider)
        package->provides = name;
}