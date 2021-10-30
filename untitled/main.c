#include <stdio.h>

int main() {
    char DF11 = 0x03;
    printf("%d\n",DF11);
    printf("%d\n",DF11*(256));
    printf("%d\n",DF11 << 8);

    printf("%d\n",DF11*(256*256));
    printf("%d\n",DF11 << 16);

    printf("%d\n",DF11*(256*256*256));
    printf("%d\n",DF11 << 24);

    return 0;
}
