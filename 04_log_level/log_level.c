#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void) {
    //Lower number corresponds to higher priority level
    printk(KERN_DEBUG "log_level - This is a debug message (priority level 7)\n");
    printk(KERN_INFO "log_level - This is an info (priority level 6)\n");
    printk(KERN_WARNING "log_level - This is an warning (priority level 4)\n");
    printk(KERN_ALERT "log_level - This is an alert (priority level 1)\n");
    return 0;
}

static void __exit my_exit(void) {
    pr_err("log_level - Error\n");
    pr_info("log_level - Final info before unloading\n");
    pr_emerg("log_level - Module has been unloaded!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Khandelwal");
MODULE_DESCRIPTION("Simple Hello World bootable module");
