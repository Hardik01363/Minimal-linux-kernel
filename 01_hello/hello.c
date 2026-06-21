#include <linux/module.h>
#include <linux/init.h>

int my_init(void) {
    printk("hello - Konnichiwa, Kernel-san!\n");
    return 0;
}

void my_exit(void) {
    printk("hello - Sayonara, Kernel-san!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
