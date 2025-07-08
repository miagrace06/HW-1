#include "PauseVec.h"
#include <iostream>
#include <stdexcept>
using namespace std;

PauseVec::PauseVec() {
    count = 0;
    capacity = 8;
    min_removed = capacity;
    
    data = new int[capacity];
    is_removed = new bool[capacity];
    for (size_t i = 0; i < capacity; i++){
        is_removed[i] = false;
    }
}

PauseVec::~PauseVec(){
    delete[] data;
    delete[] is_removed;
}

size_t PauseVec::capacity() const {
    return capacity;
}

size_t PauseVec::count() const {
    return count;
}

void PauseVec::append(int value){
    if (count == capacity){
        int* new_data = new int[capacity * 2];
        int* new_is_removed = new bool[capacity * 2];
        for (size_t i = 0; i < count; i++){
            new_data[i] = data[i];
            new_is_removed[i] = is_removed[i];
        }
        delete[] data;
        delete[] is_removed;
        
        data = new_data;
        is_removed = new_is_removed;
        capacity = capacity * 2;
        
    }
    data[count] = value;
    is_removed[count] = false; 
    count++;
}

int PauseVec::remove(size_t index) {
    if (index >= count) {
        throw std::out_of_range("Index out of range.");
    }
    
    if (index > min_removed) {
        size_t writeIndex = min_removed;
        for (size_t i = min_removed; i < count; i++) {
            if (!is_removed[i]) {
                data[writeIndex] = data[i];
                is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
        count = writeIndex;
        min_removed = capacity;
    }
    
    int removedValue = data[index];
    is_removed[index] = true;
    
    if (min_removed == capacity || index < min_removed) {
        min_removed = index;
    }
    
    if (index == min_removed) {
        size_t writeIndex = min_removed;
        for (size_t i = min_removed; i < count; i++) {
            if (!is_removed[i]) {
                data[writeIndex] = data[i];
                is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
        count = writeIndex;
        min_removed = capacity;
    }
    
    if (capacity > 8 && count <= capacity / 4) {
        size_t new_capacity = capacity / 2;
        int* new_data = new int[new_capacity];
        bool* new_is_removed = new bool[new_capacity];
        for (size_t i = 0; i < count; i++) {
            new_data[i] = data[i];
            new_is_removed[i] = false;  
        }
        delete[] data;
        delete[] is_removed;
        data = new_data;
        is_removed = new_is_removed;
        capacity = new_capacity;
    }
    
    return removedValue;
}

int PauseVec::lookup(size_t index) {
    if (index >= count) {
        throw std::out_of_range("Index out of range.");
    }
    
    if (index > min_removed) {
        size_t writeIndex = min_removed;
        for (size_t i = min_removed; i < count; i++) {
            if (!is_removed[i]) {
                data[writeIndex] = data[i];
                is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
        count = writeIndex;
        min_removed = capacity;
    }
    
    if (is_removed[index]) {
        throw std::out_of_range("Index out of range.");
    }
    return data[index];
}

