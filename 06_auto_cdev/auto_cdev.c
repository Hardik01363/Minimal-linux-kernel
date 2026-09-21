#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;

//The function definitions below are taken from linux/fs.h file file_operations struct
//my_read is a pseudo function and doesnt actually read the file
static ssize_t my_read(struct file *f, char __user *u, size_t s, loff_t *l) {
    printk("auto_cdev - read() has been called\n");
    return 0;
}

static int my_open(struct inode *inode_ptr, struct file *file_ptr) {
    //the struct file, accessed by file_ptr exists only until the file is open.
    pr_info("auto_cdev - Major Device Number: %d, Minor Device Number: %d\n", imajor(inode_ptr), iminor(inode_ptr));

    //accessing linux file struct
    pr_info("auto_cdev - file_ptr->f_pos: %lld\n", file_ptr->f_pos);
    pr_info("auto_cdev - file_ptr->f_mode: %lld\n", file_ptr->f_mode);
    pr_info("auto_cdev - file_ptr->f_flags: %lld\n", file_ptr->f_flags);

    return 0;
}

static int my_release(struct inode *inode_ptr, struct file *file_ptr) {
    pr_info("auto_cdev - File has been closed");
    return 0;
}
static struct file_operations fops = {
    .read = my_read,
    .open = my_open,
    .release = my_release,
};

static int __init my_init(void) {
    int status;
#ifdef STATIC_DEVNR
    dev_num = STATIC_DEVNR;
    //MINORMASK sets all 20 bits of the 20 bit minor dev num field to 1 (2^20 - 1), but, minor dev nums go from 0 to 2^20 - 1 (total 2^20), so, +1 added to MINORMASK.
    status = register_chrdev_region(dev_num, MINORMASK + 1, "manual_cdev");
#else
    status = alloc_chrdev_region(&dev_num, 0, MINORMASK + 1, "auto_cdev");
#endif
    if(status) {
        pr_err("auto_cdev - Error reserving the region of device numbers\n");
        return status;
    }

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    
    status = cdev_add(&my_cdev, dev_num, MINORMASK + 1);
    if(status) {
        pr_err("auto_cdev - Error registering char dev\n");
        goto free_devnum;
    }

    pr_info("auto_cdev - registered a char dev with major device num %d starting with minor device num %d\n", MAJOR(dev_num), MINOR(dev_num));
    
    my_class = class_create("my_class"); //class_create() creates a class and returns NULL if failed
    if(!my_class) {
        pr_err("auto_cdev - could not create class my_class\n");
        status = ENOMEM; //possibly the only reason for the function to fail (acc to my current knowledge)
        goto delete_cdev;
    }
    if(!device_create(my_class, NULL, dev_num, NULL, "auto_cdev%d", 0)) {
        pr_err("auto_cdev - could not create device auto_cdev0\n");
        status = ENOMEM; //possibly the only reason for the function to fail (acc to my current knowledge)
        goto delete_class;
    }
    
    return 0;

delete_class:
    class_unregister(my_class);
    class_destroy(my_class);
delete_cdev:
    cdev_del(&my_cdev);
free_devnum:
    unregister_chrdev_region(dev_num, MINORMASK + 1);
    return status;
}

static void __exit my_exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, MINORMASK + 1);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Khandelwal");
MODULE_DESCRIPTION("Registers a character device (custom manual implementation), creates device file within the kernel module itself and allows opening and closing of file.");
