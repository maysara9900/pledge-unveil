#include <linux/uaccess.h>
#include "unveil_functions.h"

// if a process writes to procfs of this driver then record the pid of the process and the path it sent
static ssize_t proc_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos){
	struct task_struct *task = current;
	char path_name[NAME_MAX];
	int error = strncpy_from_user(path_name, buff, count+1);
	if(error>0){
		save_path(path_name, count+1, task->pid);
	}
	return count;
}

struct file_operations proc_ops = {
	.write = proc_write
};