#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "unveil.h"

const char* DRIVER_PATH="/proc/unveil";

static bool check_if_path_exists_and_is_folder(const char* path){
    struct stat statbuf;
    if (stat(path, &statbuf) != 0){
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

static char* get_abs_path(const char* path){
    char* abs_path = realpath(path,NULL);
    if(abs_path==NULL){
        printf("Can not get absolute path\n");
        exit(1);
    }
    return abs_path;
}

static void send_path_to_driver(const char* path){
    FILE* fp = fopen(DRIVER_PATH,"w");
    if(fp==NULL){
        printf("Driver file does not exist\n");
        fclose(fp);
        exit(1);
    }
    fputs(path,fp);
    fclose(fp);
    printf("allowing path %s\n",path);
}

void unveil(const char* path){
    if(path[0]=='\0'){
        path = " ";
        send_path_to_driver(path);
        return;
    }
    if(!check_if_path_exists_and_is_folder(path)){
        printf("Path is not correct, enter a valid folder path\n");
        exit(1);
    }
    char* abs_path = get_abs_path(path);
    send_path_to_driver(abs_path);
}