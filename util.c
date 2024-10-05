#include <malloc.h>
#include <string.h>

/**
 * Given a string list and a new string, it reallocs the list and pushes the new string at the end.
 *
 * @param strlist The string list
 * @param s The new string
 * @param list_size Byte size of the string list
 * @param list_len List length. Amount of strings inside the string list.
 */
char **strpush(char **strlist, const char *s, size_t *list_size, size_t *list_len)
{
    size_t newsize = sizeof(char) * (strlen(s));
    if (!strlist)
        strlist = (char **)malloc(newsize);
    else
        strlist = (char **)realloc(strlist, *list_size + newsize);
    strlist[*list_len] = malloc(newsize);
    strcpy(strlist[(*list_len)++], s);
    (*list_size) += newsize;
    return strlist;
}

/**
 * Copy the given string, allocating the required space.
 */
char *strcopy(const char *source)
{
    char *dest = malloc(sizeof(char) * strlen(source));
    strcpy(dest, source);
    return dest;
}
