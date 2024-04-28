#include "MemoryStream.h"

#include "SocketUtil.h"

OutputMemoryStream::OutputMemoryStream(uint32_t start_capacity) 
	:m_buffer(nullptr), m_head(0), m_capacity(0)
{
	ReallocateBuffer(start_capacity);
}

OutputMemoryStream::~OutputMemoryStream() 
{
	free(m_buffer);
}

void OutputMemoryStream::Write(const void *in_data, size_t bytes_count) 
{
	uint32_t result_head = m_head + static_cast<uint32_t>(bytes_count);
	if(result_head > m_capacity)
	{
		ReallocateBuffer(std::max(m_capacity * 2, result_head));
	}

	memcpy(m_buffer + m_head, in_data, bytes_count);
	m_head = result_head;
}

void OutputMemoryStream::ReallocateBuffer(uint32_t new_length) 
{
    m_buffer = static_cast<char*>(realloc(m_buffer, new_length));

	assert(m_buffer && "Failed to reallocate buffer!");

	m_capacity = new_length;
}

InputMemoryStream::InputMemoryStream(char *in_buffer, uint32_t byte_count) 
	:m_buffer(in_buffer), m_capacity(byte_count), m_head(0)
{
    assert(m_buffer != nullptr && "Stream buffer is empty or null!");
}

InputMemoryStream::~InputMemoryStream() 
{
	free(m_buffer);
}

void InputMemoryStream::Read(void *out_data, size_t bytes_count) {
    assert(m_buffer != nullptr && "Stream buffer is empty or null!");
	assert(out_data != nullptr && "Cannot read into null out data buffer!");

	uint32_t result_head = m_head + bytes_count;
	assert(result_head <= m_capacity && "Attempting to read more data than capacity of stream buffer!");

	memcpy(out_data, m_buffer + m_head, bytes_count);
	m_head = result_head;
}

OutputMemoryBitStream::OutputMemoryBitStream(uint32_t start_capacity_in_bits) 
	:m_buffer(nullptr), m_bit_capacity(start_capacity_in_bits), m_bit_head(0)
{
}

OutputMemoryBitStream::~OutputMemoryBitStream() 
{
}

void OutputMemoryBitStream::WriteBits(const void* in_data, uint32_t bit_count)
{
	uint32_t result_head = m_bit_head + bit_count;
	if(result_head > m_bit_capacity)
	{
		ReallocateBuffer(std::max(m_bit_capacity * 2, result_head));
	}
}

void OutputMemoryBitStream::ReallocateBuffer(uint32_t new_bit_length) 
{
	if(m_buffer != nullptr)
	{
		char* temp_buffer = static_cast<char*>(malloc(new_bit_length >> 3));
		memset(temp_buffer, 0, new_bit_length >> 3);
		memcpy(temp_buffer, m_buffer, m_bit_capacity >> 3);

		free(m_buffer);
		m_buffer = temp_buffer;
	}
	else
	{
		// memset on first allcation
		m_buffer = static_cast<char*>(malloc(new_bit_length >> 3));
		memset(m_buffer, 0, new_bit_length >> 3);
	}

	assert(m_buffer && "Failed to reallocate buffer!");

	m_bit_capacity = new_bit_length;
}
