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

void MemoryManager::allocateMemory(int size) 
{
    array = new int[size];
    this->size = size;
}

void MemoryManager::initializeMemory() 
{
    for (int i = 0; i < size; ++i) 
    {
        array[i] = NULL;
    }
}

void MemoryManager::printMemory() 
{
    for (int i = 0; i < size; ++i) 
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void MemoryManager::deallocateMemory() 
{
    delete[] array;
    array = nullptr;
    size = 0;
}

void MemoryManager::writeToMemory(int index, int value) 
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