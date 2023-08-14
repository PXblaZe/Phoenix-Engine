#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define strdupa(str) strcpy((char*)alloca(strlen(str) + 1), (str))
    #define strdup(str) _strdup(str)
    namespace std {
        template <typename T, typename... Types>
        struct __is_one_of : std::disjunction<std::is_same<T, Types>...> {};
    }
    ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif