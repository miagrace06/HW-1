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

private:
    int* data;
    bool* is_removed;
    size_t capacity_;
    size_t count_;
    size_t min_removed;

    void compact();
    void resize(size_t new_capacity);

    PauseVec* create_pausevec(); 
};

PauseVec* create_pausevec();

#endif // PAUSEVEC_H
