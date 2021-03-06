#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "chardev"
#define DRIVER_MAJOR 60
#define BUFFER_SIZE 256

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
        printk(KERN_ERR "Failed to kamlloc\n");
        return -ENOMEM;
    }

    ret_cp_length = strlcpy(data_p->buffer, str, sizeof(data_p->buffer));
    if (ret_cp_length > strlen(str)) {
        printk(KERN_ERR "Failed to strlcpy\n");
        return ret_cp_length;
    }

    filp->private_data = data_p->buffer;

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
    int is_dev_register = register_chrdev(DRIVER_MAJOR, DRIVER_NAME, &char_device_fops);

    printk("chardev_init\n");

    if (is_dev_register < 0) {
        printk(KERN_ALERT "Failed to register the character device\n");
        return is_dev_register;
    }
    
    return 0;
}

static void chardev_exit(void)
{
    printk("chardev_exit\n");
    unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_AUTHOR("fumiya");
MODULE_LICENSE("Dual BSD/GPL");