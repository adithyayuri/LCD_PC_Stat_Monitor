#include "buffer.h"

cbuff_t * cbuff_new(int size)
{
  cbuff_t * cb = (cbuff_t*)malloc(sizeof(cbuff_t));
  memset(cb, 0, sizeof(cbuff_t));
  cb->size = size;
    cb->buff = (int*)malloc(sizeof(int)*size);

  return cb;
}

void cbuff_add(cbuff_t * cb, int elem)
{
  int end = cb->end;
  if(cb->count && (end % cb->size) == cb->start){
    cb->start = (cb->start + 1 ) %cb->size;
    cb->count --;
  }
  cb->buff[cb->end] = elem;
  cb->end = (cb->end+1) % cb->size;
  cb->count ++;
  }


int cbuff_remove(cbuff_t * cb)
{
  int start = cb->start ;
  int ret = -1;
  if(cb->count <= 0) {
    return ret;
  }
  if(cb->count || (start % cb->size) != cb->end) {
    ret = cb->buff[cb->start];
    cb->start = (cb->start + 1 ) % cb->size;
    cb->count--;
  } else {
  }
  return ret;
}

void cbuff_print(cbuff_t * cb)
{
  int start = cb->start ;
  int end = cb->end ;
  int i, count = 0;
  for(i = start; count < cb->count; i = (i + 1)%cb->size){
    count++;
    if(i == (end - 1)) {
      break;
    }
  }
}

void cbuff_delete(cbuff_t * cb)
{
  free(cb->buff);
  free(cb);
}
