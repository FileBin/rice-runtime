#include "glib.h"
#include "glibconfig.h"

#include <ctype.h>
#include <rice/rice.h>
#include <string.h>
#include <time.h>

gboolean rice_version_verify(RiceVersion version) {
    if (version.major != RICE_VERSION_MAJOR)
        return FALSE;
    if (version.minor != RICE_VERSION_MINOR)
        return FALSE;
    return TRUE;
}

gboolean rice_version_verify_exact(RiceVersion version) {
    if (version.major != RICE_VERSION_MAJOR)
        return FALSE;
    if (version.minor != RICE_VERSION_MINOR)
        return FALSE;
    if (version.patch != RICE_VERSION_PATCH)
        return FALSE;
    return TRUE;
}

gint replace_char(gchar *str, gchar orig, gchar rep) {
    char *ix = str;
    int n = 0;
    while ((ix = strchr(ix, orig)) != NULL) {
        *(ix++) = rep;
        n++;
    }
    return n;
}

gboolean rice_version_parse(gchar *str, RiceVersion *out_version) {
    if (str == NULL)
        return FALSE;

    if (out_version == NULL)
        return FALSE;

    gchar buffer[16] = {0};
    gchar *begin;
    guint length;

    for (; isspace(*str); str++)
        ; // skip any whitespaces
    for (length = 0; isspace(*str) || str == 0; str++)
        length++; // count region length

    if (length > 15)
        return FALSE;

    memcpy(buffer, begin, length); // copy input string to buffer
    buffer[length] = 0;            // zero terminate string

    gchar *major_ptr = buffer;                     // getting major number
    gchar *minor_ptr = strchr(major_ptr + 1, '.'); // getting minor number
    gchar *patch_ptr = strchr(minor_ptr + 1, '.'); // getting patch number

    if (minor_ptr == NULL)
        return FALSE;
    if (patch_ptr == NULL)
        return FALSE;

    *minor_ptr = 0; // replace dot to zero terminator
    minor_ptr++;    // move to next char

    *patch_ptr = 0; // replace dot to zero terminator
    patch_ptr++;    // move to next char

    if (!g_ascii_string_to_unsigned(major_ptr, 10, 0x0, 0xff, (guint64 *)&out_version->major, NULL)) {
        return FALSE;
    }

    if (!g_ascii_string_to_unsigned(minor_ptr, 10, 0x0, 0xff, (guint64 *)&out_version->minor, NULL)) {
        return FALSE;
    }

    if (!g_ascii_string_to_unsigned(patch_ptr, 10, 0x0, 0xffff, (guint64 *)&out_version->patch, NULL)) {
        return FALSE;
    }

    return TRUE;
}