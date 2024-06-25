#pragma once
#include <iostream>
#include <Windows.h>

class MemoryManager {
public:
    static MemoryManager& getInstance();

    void allocateMemory(int size);
    void initializeMemory();
    void printMemory();
    void deallocateMemory();
    void writeToMemory(int index, int value);

private:
    MemoryManager();
    ~MemoryManager();

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    int* array;
    int size;
};
