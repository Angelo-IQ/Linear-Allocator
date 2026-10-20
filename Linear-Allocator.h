#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

using namespace std;


class LinearAllocator
{
private:
	uint8_t* m_Buffer;
	size_t m_Capacity;
	size_t m_Offset;

public:

	LinearAllocator(size_t Capacity) : m_Capacity(Capacity), m_Offset(0)
	{
		m_Buffer = new uint8_t[m_Capacity];
	}

	~LinearAllocator()
	{
		delete[] m_Buffer;
	}


	LinearAllocator(const LinearAllocator&) = delete;
	LinearAllocator& operator = (const LinearAllocator&) = delete;


	void* Allocate(size_t size, size_t alignment = alignof(max_align_t))
	{
		size_t currentAddress = reinterpret_cast<size_t>(m_Buffer + m_Offset);

		size_t padding = 0;

		size_t alignmentAddress = (currentAddress + alignment - 1) & ~(alignment - 1);

		padding += alignmentAddress - currentAddress;

		if (m_Offset + padding + size > m_Capacity)
		{
			cerr << "[Linear] Error: Out of memory!\n";
			return nullptr;
		}

		m_Offset += (size + padding);

		return reinterpret_cast<void*>(alignmentAddress);
	}

	void Reset()
	{
		m_Offset = 0;
	}

	size_t const GetUsedMemory() const { return m_Offset; }
	size_t const GetCapacity() const { return m_Capacity; }

};