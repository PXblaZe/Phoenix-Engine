#pragma once

#include <vector>
#include <functional>
namespace px {

class VertexBufferLayout {

    struct VertexArrayAttrib {
        unsigned int type;
        unsigned int count;
        unsigned int type_size;
        unsigned char normalized;
        char* name = "";
    };

    unsigned int stride;
    std::vector<struct VertexBufferLayout::VertexArrayAttrib> layout;

    inline void insert(unsigned int type, unsigned int count, unsigned char type_size, unsigned char normalized)
    {
        layout.push_back({type, count, type_size, normalized});
    }

    void pushf(unsigned int count, bool normalized);
    void pushd(unsigned int count, bool normalized);
    void pushb(unsigned int count, bool normalized);
    void pushub(unsigned int count, bool normalized);
    void pushi(unsigned int count, bool normalized);
    void pushui(unsigned int count, bool normalized);
    void pushs(unsigned int count, bool normalized);
    void pushus(unsigned int count, bool normalized);

    void va_pushf(unsigned int count, const char* vaname, bool normalized);
    void va_pushd(unsigned int count, const char* vaname, bool normalized);
    void va_pushb(unsigned int count, const char* vaname, bool normalized);
    void va_pushub(unsigned int count, const char* vaname, bool normalized);
    void va_pushi(unsigned int count, const char* vaname, bool normalized);
    void va_pushui(unsigned int count, const char* vaname, bool normalized);
    void va_pushs(unsigned int count, const char* vaname, bool normalized);
    void va_pushus(unsigned int count, const char* vaname, bool normalized);

public:

    VertexBufferLayout();

    template<typename _Tp>
    inline constexpr void push(unsigned int count, bool normalized = false) 
    {
        if constexpr (std::is_same<_Tp, float>::value) pushf(count, normalized);
        else if constexpr (std::is_same<_Tp, double>::value) pushd(count, normalized);
        else if constexpr (std::is_same<_Tp, signed char>::value) pushb(count, normalized);
        else if constexpr (std::is_same<_Tp, unsigned char>::value) pushub(count, normalized);
        else if constexpr (std::is_same<_Tp, signed int>::value) pushi(count, normalized);
        else if constexpr (std::is_same<_Tp, unsigned int>::value) pushui(count, normalized);
        else if constexpr (std::is_same<_Tp, signed short>::value) pushs(count, normalized);
        else if constexpr (std::is_same<_Tp, unsigned short>::value) pushus(count, normalized);
        else throw std::bad_function_call();
    }

    template<typename _Tp>
    inline constexpr void push(unsigned int count, const char* attribute_name, bool normalized = false) 
    {
        if constexpr (std::is_same<_Tp, float>::value) va_pushf(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, double>::value) va_pushd(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, signed char>::value) va_pushb(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, unsigned char>::value) va_pushub(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, signed int>::value) va_pushi(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, unsigned int>::value) va_pushui(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, signed short>::value) va_pushs(count, attribute_name, normalized);
        else if constexpr (std::is_same<_Tp, unsigned short>::value) va_pushus(count, attribute_name, normalized);
        else throw std::bad_function_call();
    }

    inline unsigned int getStride() const noexcept  { return stride; }

    inline const std::vector<struct VertexBufferLayout::VertexArrayAttrib>& operator()() const noexcept
    {
        return layout;
    }

};

} // namespace px