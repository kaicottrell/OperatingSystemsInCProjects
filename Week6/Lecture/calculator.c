#include <stdio.h>
#include "basic_math.h"

int main(){
    int x,y;
    printf("Enter a number: \n");
    scanf("%d", &x);
    printf("Enter a number: \n");
    scanf("%d", &y);
    printf("%d + %d = %d\n", x, y, add(x,y));
}