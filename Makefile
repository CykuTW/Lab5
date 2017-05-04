obj-m := led163.o led164.o led165.o led166.o
KERNEL_DIR := /usr/lib/modules/$(shell uname -r)/build
PREFIX := 

PWD := $(shell pwd)
all:
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD)
clean:
	rm *.o *.ko *.mod.c
install:
	sudo insmod $(PWD)/led163.ko
	sudo insmod $(PWD)/led164.ko
	sudo insmod $(PWD)/led165.ko
	sudo insmod $(PWD)/led166.ko
	sudo mknod /dev/led163 c 60 0
	sudo mknod /dev/led164 c 61 0
	sudo mknod /dev/led165 c 62 0
	sudo mknod /dev/led166 c 63 0
	sudo chmod 666 /dev/led163
	sudo chmod 666 /dev/led164
	sudo chmod 666 /dev/led165
	sudo chmod 666 /dev/led166
uninstall:
	sudo rmmod led163
	sudo rmmod led164
	sudo rmmod led165
	sudo rmmod led166
	sudo rm /dev/led163
	sudo rm /dev/led164
	sudo rm /dev/led165
	sudo rm /dev/led166
.PHONY:clean install uninstall
