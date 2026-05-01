#include <stdio.h>

int main() {
    for(int i = 0; i < 5; i++) {
        int x = i;
        printf("The address of x in iteration %d is %p\n", i, (void*)&x);
    }
    return 0;
}
