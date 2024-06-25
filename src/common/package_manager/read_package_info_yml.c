#include "glib.h"
#include "glibconfig.h"
#include "package.h"
#include "rice/rice.h"

#include "macros.h"

#include <archive.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
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

typedef struct {
    yaml_parser_t parser;
    yaml_event_t event;
    RicePackage *out_package;
    // add required keys
} ParseData;

void parse_data_init(ParseData *data, struct archive *archive, RicePackage *out_package) {
    if (!yaml_parser_initialize(&data->parser)) {
        g_printerr("FATAL: cat't initiaize logger");
        exit(EXIT_FAILURE);
    }

    yaml_parser_set_input(&data->parser, read_handler_archive_yml, archive);

    data->event.type = YAML_NO_EVENT;
    data->out_package = out_package;
}

void parse_data_clean(ParseData *data) {
    if (data->event.type != YAML_NO_EVENT) {
        yaml_event_delete(&data->event);
    }

    yaml_parser_delete(&data->parser);
}

void parse_data_parse_next(ParseData *data) {
    if (data->event.type != YAML_NO_EVENT) {
        yaml_event_delete(&data->event);
    }
    yaml_parser_parse(&data->parser, &data->event);
}

gchar *parse_data_steal_scalar(ParseData *data) {
    gchar *result = (gchar *)data->event.data.scalar.value;
    data->event.data.scalar.value = NULL;
    return result;
}

NODISCARD ReadPackageResult read_package_info_name(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    parse_data_parse_next(parse_data);

    // set package name
    parse_data->out_package->name = parse_data_steal_scalar(parse_data);

    return result;
}

NODISCARD ReadPackageResult read_package_info_version(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    parse_data_parse_next(parse_data);

    gchar *version = (gchar *)parse_data->event.data.scalar.value;
    
    // try parse version
    if(!rice_version_parse(version, &parse_data->out_package->version)) {
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_VERSION;
    }

    return result;
}

NODISCARD ReadPackageResult read_package_info_provides(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    parse_data_parse_next(parse_data);
    // TODO: add provides parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_type(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    parse_data_parse_next(parse_data);
    // TODO: add type parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_entrypoints(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    parse_data_parse_next(parse_data);
    // TODO: add entrypoints parsing

    return result;
}

NODISCARD ReadPackageResult read_package_info_dependencies(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;
    parse_data_parse_next(parse_data);
    // TODO: add dependencies parsing

    return result;
}

char *root_level_keys[] = {
    "name",         //
    "version",      //
    "provides",     //
    "type",         //
    "entrypoints",  //
    "dependencies", //
    "resources",    //
};

typedef enum {
    KEY_NAME = 0,
    KEY_VERSION,
    KEY_PROVIDES,
    KEY_TYPE,
    KEY_ENTRYPOINTS,
    KEY_DEPENDENCIES,
    KEY_RESOURCES,
    KEY_UNDEFINED = -1,
} RootLevelKeys;

NODISCARD ReadPackageResult read_package_info_root_level_key(ParseData *parse_data) {
    RootLevelKeys key = KEY_UNDEFINED;

    for (uint i = 0; i < SIZEOF_ARR(root_level_keys); i++) {
        // compare key
        if (compare_event(&parse_data->event, root_level_keys[i])) {
            key = i;
            break;
        }
    }

    switch (key) {

    case KEY_NAME:
        return read_package_info_name(parse_data);
    case KEY_VERSION:
        return read_package_info_version(parse_data);
    case KEY_PROVIDES:
        return read_package_info_provides(parse_data);
    case KEY_TYPE:
        return read_package_info_type(parse_data);
    case KEY_ENTRYPOINTS:
        return read_package_info_entrypoints(parse_data);
    case KEY_DEPENDENCIES:
        return read_package_info_dependencies(parse_data);

    default:
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT;
    }

    return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT;
}

NODISCARD ReadPackageResult read_package_info_root_level(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;

    parse_data_parse_next(parse_data);
    switch (parse_data->event.type) {
    case YAML_SCALAR_EVENT: {
        result = read_package_info_root_level_key(parse_data);
    } break;

    default:
        result = READ_PACKAGE_RESULT_ERR_PACKAGEINFO_UNEXPECTED_STATEMENT;
        break;
    }

    return result;
}

NODISCARD ReadPackageResult read_package_info_document(ParseData *parse_data) {
    ReadPackageResult result = READ_PACKAGE_RESULT_OK;

    parse_data_parse_next(parse_data);
    if (parse_data->event.type != YAML_DOCUMENT_START_EVENT)
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_DOCUMENT;
    do {
        result = read_package_info_root_level(parse_data);
    } while (parse_data->event.type != YAML_DOCUMENT_END_EVENT);

    parse_data_parse_next(parse_data); // eat document end event

    return result;
}

NODISCARD ReadPackageResult read_package_info_yml(struct archive *archive, RicePackage *out_package) {
    ParseData parse_data;

    parse_data_init(&parse_data, archive, out_package);

    parse_data_parse_next(&parse_data);

    if (parse_data.event.type != YAML_STREAM_START_EVENT)
        return READ_PACKAGE_RESULT_ERR_PACKAGEINFO_BAD_STREAM;

    do {
        ReadPackageResult result = read_package_info_document(&parse_data);

        if (result != READ_PACKAGE_RESULT_OK) {
            // TODO:: add error details
            g_printerr("Error while reading package");
            return result;
        }

    } while (parse_data.event.type != YAML_STREAM_END_EVENT);

    parse_data_clean(&parse_data);

    return READ_PACKAGE_RESULT_OK;
}