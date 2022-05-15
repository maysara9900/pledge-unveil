#include <linux/sched/signal.h>
#include <linux/fs_struct.h>
#include "unveil_functions.h"

Path unveiled_path = {
	0,
	""
};

// save path and pid of process that wrote to procfs of the driver
void save_path(char path_name[], int size, unsigned int pid){
	unveiled_path.pid = pid;
	strncpy(unveiled_path.path_name, path_name, size);
	printk("allowed path is %s for process %d \n", path_name, pid);
}

bool check_if_allowed_path(char* abs_path){
	int opened_path_size;
	int unveiled_path_size;
	opened_path_size = strnlen(abs_path, PATH_MAX);
	unveiled_path_size = strnlen(unveiled_path.path_name, PATH_MAX);
	if(opened_path_size<unveiled_path_size){
		return false;
	}
	return strncmp(abs_path, unveiled_path.path_name, unveiled_path_size)==0;
}

void kill_process(struct task_struct *task){
	int signum = SIGKILL;
	int ret;
	struct kernel_siginfo info;
	memset(&info, 0, sizeof(struct kernel_siginfo));
	info.si_signo = signum;
	ret = send_sig_info(signum, &info, task);
	if (ret<0){
		printk(KERN_INFO "error sending signal\n");
	}else{
		printk(KERN_INFO "Target has been killed\n");
	}
}

void get_current_working_dir(char** buf){
    struct path pwd;
    char x[1000];
    get_fs_pwd(current->fs, &pwd);
    *buf = dentry_path_raw(pwd.dentry, x, 999);
}