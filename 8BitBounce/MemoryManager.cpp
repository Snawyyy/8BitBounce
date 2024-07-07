#include "MemoryManager.h"

MemoryManager& MemoryManager::getInstance() 
{
    static MemoryManager instance;
    return instance;
}

MemoryManager::MemoryManager() : array(nullptr), size(0) {}

MemoryManager::~MemoryManager() 
{
    delete[] array;
}

void MemoryManager::AllocateMemory(int size) 
{
    array = new physicsObj[size];
    this->size = size;
}

void MemoryManager::InitializeMemory() 
{
    for (int i = 0; i < size; ++i) 
    {
        array[i].pos.x = NULL;
        array[i].pos.y = NULL;
    }
}

void MemoryManager::PrintMemory() 
{
    for (int i = 0; i < size; ++i) 
    {
        std::cout << array[i].pos.y << " ";
        std::cout << array[i].pos.x << " ";
    }
    std::cout << std::endl;
}

physicsObj MemoryManager::ReadMemory(int index)
{
    return array[index];
}

void MemoryManager::DeallocateMemory() 
{
    delete[] array;
    array = nullptr;
    size = 0;
}

void MemoryManager::WriteToMemory(int index, physicsObj value) 
{
    if (index >= 0 && index < size) 
    {
        array[index] = value;
    }
    else 
    {
        MessageBox(nullptr, L"Index out of bounds", L"", MB_OK);
    }
}