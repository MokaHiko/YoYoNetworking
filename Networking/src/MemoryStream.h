#pragma once

#include "ByteSwap.h"

// TODO: define Serializable Macro for Send, Write, Receive Interface

class OutputMemoryStream
{
public:
    OutputMemoryStream(uint32_t start_capacity = 32);
    ~OutputMemoryStream();

    const char* GetBufferPtr() const { return m_buffer; }
    const uint32_t GetBufferLength() const { return m_head; }

    template<typename T>
    void Write(const T& data)
    {
        static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types");
        Write(&data, sizeof(T));

        if(SocketUtil::StreamEndianType() != SocketUtil::PlatformEndianType())
        {
            T swapped_data = ByteSwap(data);
            Write(&swapped_data, sizeof(T));
        }
    }

    void Write(const void* in_data, size_t bytes_count);
private:
    void ReallocateBuffer(uint32_t new_length);

    char* m_buffer;
    uint32_t m_head;
    uint32_t m_capacity;
};

class InputMemoryStream
{
public:
    InputMemoryStream(char* in_buffer, uint32_t byte_count);
    ~InputMemoryStream();

    const uint32_t GetRemainingBytesSize() const { return m_capacity - m_head; }

    template<typename T>
    void Read(T* out_data)
    {
        static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Read only supports primitive data types");
        Read(out_data, sizeof(T));
    }

    void Read(void* out_data, size_t bytes_count);
private:
    char* m_buffer;
    uint32_t m_head;
    uint32_t m_capacity;
};

class OutputMemoryBitStream
{
public:
    OutputMemoryBitStream(uint32_t start_capacity_in_bits = 256);
    ~OutputMemoryBitStream();

    void WriteBits(const void* in_data, uint32_t bit_count);
private:
    void ReallocateBuffer(uint32_t new_bit_length);

    char* m_buffer;

    uint32_t m_bit_capacity;
    uint32_t m_bit_head;
};