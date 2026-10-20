# C++ Linear Allocator (Arena Allocator)

A lightweight, high-performance **Linear Memory Allocator** written in C++11/C++17. This project demonstrates custom memory management using low-level pointer arithmetic and bitwise alignment operations.

---

## 📌 Overview

The **Linear Allocator** pre-allocates a contiguous block of heap memory and satisfies allocation requests sequentially using a top-offset pointer. It features custom bitwise address alignment handling to meet hardware and type-specific alignment constraints efficiently.

### Key Features
* **Zero Fragmentation**: Allocations are strictly linear within a contiguous buffer.
* **Fast Allocations**: Constant time $\mathcal{O}(1)$ allocation overhead by advancing a single offset index.
* **Bitwise Memory Alignment**: Utilizes fast bitwise mask operations `(address + align - 1) & ~(align - 1)` to handle variable byte alignment constraints (e.g., 4-byte, 8-byte, 16-byte).
* **RAII & Safety**: Manages buffer lifecycle via RAII and explicitly disables copy semantics (`operator=` and copy constructor) to prevent double-free bugs.

---

## 🛠️ How It Works

1. **Initialization**: The allocator allocates a fixed-capacity byte buffer (`uint8_t*`) on the heap and initializes `m_Offset` to `0`.
2. **Allocation**:
   * Takes the current raw memory address: `currentAddress = m_Buffer + m_Offset`.
   * Aligns `currentAddress` upward using bitwise padding calculations.
   * Computes the required padding bytes: `padding = alignmentAddress - currentAddress`.
   * Checks for buffer overflow bounds (`m_Offset + padding + size <= m_Capacity`).
   * Updates `m_Offset += size + padding` and returns the aligned pointer.
3. **Reset**: Resets `m_Offset = 0` to reuse the pre-allocated memory instantly in $\mathcal{O}(1)$ time without calling free/delete on individual pointers.

---

## 🚀 Getting Started

### Inclusion
Simply include the header file in your project:

```cpp
#include "LinearAllocator.h"


#include <iostream>
#include "LinearAllocator.h"

struct Vector3 {
    float x, y, z;
};

int main() {
    // Pre-allocate a 1 KB arena
    LinearAllocator allocator(1024);

    // Allocate an integer (aligned to 4 bytes)
    int* myInt = static_cast<int*>(allocator.Allocate(sizeof(int), alignof(int)));
    *myInt = 42;

    // Allocate a custom struct (aligned to type alignment requirements)
    Vector3* vec = static_cast<Vector3*>(allocator.Allocate(sizeof(Vector3), alignof(Vector3)));
    vec->x = 1.0f; vec->y = 2.0f; vec->z = 3.0f;

    std::cout << "Used Memory: " << allocator.GetUsedMemory() << " bytes\n";
    std::cout << "Capacity: "    << allocator.GetCapacity()   << " bytes\n";

    // Reset the allocator to reuse all memory
    allocator.Reset();

    return 0;
}
