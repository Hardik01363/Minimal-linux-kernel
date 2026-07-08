#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major_dev_num;
static struct file_operations fops = {};

static int __init my_init(void) {
    //arg 0 allocates a free najor dev num itself. if a specific number passed, all 256 minor dev nums of it are allocated to the user.
    major_dev_num = register_chrdev(0, "hello_char_dev", &fops);
    if(major_dev_num < 0) printk("hello_char_dev - Error registering chrdev\n");
    printk("hello_char_dev - chrdev registered with Major Devive Number: %d\n", major_dev_num);
    return 0;
}

static void __exit my_exit(void) {
    unregister_chrdev(major_dev_num, "hello_char_dev");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Khandelwal");
MODULE_DESCRIPTION("Registers a character device (sample module)");
