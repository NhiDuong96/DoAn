#include <stdio.h>
#include "bit.h"
typedef long long int ld;
struct MAP{
  ld walls;
  ld boxes;
  ld marks;
	uint8 x,y;
};
struct PERSON{
  uint8 x,y;
};
MAP maps[] = {
	{0,0,0,3,1},
	{0,0,0,2,2},
	{0,0,0,3,4}
};
int main(){
	MAP a = maps[0];
	set(a.walls,10,10,16);
	printf("%d\n",get(a.walls,10,10,16));
	return 0;
}
