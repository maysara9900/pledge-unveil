#include <stdio.h>     
#include <seccomp.h> 
#include <string.h>
#include <stdlib.h>

const int MAX_PROMISE_STRING_LENGTH = 1000;

// array holds all pledge promises string values
// each promise corresponds to an array of systemcalls in promises_to_syscalls_array
char* promises_array[] = {
    "stdio",
    "rpath",
    "cpath",
    NULL
};

// array holds all system calls that maps to promises values.
char* promises_to_syscalls_array[][10] = {
    {"dup","dup2","write",NULL},
    {"chdir","lstat",NULL},
    {"mkdir","rmdir",NULL}
};

// return the index of a promise in the promises_array
int get_promise_index(char* promise){
    int i = 0;
    while(promises_array[i]!=NULL){
        if(strcmp(promise,promises_array[i])==0){
            return i;
        }
        i++;
    }
    return -1;
}

// for each system call in systemcalls array, set a bpf filter to allow it.
void set_seccomp_for_syscalls(char** syscalls, scmp_filter_ctx ctx){
    int i =0;
    while(syscalls[i]!=NULL){
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, seccomp_syscall_resolve_name(syscalls[i]), 0);
        i++;
    }
}

// initialize the seccomp filters and default action.
scmp_filter_ctx init_seccomp(){
    scmp_filter_ctx ctx;
    // blacklisting everything by default
    ctx = seccomp_init(SCMP_ACT_KILL);
    return ctx;
}

// pledge takes a string that holds multiple promises separated by white space.
void pledge(const char* promises){
    printf("%s\n", "Pledging");
    // initialize seccomp
    scmp_filter_ctx ctx = init_seccomp();
    // if string is not null terminated then exit
    int promise_string_size = strnlen(promises, MAX_PROMISE_STRING_LENGTH);
    if(promise_string_size==MAX_PROMISE_STRING_LENGTH){
        exit(1);        
    } 
    // convert the string to an array of charachters
    char array_of_chars[promise_string_size];
    strcpy(array_of_chars, promises);
    // split the string on white spaces
    char* token = strtok(array_of_chars, " ");
    // for each token (promise) in the string
    while(token!=NULL){
        // get the promise index
        int index = get_promise_index(token);
        if(index!=-1){
            // get the corresponding systemcalls to this promise
            char** syscalls = promises_to_syscalls_array[index];
            // set filters for these systemcalls
            set_seccomp_for_syscalls(syscalls, ctx);
        }
        // get next token in the string
        token = strtok (NULL, " ");
    }
    // allow the program to use exit_group systemcall to exit without problems
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, seccomp_syscall_resolve_name("exit_group"), 0);
    // load the bpf filters
    seccomp_load(ctx);  
}