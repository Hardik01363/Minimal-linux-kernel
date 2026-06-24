#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>

#define IO_LED 21
#define IO_BUTTON 20
#define IO_OFFSET 0 //May be different for another Raspberry pi (0 for mine)

static struct gpio_desc *led, *button;

static int __init my_init(void) {
    led = gpio_to_desc(IO_LED + IO_OFFSET);
    if(!led){
        printk("gpioctrl - Error getting pin 21");
        return -ENODEV;
    }

    button = gpio_to_desc(IO_BUTTON + IO_OFFSET);
    if(!button){
        printk("gpioctrl - Error getting pin 20");
        return -ENODEV;
    }

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
