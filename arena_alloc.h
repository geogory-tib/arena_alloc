#ifndef ARENA_H
#define ARENA_H
#include <stddef.h>
#include <stdlib.h>
#define ARENA_FULL -1

typedef struct arena_t {
  void *buffer;
  int brk;
  int cap;
} arena;
static const int G_ALLOC_DEFAULT_AR_SIZE = (sizeof(arena)) * 10;
static inline arena arena_new(int cap) {
  arena ret;
  ret.brk = 0;
  ret.cap = cap;
  ret.buffer = calloc(cap, 1);
  return ret;
}
static inline void *arena_alloc(arena *ar, int size) {
  if ((ar->brk + size - 1) >= ar->cap) {
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
static inline void arena_free(arena *ar) {
  free(ar->buffer);
  ar->buffer = NULL;
  ar->cap = 0;
  ar->brk = 0;
}
/* the following function can invaildate all of your current refrences if you
 * call this. this is mainly used as an internal thing for the garena. there is
 * only one pointer I have to keep track of and it's basicly just the buffer for
 * the pages so im  just using this as a dynamic array of sorts to handle page
 * management if you use this please be mindful of this as it can segfault your
 * program
 */
void arena_realloc(arena *ar, int size) {
  ar->buffer = realloc(ar->buffer, size);
  ar->cap = size;
}
typedef struct garena_t {
  arena *pages;
  int page_size;
  int current_page;
  int page_count;
  arena alloc_ar; // use arena for ease of use
} garena;

static inline garena garena_new(int page_size) {
  garena ret;
  ret.alloc_ar = arena_new(G_ALLOC_DEFAULT_AR_SIZE + 1);
  ret.page_size = page_size;
  ret.current_page = 0;
  ret.page_count = 10;
  ret.pages = arena_alloc(&ret.alloc_ar, (sizeof(arena) * 10));
  ret.pages[ret.current_page] = arena_new(ret.page_size);
  return ret;
}
static inline arena *garena_page_new(garena *ar) {
  if (ar->current_page + 1 == ar->page_count) {
    arena_realloc(&ar->alloc_ar,
                  ar->alloc_ar.cap + G_ALLOC_DEFAULT_AR_SIZE + 1);
    ar->pages = ar->alloc_ar.buffer;
    ar->pages += 10;
  }
  ar->pages[ar->current_page + 1] = arena_new(ar->page_size);
  ar->current_page++;
  arena *current_page = &ar->pages[ar->current_page];
  return current_page;
}
static inline void *garena_alloc(garena *ar, int size) {
  arena *current_page = &ar->pages[ar->current_page];
  if ((current_page->brk + size) >= current_page->cap) {
    current_page = garena_page_new(ar);
  }
  void *ret = arena_alloc(current_page, size);
  return ret;
}
static inline int garena_used(garena *ar) {
  arena *current_page = &ar->pages[ar->current_page];
  if (current_page->brk >= current_page->cap) {
    return ARENA_FULL;
  }
  return current_page->brk;
}
static inline void garena_destroy(garena *gar) {
  for (int i = gar->current_page; i >= 0; i--) {
    arena *current_page = &gar->pages[i];
    arena_free(current_page);
  }
  gar->page_size = 0;
  gar->current_page = 0;
  gar->page_count = 0;
  arena_free(&gar->alloc_ar);
}

#endif // !
