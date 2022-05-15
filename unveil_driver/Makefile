obj-m := unveil.o
unveil-objs := procfs_hook.o unveil_functions.o init.o
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR)  M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean