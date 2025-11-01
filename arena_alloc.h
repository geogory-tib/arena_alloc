#ifndef ARENA_H
#define ARENA_H
#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>
#define ARENA_FULL 0 
typedef struct arena_t {
  void *buffer;
  int brk;
  int cap;
} arena;
static inline arena arena_new(int cap) {
  arena ret;
  ret.cap = cap;
  ret.buffer = calloc(cap, 1);
  return ret;
}
static inline void *arena_alloc(arena *ar, int size) {
  if ((ar->brk + size) >= ar->cap) {
    return NULL;
  }
  void *ret = (((char *)ar->buffer) + ar->brk);
  ar->brk += (size);
  return ret;
}
static inline int arena_used(arena *ar) {
  if (ar->brk >= ar->cap) {
    return ARENA_FULL;
  }
  return ar->brk;
}
static inline void arena_free(arena *ar){
  free(ar->buffer);
  ar->buffer = NULL;
  ar->cap = 0;
  ar->brk =0;
  
}
#endif // !
