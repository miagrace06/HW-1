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
        resize(capacity_ * 2);
    }
    
    data[count_] = value;
    is_removed[count_] = false; 
    count_++;
}

int PauseVec::remove(size_t index) {
    if (index >= count_) {
        throw std::out_of_range("Index out of range.");
    }
    if (is_removed[index]) {
        throw std::out_of_range("Index out of range.");
    }
    
    int removedValue = data[index];
    is_removed[index] = true;
    
    if (min_removed == capacity_ || index < min_removed) {
        min_removed = index;
    }
    
    size_t active_count = 0;
    for (size_t i = 0; i < count_; i++) {
        if (!is_removed[i]) {
            active_count++;
        }
    }
    
    if (capacity_ > 8 && active_count <= capacity_ / 4) {
        compact();
        size_t new_capacity = capacity_ / 2;
        if (new_capacity < 8) new_capacity = 8;
        resize(new_capacity);
    }
    
    return removedValue;
}

int PauseVec::lookup(size_t index) {
    if (min_removed < capacity_ && index > min_removed) {
        compact();
    }
    
    if (index >= count_) {
        throw std::out_of_range("Index out of range.");
    }
    if (is_removed[index]) {
        throw std::out_of_range("Index out of range.");
    }
    return data[index];
}

void PauseVec::remove_val(int x) {
    for (size_t i = 0; i < count_; i++) {
        if (!is_removed[i] && data[i] == x) {
            remove(i);
            return;
        }
    }
}

void PauseVec::compact() {
    if (min_removed >= capacity_) return;
    
    size_t writeIndex = min_removed;
    
    for (size_t readIndex = min_removed; readIndex < count_; readIndex++) {
        if (!is_removed[readIndex]) {
            if (writeIndex != readIndex) {
                data[writeIndex] = data[readIndex];
            }
            is_removed[writeIndex] = false;
            writeIndex++;
        }
    }
    
    count_ = writeIndex;
    min_removed = capacity_;
}

void PauseVec::resize(size_t new_capacity) {
    int* new_data = new int[new_capacity];
    bool* new_is_removed = new bool[new_capacity];
    
    for (size_t i = 0; i < count_; i++) {
        new_data[i] = data[i];
        new_is_removed[i] = is_removed[i];
    }
    
    for (size_t i = count_; i < new_capacity; i++) {
        new_is_removed[i] = false;
    }
    
    delete[] data;
    delete[] is_removed;
    
    data = new_data;
    is_removed = new_is_removed;
    capacity_ = new_capacity;
    min_removed = capacity_;
}
