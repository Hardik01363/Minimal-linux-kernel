#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void) {
    printk("hello - Konnichiwa, Kernel-san!\n");
    return 0;
}

static void __exit my_exit(void) {
    printk("hello - Sayonara, Kernel-san!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Khandelwal");
MODULE_DESCRIPTION("Slightly Improved Hello World Module");
