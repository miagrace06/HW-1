#include "PauseVec.h"
#include <stdexcept>

PauseVec::PauseVec() {
    count_ = 0;
    capacity_ = 1; 
    min_removed = capacity_;
    data = new int[capacity_];
    is_removed = new bool[capacity_];
    for (size_t i = 0; i < capacity_; i++) {
        is_removed[i] = false;
    }
}

PauseVec::~PauseVec() {
    delete[] data;
    delete[] is_removed;
}

size_t PauseVec::capacity() const {
    return capacity_;
}

size_t PauseVec::count() const {
    return count_;
}

void PauseVec::append(int value) {
    if (min_removed < capacity_) {
        compact();
    }
    
    if (count_ == capacity_) {
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
    
    size_t actualIndex = findActualIndex(index);
    if (actualIndex == capacity_) { 
        throw std::out_of_range("Index out of range.");
    }
    
    int removedValue = data[actualIndex];
    is_removed[actualIndex] = true;
    count_--;
    
    if (min_removed == capacity_ || actualIndex < min_removed) {
        min_removed = actualIndex;
    }
    
    checkAndShrink();
    
    return removedValue;
}

int PauseVec::lookup(size_t index) {
    if (index >= count_) {
        throw std::out_of_range("Index out of range.");
    }
    
    size_t actualIndex = findActualIndex(index);
    if (actualIndex == capacity_) {
        throw std::out_of_range("Index out of range.");
    }
    
    return data[actualIndex];
}

void PauseVec::remove_val(int x) {
    for (size_t i = 0; i < capacity_; i++) {
        if (!is_removed[i] && data[i] == x) {
            is_removed[i] = true;
            count_--;
            
            if (min_removed == capacity_ || i < min_removed) {
                min_removed = i;
            }
            
            checkAndShrink(); 
            return; 
        }
    }
}

void PauseVec::compact() {
    if (min_removed >= capacity_) return;
    
    size_t writeIndex = 0;
    for (size_t readIndex = 0; readIndex < capacity_; readIndex++) {
        if (!is_removed[readIndex]) {
            if (writeIndex != readIndex) {
                data[writeIndex] = data[readIndex];
            }
            is_removed[writeIndex] = false;
            writeIndex++;
        }
    }
    
    for (size_t i = writeIndex; i < capacity_; i++) {
        is_removed[i] = false;
    }
    
    count_ = writeIndex;
    min_removed = capacity_;
}

void PauseVec::checkAndShrink() {
    while (capacity_ > 1 && count_ <= capacity_ / 4) {
        size_t new_capacity = capacity_ / 2;
        if (new_capacity < 1) new_capacity = 1;
        resize(new_capacity);
    }
}


void PauseVec::resize(size_t new_capacity) {
    int* new_data = new int[new_capacity];
    bool* new_is_removed = new bool[new_capacity];

    size_t writeIndex = 0;
    for (size_t i = 0; i < capacity_; ++i) {
        if (!is_removed[i]) {
            if (writeIndex < new_capacity) {
                new_data[writeIndex] = data[i];
                new_is_removed[writeIndex] = false;
                writeIndex++;
            }
        }
    }

    for (size_t i = writeIndex; i < new_capacity; i++) {
        new_is_removed[i] = false;
    }

    delete[] data;
    delete[] is_removed;
    data = new_data;
    is_removed = new_is_removed;
    capacity_ = new_capacity;
    count_ = writeIndex;      
    min_removed = capacity_;    
}

size_t PauseVec::findActualIndex(size_t logicalIndex) {
    size_t currentLogical = 0;
    for (size_t i = 0; i < capacity_; i++) {
        if (!is_removed[i]) {
            if (currentLogical == logicalIndex) {
                return i;
            }
            currentLogical++;
        }
    }
    return capacity_; 
}

PauseVec* create_pausevec() {
    return new PauseVec();
}
