#include "stringm.h"
#include <stdlib.h>

/*
** strlen_m calculates the length of a string
** const char *string - string to calculate length of
** return the size of the string
**
** note: you can assume string is not NULL
*/
size_t strlen_m(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0')
    {
        length++;
    }
    return length;
}

/*
** strncpy_m copies n characters of string and returns it
** const char *string - string to copy
** size_t n - number of characters to copy (not including null character)
** return a copy of first n characters of string
**
** note: you can assume string is not NULL
** hint: you will need to malloc a size n + 1 string to accommodate the null character
*/
char *strncpy_m(const char *string, size_t n)
{
    // Allocate memory for the string + 1 for the null character
    char *copy = malloc(n + 1);
    if (copy == NULL)
    {
        return NULL; // Return NULL if malloc fails
    }

    // Copy the first n characters of string
    for (size_t i = 0; i < n; i++)
    {
        copy[i] = string[i];
    }
    // Null-terminate the string
    copy[n] = '\0';
    
    return copy;
}

/*
** join_m joins an array of strings separated by a delimiter
** Strings strings - structure that stores an array of strings
** const char *delimiter - delimiter string which joins each string
** return the string created by joining all strings with the delimiter
**
** note: you can assume delimiter is not NULL
** hint: return NULL if strings.num_strings is 0
*/
char *join_m(Strings strings, const char *delimiter)
{
    if (strings.num_strings == 0)
    {
        return NULL; // Return NULL if there are no strings to join
    }

    // Calculate the total length of the resulting string
    size_t total_length = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        total_length += strlen_m(strings.strings[i]);
    }

    // Add space for delimiters
    total_length += strlen_m(delimiter) * (strings.num_strings - 1);

    // Allocate memory for the joined string
    char *result = malloc(total_length + 1);
    if (result == NULL)
    {
        return NULL; // Return NULL if malloc fails
    }

    // Join the strings with the delimiter
    result[0] = '\0'; // Initialize the result string
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        if (i > 0)
        {
            // Add the delimiter before each string (except the first)
            strcat(result, delimiter);
        }
        strcat(result, strings.strings[i]);
    }

    return result;
}

/*
** free_strings frees all allocated elements in strings
** String strings - free each string in strings.strings and strings.strings itself
*/
void free_strings(Strings strings)
{
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        free(strings.strings[i]); // Free each string
    }
    free(strings.strings); // Free the array of strings
}

/*
** split_m splits a string at any occurrence of pattern
** const char *string - string that is searched for the pattern
** const char *pattern - pattern which string should be split
** return a String structure which contains an array of each string
**
** note: you may assume string and pattern are not NULL
** hint 1: TA solution uses strlen_m, strstr_m, and strncpy_m
** hint 2: first calculate how many strings are needed, which is:
**         (the number of times the delimiter appears + 1)
** hint 3: when trying to store a substring, think about how the length of
**         that substring might be calculated in terms of pointer arithmetic
**         - what is the outcome of adding or subtracting pointers?
** hint 3.5: strstr_m will return a pointer to the first character of the next occurrence
**           or NULL if not found
*/
Strings split_m(const char *string, const char *pattern)
{
    Strings result = {.num_strings = 0, .strings = NULL};

    // Initialize pointer and counter for number of substrings
    const char *temp = string;
    const char *next_pos = strstr_m(temp, pattern);

    // Count the number of substrings
    while (next_pos != NULL)
    {
        result.num_strings++;
        temp = next_pos + strlen_m(pattern);
        next_pos = strstr_m(temp, pattern);
    }
    result.num_strings++;

    // Allocate memory for the substrings
    result.strings = malloc(result.num_strings * sizeof(char *));
    if (result.strings == NULL)
    {
        return result; // Return empty result if malloc fails
    }

    // Extract the substrings
    temp = string;
    next_pos = strstr_m(temp, pattern);
    size_t i = 0;

    while (next_pos != NULL)
    {
        result.strings[i] = strncpy_m(temp, next_pos - temp, strlen_m(temp));
        i++;
        temp = next_pos + strlen_m(pattern);
        next_pos = strstr_m(temp, pattern);
    }
    result.strings[i] = strncpy_m(temp, strlen_m(temp), strlen_m(temp));
    return result;
}

/*
** find_and_replace_all_m finds each occurrence of the pattern in the string and replaces it
** const char *string - string to search through
** const char *pattern - pattern to search for in the string
** const char *replacement - replacement string for each occurrence of the pattern in the string
** return a string in which every occurrence of pattern is replaced replacement
**
** note: you may assume string, pattern, and replacement are all not NULL
** hint: there are two main ways of implementing this function, one involves many lines, one involves four
*/
char *find_and_replace_all_m(const char *string, const char *pattern, const char *replacement)
{
    size_t pattern_len = strlen_m(pattern);
    size_t replacement_len = strlen_m(replacement);

    // Count how many times the pattern appears in the string
    size_t count = 0;
    const char *temp = string;
    const char *next_pos = strstr_m(temp, pattern);

    while (next_pos != NULL)
    {
        count++;
        temp = next_pos + pattern_len;
        next_pos = strstr_m(temp, pattern);
    }

    // Allocate memory for the result string
    size_t result_len = strlen_m(string) - count * pattern_len + count * replacement_len;
    char *result = malloc(result_len + 1);
    if (result == NULL)
    {
        return NULL; // Return NULL if malloc fails
    }

    // Replace the pattern with the replacement in the result string
    temp = string;
    next_pos = strstr_m(temp, pattern);
    char *current_pos = result;

    while (next_pos != NULL)
    {
        size_t prefix_len = next_pos - temp;
        strncpy_m(current_pos, temp, prefix_len);
        current_pos += prefix_len;
        strncpy_m(current_pos, replacement, replacement_len);
        current_pos += replacement_len;
        temp = next_pos + pattern_len;
        next_pos = strstr_m(temp, pattern);
    }

    // Copy the remainder of the string after the last pattern match
    strncpy_m(current_pos, temp, strlen_m(temp));
    result[result_len] = '\0'; // Null-terminate the string

    return result;
}

/*
** The strstr function is implemented for you to use -- DO NOT MODIFY
** If you are curious about the algorithm used, look up the Knuth-Morris-Pratt (KMP)
** algorithm that can find a substring inside another string 'blazingly fast'
*/
const char *strstr_m(const char *haystack, const char *needle)
{
    size_t haystack_len = 0, needle_len = 0;
    for (const char *h = haystack; *h; h++)
        haystack_len++;
    for (const char *n = needle; *n; n++)
        needle_len++;

    if (needle_len > haystack_len)
        return NULL;

    char *lps_str = malloc(haystack_len + needle_len + 1);
    size_t i = 0;
    for (const char *n = needle; *n; n++, i++)
        lps_str[i] = *n;
    lps_str[i++] = '\1';
    for (const char *h = haystack; *h; h++, i++)
        lps_str[i] = *h;

    int *lps_arr = calloc((haystack_len + needle_len + 1), sizeof *lps_arr);
    size_t l = 0, r = 1;
    bool success = false;

    while (r < haystack_len + needle_len + 1)
    {
        if (lps_str[l] == lps_str[r])
        {
            l++;
            lps_arr[r] = l;
            r++;
        }
        else if (l)
            l = lps_arr[l - 1];
        else
        {
            lps_arr[r] = 0;
            r++;
        }

        if (l == needle_len)
        {
            success = true;
            break;
        }
    }

    free(lps_arr);
    free(lps_str);
    if (success)
        return haystack + (r - l - needle_len - 1);
    return NULL;
}
