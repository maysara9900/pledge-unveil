#pragma once

#include <linux/fs.h>

typedef struct unveil_path {
	unsigned int pid;
	char path_name[PATH_MAX];
} Path;

extern Path unveiled_path;

void save_path(char path_name[], int size, unsigned int pid);
bool check_if_allowed_path(char* abs_path);
void kill_process(struct task_struct *task);
void get_current_working_dir(char** buff);