
#if defined(_WIN32) || defined(_WIN64)

#include <stdio.h>
#include <stdlib.h>

    
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        errno = EINVAL;
        return -1;
    }

    // Use a reasonable initial buffer size if *lineptr is NULL or *n is 0
    if (*lineptr == NULL || *n == 0) {
        *n = 256;  // Start with a larger initial buffer
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }

    size_t pos = 0;
    while (1) {
        size_t bytes_read = fread(*lineptr + pos, 1, *n - pos, stream);
        if (bytes_read == 0) {
            if (pos == 0) {
                return -1;  // No data read
            } else {
                break;  // End of file, but we have some data
            }
        }

        pos += bytes_read;

        // Check for the end of line or buffer full
        int found_newline = 0;
        for (size_t i = pos - bytes_read; i < pos; i++) {
            if ((*lineptr)[i] == '\n') {
                found_newline = 1;
                break;
            }
        }

        if (found_newline) {
            break;  // End of line
        }

        if (pos == *n) {
            // Buffer full, double the buffer size
            *n *= 2;
            char *new_lineptr = (char *)realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                return -1;  // Memory allocation failed
            }
            *lineptr = new_lineptr;
        }
    }

    (*lineptr)[pos] = '\0';  // Null-terminate the string
    return pos;
}

#endif