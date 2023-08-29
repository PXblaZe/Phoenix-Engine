#pragma once

#include <vector>
#include <cstdlib>

namespace px {

class VertexBuffer {

    unsigned int vboID;

public:

    const unsigned int bufferSize;

    VertexBuffer(unsigned int size, const void* data = nullptr);
    VertexBuffer(unsigned int size, unsigned int glUsage, const void* data = nullptr);
    ~VertexBuffer();

    void updateBufferData(const void* data, unsigned int offset = 0, size_t size = 0) const;

    void getBufferData(void* data, unsigned int offset = 0, size_t size = 0) const;

    void bind() const;

    static void unbind();

};

class ElementBuffer {

    unsigned int iboID;
    unsigned int count;
    unsigned int type_size;

    void create(unsigned int type_size, unsigned int count, const void* data);

    template<typename... Args>
    static constexpr bool isIntegrals = (std::is_integral_v<Args> && ...);

    template <typename First, typename... Next>
    inline constexpr unsigned int sizeofFirstDataType(First first, Next... args) { return sizeof(First); }

    template <typename First, typename... Next>
    inline constexpr const First* firstPointer(First first, Next... args) const
    { 
        First* pData = (First*)malloc(sizeof(First)*((sizeof...(args))+1));
        unsigned int i = 1; pData[0] = first;
        ((pData[i++] = args), ...);
        return pData;
    }

public:

    template<typename _Tp>
    explicit constexpr ElementBuffer(unsigned int count, const _Tp* data) { create(sizeof(_Tp), count, data); }

    template<typename... Args, typename = std::enable_if_t<isIntegrals<Args...>>>
    constexpr ElementBuffer(Args... data)
    {
        auto* pData = firstPointer(data...);
        create(sizeofFirstDataType(data...), sizeof...(Args), pData);
        free((void*)pData);
    }

    ~ElementBuffer();

    void updateBufferData(const void* data, unsigned int offset = 0, size_t size = 0) const;

    void getBufferData(void* data, unsigned int offset = 0, size_t size = 0) const;

    const unsigned int elementCount() const noexcept;

    const unsigned int draw_callType() const noexcept;

    void bind() const;

    static void unbind();

};

}