#include "ftrace_helper.h"
#include "procfs_hook.h"
#include "unveil_functions.h"

// real openat syscall
static asmlinkage long (*real_sys_openat)(struct pt_regs *regs);

static int kill_process_and_return(char* path, struct task_struct* task){
	printk(KERN_INFO "not allowed file %s for process %d", path, task->pid);
	kill_process(task);
	return -1;
}

// openat syscall hook
static asmlinkage long fh_sys_openat(struct pt_regs *regs){
	struct task_struct *task;
    char __user *pathname = (char *)regs->si;
    char opened_file_path[NAME_MAX] = {0};
    long error;
    bool is_allowed_path;
    char* absolute_path;
	task = current;
	// if current process is not the process that called unveil then return
	if(unveiled_path.pid!=task->pid){
		return real_sys_openat(regs);
	}
    error = strncpy_from_user(opened_file_path, pathname, NAME_MAX);
    if(error==0){
    	return real_sys_openat(regs);
    }
    // if empty pledge then don't allow any openat syscall
    if(opened_file_path[0]==' ' && opened_file_path[1]=='\0'){
		return kill_process_and_return(opened_file_path,task);
    }
    // if opened file has relative path then use process working dir to check if it is allowed
	if(opened_file_path[0]!='/'){
        get_current_working_dir(&absolute_path);
        strcpy(opened_file_path, absolute_path);
    }
	is_allowed_path = check_if_allowed_path(opened_file_path);
	if(!is_allowed_path){
		return kill_process_and_return(opened_file_path,task);
	}
	return real_sys_openat(regs);
}

// hook openat syscall
static struct ftrace_hook demo_hooks[] = {
	HOOK("sys_openat", fh_sys_openat, &real_sys_openat),
};

static int fh_init(void){
	int err;
	err = fh_install_hooks(demo_hooks, ARRAY_SIZE(demo_hooks));
	if (err){
		return err;
	}
	if (!proc_create(MODULE_NAME, 0666, NULL, &proc_ops)){
		return -ENOMEM;
	}
	pr_info("module loaded\n");
	return 0;
}

static void fh_exit(void){
	fh_remove_hooks(demo_hooks, ARRAY_SIZE(demo_hooks));\
	remove_proc_entry(MODULE_NAME, NULL);
	pr_info("module unloaded\n");
}

module_init(fh_init);
module_exit(fh_exit);

MODULE_AUTHOR("maysara9900");
MODULE_LICENSE("GPL");