#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DRIVER_NAME "chardev"
#define DRIVER_MAJOR 60

struct data {
    unsigned char buffer[BUFFER_SIZE];
};

static int chardev_open(struct inode *inode, struct file *file)
{
    char *str = "HelloWorld";
    int ret_cp_length;

    printk("chardev_open\n");

    struct data *p = kmalloc(sizeof(struct data), GFP_KERNEL);
    if (p == NULL) {
        printk(KERN_ERR "kmalloc - Null");
        return -ENOMEM;
    }

    ret_cp_length = strlcpy(p->buffer, str, sizeof(p->buffer));
    if (ret_cp_length > strlen(str)) {
        printk(KERN_ERR "strlcpy - too long (%d)\n", ret);
    }

    file->private_data = p;

    return 0;
}

static int chardev_close(struct inode *inode, struct file *file)
{
    printk("chardev_release\n");

    if (file->private_data) {
        kfree(file->private_data);
        file->private_data = NULL;
    }

    return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{    
    printk("chardev_read\n");
    
    struct data *p = filp->private_data;
    
    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE;
    }

    if (copy_to_user(buf, p->buffer, count) != 0) {
        return -EFAULT;
    }
    
    return count;
}

static ssize_t chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    printk("chardev_write\n");

    struct data *p = filp->private_data;

    if (copy_from_user(p->buffer, buf, count) != 0) {
        return -EFAULT;
    }
    
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
    
    return is_dev_register;
}

static void chardev_exit(void)
{
    printk("Bye！\n");
    unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_AUTHOR("fumiya");
MODULE_LICENSE("Dual BSD/GPL");