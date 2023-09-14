#pragma once

#if defined(_WIN32) || defined(_WIN64)

    #include <type_traits>

    #ifdef _WIN64
        typedef __int64 ssize_t; 
    #else
        typedef long ssize_t;
    #endif

    #define strdupa(str) strcpy((char*)alloca(strlen(str) + 1), (str))
    #define strdup(str) _strdup(str)
    #define strtok_r(a, b, c) strtok_s(a, b, c)
    namespace std {
        template <typename T, typename... Types>
        struct __is_one_of : std::disjunction<std::is_same<T, Types>...> {};
    }
    ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif