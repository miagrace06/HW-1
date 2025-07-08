#ifndef PAUSEVEC_H
#define PAUSEVEC_H
#include <cstddef>
class PauseVec {
public:
    PauseVec();
    ~PauseVec();
    size_t capacity() const;
    size_t count() const;
    void append(int value);
    int remove(size_t index);
    int lookup(size_t index);
    void remove_val(int x);  
private:
    int* data;
    bool* is_removed;
    size_t capacity_;
    size_t count_;
    size_t min_removed;
    
    void compact();
    void resize(size_t new_capacity);
    size_t findActualIndex(size_t logicalIndex);
    void checkAndShrink();  
};
PauseVec* create_pausevec(); 
#endif 
