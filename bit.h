#ifndef BIT_H
#define BIT_H

typedef unsigned long long int uint64;
typedef unsigned char uint8;
template <typename T>
inline void set(T &A, int x, int y, int c){
  A |= 1 << (y*c+x);
}
template <typename T>
inline int get(T A, int x, int y, int c){
  return A >> (y*c+x) & 1;
}
template <typename T>
inline void del(T &A, int x, int y, int c){
  A &= ~(1 << (y*c + x));
}
template <typename T>
inline void mov(T &A, int x, int y, int dx, int dy, int c){
  del(A,x,y,c);
  set(A,x+dx,y+dy,c);
}

#endif
