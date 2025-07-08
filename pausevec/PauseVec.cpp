#include "PauseVec.h"
#include <stdexcept>
#include <iostream>

PauseVec::PauseVec() {
    count_ = 0;
    capacity_ = 1; 
    min_removed = capacity_;
    data = new int[capacity_];
    is_removed = new bool[capacity_];
    for (size_t i = 0; i < capacity_; i++) {
        is_removed[i] = false;
    }
    std::cerr << "PV Ctor: cap=" << capacity_ << ", count=" << count_ << ", min_rem=" << min_removed << std::endl;
}

PauseVec::~PauseVec() {
    std::cerr << "PV Dtor: Final: cap=" << capacity_ << ", count=" << count_ << std::endl;
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
    std::cerr << "Append(" << value << ") BEFORE: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
    if (min_removed < capacity_) {
        compact();
    }
    
    if (count_ == capacity_) {
        resize(capacity_ * 2);
    }
    
    data[count_] = value;
    is_removed[count_] = false;
    count_++;
    std::cerr << "Append(" << value << ") AFTER: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
}

int PauseVec::remove(size_t index) {
    std::cerr << "Remove(" << index << ") BEFORE: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
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
    
    std::cerr << "Remove(" << index << ") AFTER: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
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
    std::cerr << "RemoveVal(" << x << ") BEFORE: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
    for (size_t i = 0; i < capacity_; i++) {
        if (!is_removed[i] && data[i] == x) {
            is_removed[i] = true;
            count_--;
            
            if (min_removed == capacity_ || i < min_removed) {
                min_removed = i;
            }
            
            checkAndShrink(); 
            std::cerr << "RemoveVal(" << x << ") AFTER: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
            return; 
        }
    }
    std::cerr << "RemoveVal(" << x << ") - Value not found." << std::endl;
}

void PauseVec::compact() {
    std::cerr << "Compact() BEFORE: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
    if (min_removed >= capacity_) {
        std::cerr << "Compact() - No holes, returning." << std::endl;
        return;
    }
    
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
    std::cerr << "Compact() AFTER: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
}

void PauseVec::checkAndShrink() {
    std::cerr << "CheckAndShrink() START: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
    while (capacity_ > 1 && count_ <= capacity_ / 4) {
        std::cerr << "  CheckAndShrink() LOOP: C=" << count_ << ", Cap=" << capacity_ << ", Threshold=" << (capacity_ / 4) << std::endl;
        if (min_removed < capacity_) {
            compact();
        }
        size_t new_capacity = capacity_ / 2;
        if (new_capacity < 1) new_capacity = 1; 
        resize(new_capacity);
        std::cerr << "  CheckAndShrink() After resize: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
    }
    std::cerr << "CheckAndShrink() END: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
}

void PauseVec::resize(size_t new_capacity) {
    std::cerr << "Resize(" << new_capacity << ") BEFORE: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
    compact();
    
    int* new_data = new int[new_capacity];
    bool* new_is_removed = new bool[new_capacity];
    
    size_t copyCount = (count_ < new_capacity) ? count_ : new_capacity;
    for (size_t i = 0; i < copyCount; i++) {
        new_data[i] = data[i];
        new_is_removed[i] = false;
    }
    
    for (size_t i = copyCount; i < new_capacity; i++) {
        new_is_removed[i] = false;
    }
    
    delete[] data;
    delete[] is_removed;
    data = new_data;
    is_removed = new_is_removed;
    capacity_ = new_capacity;
    min_removed = capacity_;
    std::cerr << "Resize(" << new_capacity << ") AFTER: C=" << count_ << ", Cap=" << capacity_ << ", MinRem=" << min_removed << std::endl;
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
