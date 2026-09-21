#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;

//The function definitions below are taken from linux/fs.h file file_operations struct
static ssize_t my_read(struct file *filp, char __user *user_buf, size_t len, loff_t *off)
{
	int not_copied, delta, to_copy = (len + *off) < sizeof(text) ? len : (sizeof(text) - *off);

	pr_info("cdev - read is called, we want to read %ld bytes, but actually only copying %d bytes. The offset is %lld\n", len, to_copy, *off);

	if (*off >= sizeof(text))
		return 0;

	not_copied = copy_to_user(user_buf, &text[*off], to_copy);
	delta = to_copy - not_copied;
	if (not_copied) 
		pr_warn("cdev - could only copy %d bytes\n", delta);

	*off += delta;

	return delta;
}

static ssize_t my_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off)
{
	int not_copied, delta, to_copy = (len + *off) < sizeof(text) ? len : (sizeof(text) - *off);

	pr_info("cdev - write is called, we want to write %ld bytes, but actually only copying %d bytes. The offset is %lld\n", len, to_copy, *off);

	if (*off >= sizeof(text))
		return 0;

	not_copied = copy_from_user(&text[*off], user_buf, to_copy);
	delta = to_copy - not_copied;
	if (not_copied) 
		pr_warn("cdev - could only copy %d bytes\n", delta);

	*off += delta;
	return delta;
}

static int my_open(struct inode *inode_ptr, struct file *file_ptr) {
    //the struct file, accessed by file_ptr exists only until the file is open.
    return 0;
}

static int my_release(struct inode *inode_ptr, struct file *file_ptr) {
    return 0;
}
static struct file_operations fops = {
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_release
};

static int __init my_init(void) {
    int status;
#ifdef STATIC_DEVNR
    dev_num = STATIC_DEVNR;
    //MINORMASK sets all 20 bits of the 20 bit minor dev num field to 1 (2^20 - 1), but, minor dev nums go from 0 to 2^20 - 1 (total 2^20), so, +1 added to MINORMASK.
    status = register_chrdev_region(dev_num, MINORMASK + 1, "manual_cdev");
#else
    status = alloc_chrdev_region(&dev_num, 0, MINORMASK + 1, "cdev");
#endif
    if(status) {
        pr_err("cdev - Error reserving the region of device numbers\n");
        return status;
    }

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    
    status = cdev_add(&my_cdev, dev_num, MINORMASK + 1);
    if(status) {
        pr_err("cdev - Error registering char dev\n");
        goto free_devnum;
    }

    pr_info("cdev - registered a char dev with major device num %d starting with minor device num %d\n", MAJOR(dev_num), MINOR(dev_num));
    
    my_class = class_create("my_class"); //class_create() creates a class and returns NULL if failed
    if(!my_class) {
        pr_err("cdev - could not create class my_class\n");
        status = ENOMEM; //possibly the only reason for the function to fail (acc to my current knowledge)
        goto delete_cdev;
    }
    if(!device_create(my_class, NULL, dev_num, NULL, "cdev%d", 0)) {
        pr_err("cdev - could not create device cdev0\n");
        status = ENOMEM; //possibly the only reason for the function to fail (acc to my current knowledge)
        goto delete_class;
    }
    pr_info("cdev - created device in /sys/class/my_class/cdev0\n");
    
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
    device_destroy(my_class, dev_num);
    class_unregister(my_class);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, MINORMASK + 1);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Khandelwal");
MODULE_DESCRIPTION("Registers a character device (custom manual implementation), creates device file within the kernel module itself and allows opening and closing of file.");
