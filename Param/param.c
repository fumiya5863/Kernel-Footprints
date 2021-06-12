#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int param;
module_param(param, int, S_IRUGO);

static int param_init(void)
{
    printk("%d param\n", param);
    return 0;
}

static void param_exit(void)
{
    printk("Bye！\n");
}

module_init(param_init);
module_exit(param_exit);

MODULE_AUTHOR("fumiya");
MODULE_LICENSE("Dual BSD/GPL");