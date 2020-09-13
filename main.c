// Required by basically all Linux devices
#include <linux/module.h>
#include <linux/init.h>

// For registering devices
#include <linux/fs.h>

MODULE_LICENSE("GPL v2");

dev_t dev;
unsigned int count;

static int __init create_devices(void) {
    int firstminor = 0;
    char* name = "jeff";
    count = 1;

    printk(KERN_ALERT "Printing kernel alert\n");

    alloc_chrdev_region(&dev, firstminor, count, name);

    printk(KERN_ALERT "Created device\n");
    printk(KERN_ALERT "Major version: %u\n", MAJOR(dev));
    printk(KERN_ALERT "Minor version: %u\n", MINOR(dev));

    printk(KERN_DEBUG "created devices\n");
    return 0;
}

static void __exit destroy_devices(void) {
    unregister_chrdev_region(dev, count);

    printk(KERN_DEBUG "destroyed devices\n");
}

module_init(create_devices);
module_exit(destroy_devices);
