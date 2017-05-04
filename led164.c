#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <asm/uaccess.h>

#define MAJOR_NUM 61
#define THE_MODULE_NAME "led164"
#define THE_MODULE_AUTHOR "CYH"
#define THE_MODULE_LICENSE "GPL"
#define GPIO_NUMBER "164"
#define GPIO_NUMBER_LEN 3
 
mm_segment_t old_fs;

void switch_to_kernel_ds(void) { 
    old_fs = get_fs(); 
    set_fs(get_ds()); 
} 

void revert_from_kernel_ds(void) {
    set_fs(old_fs);
}

struct file *open_file(char *path, int flag, int mode) { 
    struct file *fp = NULL;
    fp = filp_open(path, flag, mode); 
    if (fp)
        return fp; 
    else
        return NULL;
} 
 
int read_file(struct file *fp, char *buff, int size) { 
    loff_t pos = 0;
    if (fp) 
        return vfs_read(fp, buff, size, &pos); 
    else 
        return -1; 
} 

int write_file(struct file *fp, char *buff, int size) {
    loff_t pos = 0;
    if (fp) {
        return vfs_write(fp, buff, size, &pos);
    } else
        return -1;
}
 
int close_file(struct file *fp) { 
    filp_close(fp, NULL); 
    return 0; 
}

int turn_on_led(void) {
    struct file *fp;

    fp = open_file("/sys/class/gpio/export", O_WRONLY, 0);
    write_file(fp, GPIO_NUMBER, GPIO_NUMBER_LEN);
    close_file(fp);

    fp = open_file("/sys/class/gpio/gpio164/direction", O_WRONLY, 0);
    write_file(fp, "out", 3);
    close_file(fp);

    fp = open_file("/sys/class/gpio/gpio164/value", O_WRONLY, 0);
    write_file(fp, "1", 1);
    close_file(fp);

    return 1;
}

int turn_off_led(void) {
    struct file *fp;

    fp = open_file("/sys/class/gpio/gpio164/value", O_WRONLY, 0);
    write_file(fp, "0", 1);
    close_file(fp);

    fp = open_file("/sys/class/gpio/unexport", O_WRONLY, 0);
    write_file(fp, GPIO_NUMBER, GPIO_NUMBER_LEN);
    close_file(fp);

    return 1;
}

static ssize_t drv_read(struct file *filp, char *buff, size_t count, loff_t *ppos) {
    // read_file(struct file *fp, char *buff, int size)
    /*struct file *fp;
    char value_buff[64];
    switch_to_kernel_space();
    fp = open_file("/sys/class/gpio/gpio166/value" "/tmp/lab5", O_RDONLY, 0);
    read_file(fp, value_buff, 1);
    copy_to_user(buff, value_buff, 1);
    close_file(fp);
    revert_from_kernel_space();
    return 1;*/
    if (*ppos != 0)
        return 0;

    struct file *fp;
    char value_buff[2] = {'0', 0x0A};
    switch_to_kernel_ds();
    fp = open_file("/sys/class/gpio/gpio164/value", O_RDONLY, 0);
    read_file(fp, value_buff, 1);
    close_file(fp);
    revert_from_kernel_ds();
    copy_to_user(buff, value_buff, 2);
    // printk("ppos=%d buff[0]=%c count=%d\n", *ppos, buff[0], count);
    *ppos = 1;
    return 2;
}

static ssize_t drv_write(struct file *filp, const char *buff, size_t count, loff_t *ppos) {
    uint8_t byte = 0;
    ssize_t result;

    copy_from_user(&byte, buff, 1);
    switch_to_kernel_ds();
    if (byte == 0x31) {
        if (!turn_on_led()) {
            printk("drv_write: turning on led failed");
            result = 0;
        } else {
            printk("drv_write: turn on led successfully");
            result = count;
        }
    } else {
        if (!turn_off_led()) {
            printk("drv_write: turning off led failed");
            result = 0;
        } else {
            printk("drv_write: turn off led successfully");
            result = count;
        }
    }
    revert_from_kernel_ds();
    return result;
}

static int drv_open(struct inode *inode, struct file *filp) {
    printk("device open\n");
    return 0;
}

long drv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    printk("device ioctl\n");
    return 0;
}

static int drv_release(struct inode *inode, struct file *filp){
    printk("device close\n");
    return 0;
}

struct file_operations drv_fops =
{
    read: drv_read,
    write: drv_write,
    unlocked_ioctl: drv_ioctl,
    open: drv_open,
    release: drv_release,
};

static int __init lab5_init(void) { 
    if (register_chrdev(MAJOR_NUM, THE_MODULE_NAME, &drv_fops) < 0) {
        printk("%s: can't get major %d\n", THE_MODULE_NAME, MAJOR_NUM);
        return (-EBUSY);
    }
    printk("%s: started\n", THE_MODULE_NAME);
    return 0; 
} 
 
static void __exit lab5_exit(void) { 
    unregister_chrdev(MAJOR_NUM, THE_MODULE_NAME);
    printk("Remove Lab5-Module successfully\n"); 
} 

module_init(lab5_init); 
module_exit(lab5_exit); 
 
MODULE_DESCRIPTION(THE_MODULE_NAME); 
MODULE_AUTHOR(THE_MODULE_AUTHOR); 
MODULE_LICENSE(THE_MODULE_LICENSE); 
MODULE_ALIAS(THE_MODULE_NAME);
