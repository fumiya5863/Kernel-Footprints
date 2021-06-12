#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int hello_start(void)
{
    printk("Hello World\n");
    return 0;
}

static void hello_exit(void)
{
    printk("Bye！\n");
}

module_init(hello_start);
module_exit(hello_exit);

MODULE_AUTHOR("fumiya");
MODULE_LICENSE("Dual BSD/GPL");