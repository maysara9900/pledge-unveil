# To fix list
* Unveil driver does not support multiprocess. 
* Child processes do not inherent the parent's pledge and unveil calls.
* Vunlrable to Time-of-check to time-of-use attack.
* Unveil driver only works with absoulute paths. It uses a very simple string comparasion to check for allowed paths and it can be fooled in many ways. For example, `../../`,`/d?v/` or using a network address.
* Unveil driver hooks only `openat` system call to check if opened files are allowed. Other system calls need to be hooked like `open`.
* A lot of functions lack error checking.
* Incomplete mapping from pledge promises to Linux system calls.
* Pledge second parameter  `char *execpromises` is not implemented
* Unveil second parameter `char *permissions` is not implemented.

unveil_driver/ftrace_helper.h is a modified copy form https://github.com/shubham0d/Immutable-file-linux
