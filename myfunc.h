#define fi 10e-6
#define pi 3.141592654
unsigned long G(int n){
        unsigned long result = 1;
        for(int i = 1; i<=n ; i++)
                result*=i;;
        return result;
}
float B(float x, int n){
        float result = 1;
        for(int i = 0; i < n; i++)
                result*=x;
        return result;
}
float sin(float x){
        int d = (x > 0)?1:-1;
        x = d*x;
        int a = 1;
        int n = 0;
        float sum = 0;
        float cur = x;
        while(1){
                cur = B(x,2*n+1)/G(2*n+1);
                if(cur < fi) break;             
                sum+=a*cur;
                a=-a;
                n++;
        }
        return d*sum;
}
void rot(const uint8_t *bitmap, int nbyte, int r, float al, uint8_t *buf){
        float sinx = sin(al);
        float cosx = sin(pi/2-al);
        for(int j = 0; j < r*nbyte; j++)
                for(int i = 0; i < 8; i++){
                        if((bitmap[j] >> (7-i)) & 1 == 1){
                                int x = (j*8+i)%r;
                                int y = (j*8+i)/r;
                                int xp = x*cosx - y*sinx + r/2.0*(sinx-cosx+1);
                                int yp = x*sinx + y*cosx - r/2.0*(sinx+cosx-1);
                                int p = yp*r+xp;
                                buf[p/8] |= (1 << (7 - p%8));
                        }
                }       
}
