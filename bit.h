#include "variables.h"

#ifndef BIT_H
#define BIT_H

inline void set(uint8 *A, uint8 x, uint8 y, uint8 c){
  uint8 pos = y*c+x;
  *(A+pos/8) |= 1 << pos%8;
}

inline uint8 get(const uint8 *A, uint8 x, uint8 y, uint8 c){
  uint8 pos = y*c+x;
  return *(A+pos/8) >> (pos%8) & 1;
}

inline uint8 get16(const uint8 *A, uint8 x, uint8 y){
  uint8 c = y*16+x;
  uint8 k = 0;
  while(k < 8){
    if(c == *(A+k))  return k+1;
    k++;
  }
  return 0;
}

inline void mov16(uint8 *A, uint8 x, uint8 y, int8 dx, int8 dy){
  uint8 k = get16(A,x,y) - 1;
  *(A+k) = (y+dy)*16+(x+dx);
}
#endif
