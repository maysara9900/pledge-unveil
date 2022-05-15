#include <stdio.h>
#include "pledge.h"
#include "unveil.h"

void test_unveil(){
    while(1){
        printf("enter file name to create\n");
        char buf[100];
        scanf("%s",buf);
        FILE* fp = fopen(buf,"a");
        fclose(fp);
        printf("%s has been created\n",buf);
    }
}

int main(){
    unveil("/dev/shm");
    pledge("stdio rpath");
    test_unveil();
    // makdir should not work unless we are using cpath promise
    // mkdir("hello",0777);
    printf("%s\n","code execution ended successfully :D");
    return 0;
}
