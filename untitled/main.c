#include <stdio.h>
#include <time.h>
int main() {
    float time = 0;
    clock_t starttime, endtime;
    starttime = clock();

    for(int x = 1; x <=1500; x++){
        for(int y = 1; y <=1500; y++){

            if(x==y){
                break;
            }

            int sum1 = 0;
            int sum2 = 0;

            for(int i = 1; i < x; i++){
                if((x%i)==0){
                    sum1 += i;
                }
            }

            for(int j = 1; j < y; j++){
                if((y%j)==0){
                    sum2 += j;
                }
            }

            if (sum1 < y){
                break;
            }

            if ((sum1 == y) && (sum2 == x)){
                printf("%d의 친화수 %d\n\r",x,y);
                printf("%d의 친화수 %d\n\r",y,x);
            }
        }
    }

    endtime = clock();
    time = (float)(endtime - starttime)/CLOCKS_PER_SEC;
    printf("Runtime : %.3f S \n", time);
    return 0;
}
