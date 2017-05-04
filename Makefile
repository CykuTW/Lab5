obj-m := led163.o led164.o led165.o led166.o
KERNEL_DIR := /usr/src/linux-headers-$(shell uname -r)
PREFIX := 
EXPORT := /sys/class/gpio/export
UNEXPORT := /sys/class/gpio/unexport

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
	sudo echo 163 > $(EXPORT)
	sudo echo 164 > $(EXPORT)
	sudo echo 165 > $(EXPORT)
	sudo echo 166 > $(EXPORT)
uninstall:
	sudo rmmod led163
	sudo rmmod led164
	sudo rmmod led165
	sudo rmmod led166
	sudo rm /dev/led163
	sudo rm /dev/led164
	sudo rm /dev/led165
	sudo rm /dev/led166
	sudo echo 163 > $(UNEXPORT)
	sudo echo 164 > $(UNEXPORT)
	sudo echo 165 > $(UNEXPORT)
	sudo echo 166 > $(UNEXPORT)
.PHONY:clean install uninstall
