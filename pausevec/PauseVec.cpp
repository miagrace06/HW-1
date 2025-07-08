#include "PauseVec.h"
#include <iostream>
#include <stdexcept>
using namespace std;

PauseVec::PauseVec() {
    count_ = 0;
    capacity_ = 8;
    min_removed = capacity_;
    
    data = new int[capacity_];
    is_removed = new bool[capacity_];
    for (size_t i = 0; i < capacity_; i++){
        is_removed[i] = false;
    }
}

PauseVec::~PauseVec(){
    delete[] data;
    delete[] is_removed;
}

size_t PauseVec::capacity() const {
    return capacity_;
}

size_t PauseVec::count() const {
    return count_;
}

void PauseVec::append(int value){
    if (count_ == capacity_){
        int* new_data = new int[capacity_ * 2];
        bool* new_is_removed = new bool[capacity_ * 2];
        for (size_t i = 0; i < count_; i++){
            new_data[i] = data[i];
            new_is_removed[i] = is_removed[i];
        }
        delete[] data;
        delete[] is_removed;
        
        data = new_data;
        is_removed = new_is_removed;
        capacity_ = capacity_ * 2;
        
    }
    data[count_] = value;
    is_removed[count_] = false; 
    count_++;
}

int PauseVec::remove(size_t index) {
    if (index >= count_) {
        throw std::out_of_range("Index out of range.");
    }
    
    if (index > min_removed) {
        size_t writeIndex = min_removed;
        for (size_t i = min_removed; i < count_; i++) {
            if (!is_removed[i]) {
                data[writeIndex] = data[i];
                is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
        count_ = writeIndex;
        min_removed = capacity_;
    }
    
    int removedValue = data[index];
    is_removed[index] = true;
    
    if (min_removed == capacity_ || index < min_removed) {
        min_removed = index;
    }
    
    if (index == min_removed) {
        size_t writeIndex = min_removed;
        for (size_t i = min_removed; i < count_; i++) {
            if (!is_removed[i]) {
                data[writeIndex] = data[i];
                is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
        count_ = writeIndex;
        min_removed = capacity_;
    }
    
    if (capacity_ > 8 && count_ <= capacity_ / 4) {
        size_t new_capacity = capacity_ / 2;
        int* new_data = new int[new_capacity];
        bool* new_is_removed = new bool[new_capacity];
        for (size_t i = 0; i < count_; i++) {
            new_data[i] = data[i];
            new_is_removed[i] = false;  
        }
        delete[] data;
        delete[] is_removed;
        data = new_data;
        is_removed = new_is_removed;
        capacity_ = new_capacity;
    }
    
    return removedValue;
}

int PauseVec::lookup(size_t index) {
    if (index >= count_) {
        throw std::out_of_range("Index out of range.");
    }
    
    if (index > min_removed) {
        size_t writeIndex = min_removed;
        for (size_t i = min_removed; i < count_; i++) {
            if (!is_removed[i]) {
                data[writeIndex] = data[i];
                is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
        count_ = writeIndex;
        min_removed = capacity_;
    }
    
    if (is_removed[index]) {
        throw std::out_of_range("Index out of range.");
    }
    return data[index];
}

