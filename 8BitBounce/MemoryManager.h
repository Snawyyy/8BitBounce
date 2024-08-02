#pragma once
#include <iostream>
#include <Windows.h>
#include "RigidBody.h"

class MemoryManager {
public:
    static MemoryManager& getInstance();

    void AllocateMemory(int size);
    void InitializeMemory();
    void PrintMemory();
    physicsObj ReadMemory(int index);
    void DeallocateMemory();
    void WriteToMemory(int index, physicsObj value);

private:
    MemoryManager();
    ~MemoryManager();

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    physicsObj* array;
    int size;
};
