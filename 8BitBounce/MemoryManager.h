#pragma once
#include <iostream>
#include <Windows.h>

class MemoryManager {
public:
    static MemoryManager& getInstance();

    void AllocateMemory(int size);
    void InitializeMemory();
    void PrintMemory();
    int ReadMemory(int index);
    void DeallocateMemory();
    void WriteToMemory(int index, int value);

private:
    MemoryManager();
    ~MemoryManager();

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    int* array;
    int size;
};
