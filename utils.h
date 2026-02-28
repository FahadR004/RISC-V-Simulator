#ifndef UTILS_H
#define UTILS_H

/**
 * Prints an array of strings with a custom label prefix.
 * @param arr       Array of C strings (NULL-terminated not required, uses count)
 * @param count     Number of elements in the array
 * @param label     Prefix string to print before each index (e.g., "Token")
 */
void print_array(char **arr, int count, const char *label);

#endif