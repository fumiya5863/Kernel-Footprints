#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "chardev"
#define BUFFER_SIZE 256

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM = 2;
static unsigned int chardev_major;
static struct cdev chardev_cdev;
static struct class *chardev_class = NULL;

struct data {
    unsigned char buffer[BUFFER_SIZE];
};

static int chardev_open(struct inode *inode, struct file *filp)
{
    char *str = "HelloWorld";
    int ret_cp_length;
    struct data *data_p = kmalloc(sizeof(struct data), GFP_KERNEL);

    printk("chardev_open\n");

    if (data_p == NULL) {
        printk(KERN_ERR "Failed to kmalloc");
        return -ENOMEM;
    }

    ret_cp_length = strlcpy(data_p->buffer, str, sizeof(data_p->buffer));
    if (ret_cp_length > strlen(str)) {
        printk(KERN_ERR "Failed to strlcpy\n");
        return ret_cp_length;
    }

    filp->private_data = data_p;

    return 0;
}

static int chardev_release(struct inode *inode, struct file *filp)
{
    printk("chardev_release\n");

    if (filp->private_data) {
        kfree(filp->private_data);
        filp->private_data = NULL;
    }

    return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{    
    struct data *data_p = filp->private_data;

    printk("chardev_read\n");
    
    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE;
    }

    if (copy_to_user(buf, data_p->buffer, count) != 0) {
        return -EFAULT;
    }
    
    return count;
}

static ssize_t chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    struct data *data_p = filp->private_data;

    printk("chardev_write\n");

    if (copy_from_user(data_p->buffer, buf, count) != 0) {
        return -EFAULT;
    }

    printk("%s", data_p->buffer);
    
    return count;
}

struct file_operations char_device_fops = {
    .open = chardev_open,
    .release = chardev_release,
    .read = chardev_read,
    .write = chardev_write
};

static int chardev_init(void)
{   
    int is_alloc_region = 0, cdev_add_err = 0;
    int i;
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

    for (i = MINOR_BASE; i < MINOR_BASE + MINOR_NUM; i++) {
		device_create(chardev_class, NULL, MKDEV(chardev_major, i), NULL, "chardev%d", i);
	}

	return 0;
}

static void chardev_exit(void)
{
	int i;	
	dev_t dev = MKDEV(chardev_major, MINOR_BASE);
	
	printk("chardev_exit\n");
	
	for (i = MINOR_BASE; i < MINOR_BASE + MINOR_NUM; i++) {
		device_destroy(chardev_class, MKDEV(chardev_major, i));
	}

	class_destroy(chardev_class);
	cdev_del(&chardev_cdev);
	unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(chardev_init);
module_exit(chardev_exit);
