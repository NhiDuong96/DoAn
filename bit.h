#ifndef BIT_H
#define BIT_H

typedef unsigned char uint8;
typedef char int8;
typedef unsigned long int uint32;
typedef unsigned long long int uint64; 
typedef unsigned long uint16;

inline void set(uint8 *A, uint8 x, uint8 y, uint8 c){
  uint8 pos = y*c+x;
  *(A+pos/8) |= 1 << pos%8;
}

inline uint8 get(const uint8 *A, uint8 x, uint8 y, uint8 c){
  uint8 pos = y*c+x;
  return *(A+pos/8) >> (pos%8) & 1;
}

inline uint8 get16(uint64 A, uint8 x, uint8 y){
  uint8 i,j;
  uint8 k = 0;
  while(A != 0){
    i = A & 0x0F; A = A >> 4;
    j = A & 0x0F; A = A >> 4;
    k++;
    if(i == x && j == y)  return k;
  }
  return 0;
}

inline void del(uint8 *A, uint8 x, uint8 y, uint8 c){
  uint8 pos = y*c+x;
  *(A+pos/8) &= ~(1 << (pos%8));
}

inline void mov(uint8 *A, uint8 x, uint8 y, uint8 dx, uint8 dy, uint8 c){
  del(A,x,y,c);
  set(A,x+dx,y+dy,c);
}

inline void mov16(uint64 &A, uint8 x, uint8 y, int8 dx, int8 dy){
  uint8 k = get16(A,x,y) - 1;
  uint64 a = (y+dy)*16+(x+dx);
  uint64 b = 0xff;
  A &= ~(b << (k*8));
  A |= (a <<(k*8));
}
#endif
