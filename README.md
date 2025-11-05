# This is a simple single header file arena libary built ontop of malloc!   


The API is simple there is a "garena" and a "arena".   
The arena is a fixed size memory buffer that you can allocate from it's usecase is for when you know roughly how much heap allocated space you need.   
Example use case      
```
#include <stdio.h>
#inlcude <stdlib.h>
#include <string.h>
int main(void){
  arena ar_buf = arena_new(500);
  char *str = arena_alloc(&ar_buf,10);
  strcpy(str,"Hello world!");
  puts(str);
  arena_free(&ar_buff);
  return 0;
}
```
If arena_alloc fails it will return a NULL pointer. You can get the size remaining in the arena by using the arena_used() function if it is full it will return -1.   

The garena is much like the arenea exepect that it is a growable arena that works on a per "page" basis. This is so you do not invaildate any previous pointers to the arena's buffer.   
Currently there is an issue witht he garena as when you reallocate the buffer when the page buffer gets filled it segfaults(I haven't had the time to look at it yet).

```
#include <stdio.h>
#inlcude <stdlib.h>
#include <string.h>
int main(void){
  garena ar_buf = arena_new(500);
  char *str = garena_alloc(&ar_buf,10);
  strcpy(str,"Hello world!");
  puts(str);
  garena_destroy(&ar_buff); 
  return 0;
}
```
