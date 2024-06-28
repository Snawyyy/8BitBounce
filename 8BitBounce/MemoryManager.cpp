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
    array = new int[size];
    this->size = size;
}

void MemoryManager::InitializeMemory() 
{
    for (int i = 0; i < size; ++i) 
    {
        array[i] = NULL;
    }
}

void MemoryManager::PrintMemory() 
{
    for (int i = 0; i < size; ++i) 
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int MemoryManager::ReadMemory(int index)
{
    return array[index];
}

void MemoryManager::DeallocateMemory() 
{
    delete[] array;
    array = nullptr;
    size = 0;
}

void MemoryManager::WriteToMemory(int index, int value) 
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