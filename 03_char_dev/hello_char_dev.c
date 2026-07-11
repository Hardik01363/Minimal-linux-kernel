#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major_dev_num;
//The function definition below taken from linux/fs.h file file_operations struct
static ssize_t my_read(struct file *f, char __user *u, size_t s, loff_t *l) {
    printk("hello_char_dev - read() has been called\n");
    return 0;
}

static int my_open(struct inode *inode_ptr, struct file *file_ptr) {
    ///the struct file, accessed by file_ptr exists only until the file is open.
    pr_info("hello_char_dev - Major Device Number: %d, Minor Device Number: %d\n", imajor(inode_ptr), iminor(inode_ptr));
}

static struct file_operations fops = {
    .read = my_read
};

static int __init my_init(void) {
    //arg 0 allocates a free major dev num itself with all of its 256 minor device numbers. if a specific number passed, all 256 minor dev nums of it are allocated to the user. But, then, 0 is returned on successful registration and error code on failure. so, this code snippet would require some minor changes to fetch the major device number.
    major_dev_num = register_chrdev(0, "hello_char_dev", &fops);
    if(major_dev_num < 0) {
        pr_err("hello_char_dev - Error registering chrdev\n");
        return major_dev_num;
    }
    pr_info("hello_char_dev - chrdev registered with Major Devive Number: %d\n", major_dev_num);
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
