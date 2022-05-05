#include <sys/stat.h>
#include <stdio.h>
#include "pledge.h"

int main(){
    pledge("stdio");
    // makdir should not work unless we are using cpath promise
    // mkdir("hello",0777);
    printf("%s\n","code execution ended successfully :D");
    return 0;
}
