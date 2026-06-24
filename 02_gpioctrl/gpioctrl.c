#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>

#define IO_LED 21
#define IO_BUTTON 20
#define IO_OFFSET 0 //May be different for another Raspberry pi (0 for mine)

static struct gpio_desc *led, *button;

static int __init my_init(void) {
    int status;

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

    status = gpiod_direction_input(led);
    if(status) {
        printk("gpioctrl - Error setting pin 21 to input");
        return status;
    }

    status = gpiod_direction_input(button);
    if(status) {
        printk("gpioctrl - Error setting pin 20 to input");
        return status;
    }

    gpiod_set_value(led, 1);
    printk("gpioctrl - Button is %spressed\n", gpiod_get_value(button) ? "" : "not ");

    return 0;
}

static void __exit my_exit(void) {
    gpiod_set_value(led, 0);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Khandelwal");
MODULE_DESCRIPTION("Using GPIO without the device tree");
