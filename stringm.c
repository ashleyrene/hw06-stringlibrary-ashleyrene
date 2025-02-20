#include "stringm.h"
#include <stdlib.h>

// Returns the length of the string
size_t strlen_m(const char *str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

// Copies up to n characters from source to destination
char *strncpy_m(char *destination, const char *source, size_t n) {
    size_t i;
    for (i = 0; i < n && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    while (i < n) {
        destination[i++] = '\0';
    }
    return destination;
}

// Concatenates two strings with a separator
char *join_m(const char *str1, const char *str2, char sep) {
    size_t len1 = strlen_m(str1);
    size_t len2 = strlen_m(str2);
    char *result = (char *)malloc(len1 + len2 + 2);
    if (!result) return NULL;
    
    strncpy_m(result, str1, len1);
    result[len1] = sep;
    strncpy_m(result + len1 + 1, str2, len2);
    result[len1 + len2 + 1] = '\0';
    return result;
}

// Splits a string into an array of strings based on a separator
char **split_m(const char *str, char sep) {
    if (!str) return NULL;
    
    size_t count = 1;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == sep) count++;
    }
    
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    size_t start = 0, index = 0;
    for (size_t i = 0; ; i++) {
        if (str[i] == sep || str[i] == '\0') {
            size_t len = i - start;
            result[index] = (char *)malloc(len + 1);
            if (!result[index]) {
                free_strings(result);
                return NULL;
            }
            strncpy_m(result[index], str + start, len);
            result[index][len] = '\0';
            index++;
            start = i + 1;
            if (str[i] == '\0') break;
        }
    }
    result[count] = NULL;
    return result;
}

// Frees the memory allocated for an array of strings
void free_strings(char **str_array) {
    if (!str_array) return;
    for (size_t i = 0; str_array[i] != NULL; i++) {
        free(str_array[i]);
    }
    free(str_array);
}

// Replaces all occurrences of find in source with replace
char *find_and_replace_all_m(const char *source, const char *find, const char *replace) {
    if (!source || !find || !replace) return NULL;
    
    size_t find_len = strlen_m(find);
    size_t replace_len = strlen_m(replace);
    
    size_t count = 0;
    for (size_t i = 0; source[i] != '\0'; i++) {
        if (source[i] == find[0]) {
            size_t match = 1;
            for (size_t j = 1; j < find_len; j++) {
                if (source[i + j] != find[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) count++;
        }
    }
    
    size_t new_len = strlen_m(source) + count * (replace_len - find_len);
    char *result = (char *)malloc(new_len + 1);
    if (!result) return NULL;
    
    size_t i = 0, j = 0;
    while (source[i] != '\0') {
        if (source[i] == find[0]) {
            size_t match = 1;
            for (size_t k = 1; k < find_len; k++) {
                if (source[i + k] != find[k]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                for (size_t k = 0; k < replace_len; k++) {
                    result[j++] = replace[k];
                }
                i += find_len;
                continue;
            }
        }
        result[j++] = source[i++];
    }
    result[j] = '\0';
    return result;
}
