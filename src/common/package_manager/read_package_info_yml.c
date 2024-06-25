#include "glib.h"
#include "glibconfig.h"
#include "package.h"
#include "rice/rice_package.h"

#include "macros.h"

#include <archive.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <yaml.h>

int read_handler_archive_yml(void *archive, unsigned char *buffer, size_t size, size_t *size_read) {
    ssize_t read_bytes = archive_read_data(archive, buffer, size);

    if (read_bytes >= 0) {
        *size_read = read_bytes;
        return 1;
    }

    return 0;
}

gboolean compare_event(yaml_event_t *event, const char *str) {
    if (memcmp(event->data.scalar.value, str, event->data.scalar.length) == 0) {
        return TRUE;
    }
    return FALSE;
}

NODISCARD ReadPackageResult read_package_info_name(yaml_parser_t *parser, yaml_event_t *out_event,
                                                   RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    yaml_parser_parse(parser, out_event);
    // TODO: add name parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_version(yaml_parser_t *parser, yaml_event_t *out_event,
                                                      RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    yaml_parser_parse(parser, out_event);
    // TODO: add version parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_provides(yaml_parser_t *parser, yaml_event_t *out_event,
                                                       RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    yaml_parser_parse(parser, out_event);
    // TODO: add provides parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_type(yaml_parser_t *parser, yaml_event_t *out_event,
                                                   RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    yaml_parser_parse(parser, out_event);
    // TODO: add type parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_entrypoints(yaml_parser_t *parser, yaml_event_t *out_event,
                                                          RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    yaml_parser_parse(parser, out_event);
    // TODO: add entrypoints parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_dependencies(yaml_parser_t *parser, yaml_event_t *out_event,
                                                           RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    yaml_parser_parse(parser, out_event);
    // TODO: add dependencies parsing

    return result;
}

char *root_level_keys[] = {"name", "version", "provides", "type", "entrypoints", "dependencies"};
typedef enum {
    KEY_NAME,
    KEY_VERSION,
    KEY_PROVIDES,
    KEY_TYPE,
    KEY_ENTRYPOINTS,
    KEY_DEPENDENCIES,
    KEY_UNDEFINED = -1
} RootLevelKeys;

NODISCARD ReadPackageResult read_package_info_root_level_key(yaml_parser_t *parser, yaml_event_t *in_out_event,
                                                             RicePackage *out_package) {
    RootLevelKeys key = KEY_UNDEFINED;

    for (uint i = 0; i < SIZEOF_ARR(root_level_keys); i++) {
        // compare key
        if (compare_event(in_out_event, root_level_keys[i])) {
            key = i;
            break;
        }
    }
    yaml_event_delete(in_out_event);

    switch (key) {

    case KEY_NAME:
        return read_package_info_name(parser, in_out_event, out_package);
    case KEY_VERSION:
        return read_package_info_version(parser, in_out_event, out_package);
    case KEY_PROVIDES:
        return read_package_info_provides(parser, in_out_event, out_package);
    case KEY_TYPE:
        return read_package_info_type(parser, in_out_event, out_package);
    case KEY_ENTRYPOINTS:
        return read_package_info_entrypoints(parser, in_out_event, out_package);
    case KEY_DEPENDENCIES:
        return read_package_info_dependencies(parser, in_out_event, out_package);

    default:
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT;
    }

    return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT;
}

NODISCARD ReadPackageResult read_package_info_root_level(yaml_parser_t *parser, yaml_event_t *out_event,
                                                         RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;

    yaml_parser_parse(parser, out_event);
    switch (out_event->type) {
    case YAML_SCALAR_EVENT: {
        result = read_package_info_root_level_key(parser, out_event, out_package);
    } break;

    default:
        result = READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT;
        break;
    }

    return result;
}

NODISCARD ReadPackageResult read_package_info_document(yaml_parser_t *parser, yaml_event_t *out_event,
                                                       RicePackage *out_package) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;

    yaml_parser_parse(parser, out_event);
    if (out_event->type != YAML_DOCUMENT_START_EVENT)
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_DOCUMENT;
    do {
        yaml_event_delete(out_event);
        result = read_package_info_root_level(parser, out_event, out_package);

    } while (out_event->type != YAML_DOCUMENT_END_EVENT);

    return result;
}

NODISCARD ReadPackageResult read_package_info_yml(struct archive *archive, RicePackage *out_package) {

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser!\n", stderr);

    yaml_parser_set_input(&parser, read_handler_archive_yml, archive);

    yaml_parser_parse(&parser, &event);

    if (event.type != YAML_STREAM_START_EVENT)
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_STREAM;

    do {
        yaml_event_delete(&event);

        ReadPackageResult result = read_package_info_document(&parser, &event, out_package);

        if (result != READ_PACKAGE_RESULT_OK) {
            // TODO:: add error details
            g_printerr("Error while reading package");
            return result;
        }

    } while (event.type != YAML_STREAM_END_EVENT);

    yaml_parser_delete(&parser);

    return READ_PACKAGE_RESULT_OK;
}