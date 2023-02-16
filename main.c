#include <stdio.h>

int add(int a, int b){

    return a + b;
}
int main(void){

    int a = 10;
    int b = 40;

    printf("Result: %d", add(a, b));

    return 0;
}
