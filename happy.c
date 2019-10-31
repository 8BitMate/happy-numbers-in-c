#include<stdio.h>
#include<math.h>
void main(){
    int n,i,sum=0,rem;
    scanf("%d",&n);
    while(sum!=1 && sum!=4){
        sum=0;
        while(n>0){
            rem=n%10;
            sum+=pow(rem,2);
            n=n/10;
        }
        n=sum;

    }
    if(sum==1){
        printf("hap");
    }
    else
        printf("not happy");
}
