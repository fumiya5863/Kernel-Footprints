#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <linux/uaccess.h>

#include "chardev.h"

#define DRIVER_NAME "chardev"

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM = 1;
static unsigned int chardev_major;
static struct cdev chardev_cdev;
static struct class *chardev_class = NULL;
static struct ioctl_info info;

static int chardev_open(struct inode *inode, struct file *filp)
{
    printk("chardev_open\n");
    return 0;
}
  
static int chardev_release(struct inode *inode, struct file *filp)
{
    printk("chardev_release\n");
    return 0;
}
  
static ssize_t chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    printk("chardev_write\n");  
    return count;
}
  
static ssize_t chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk("chardev_read\n");
    return count;
}

static long chardev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    printk("chardev_ioctl\n");
  
    switch (cmd) {
        case SET_DATA:
            printk("SET_DATA\n");
            if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
                return -EFAULT;
            }
            printk("info.size : %ld, info.buf : %s\n", info.size, info.buf);
            break;
        case GET_DATA:
            printk("GET_DATA\n");
            if (copy_to_user((void __user *)arg, &info, sizeof(info))) {
                return -EFAULT;
            }
            break;
        default:
            printk(KERN_WARNING "Failed to %d\n", cmd);
  
        return -EFAULT;
    }
    return 0;
}

struct file_operations char_device_fops = {
    .open = chardev_open,
    .release = chardev_release,
    .read = chardev_read,
    .write = chardev_write,
    .unlocked_ioctl = chardev_ioctl
};

static int chardev_init(void)
{   
    int is_alloc_region = 0, cdev_add_err = 0;
    dev_t dev;

    printk("chardev_init\n");

    is_alloc_region = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
    if (is_alloc_region != 0) {
        printk(KERN_ERR "Failed to alloc_chrdev_region\n");
        return -1;
    }

    chardev_major = MAJOR(dev);

    cdev_init(&chardev_cdev, &char_device_fops);
    chardev_cdev.owner = THIS_MODULE;

    cdev_add_err = cdev_add(&chardev_cdev, dev, MINOR_NUM);
    if (cdev_add_err != 0) {
        printk(KERN_ERR "Failed to cdev_add\n");
        unregister_chrdev_region(dev, MINOR_NUM);
        return -1;
    }

    chardev_class = class_create(THIS_MODULE, DRIVER_NAME);
    if (IS_ERR(chardev_class)) {
        printk(KERN_ERR "Failed to class_create\n");
        cdev_del(&chardev_cdev);
        unregister_chrdev_region(dev, MINOR_NUM);
        return -1;
    }
	
    device_create(chardev_class, NULL, MKDEV(chardev_major, MINOR_BASE), NULL, "chardev%d", MINOR_BASE);

	return 0;
}

static void chardev_exit(void)
{
	dev_t dev = MKDEV(chardev_major, MINOR_BASE);
	
	printk("chardev_exit\n");
	
	device_destroy(chardev_class, MKDEV(chardev_major, MINOR_BASE));

	class_destroy(chardev_class);
	cdev_del(&chardev_cdev);
	unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_AUTHOR("fumiya");
MODULE_LICENSE("Dual BSD/GPL");