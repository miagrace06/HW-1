 #ifndef PAUSEVEC_H
#define PAUSEVEC_H

#include <cstddef>

class PauseVec {
private:
int* data;
bool* is_removed;
size_t count_;
size_t capacity_;
size_t min_removed;

public:
  PauseVec();
  ~PauseVec();

  size_t capacity() const;
  size_t count() const;

  int    lookup(size_t index);
  int    remove(size_t index);
  void	 remove_val(int x);
  void   append(int value);
};

PauseVec* create_pausevec();

#endif
