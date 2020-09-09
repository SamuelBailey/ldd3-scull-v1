// Required by basically all Linux devices
#include <linux/module.h>
#include <linux/init.h>

// For registering devices
#include <linux/fs.h>

MODULE_LICENSE("GPL");

static int __init create_devices(void) {
    printk(KERN_DEBUG "created devices\n");
    return 0;
}

static void __exit destroy_devices(void) {
    printk(KERN_DEBUG "destroyed devices\n");
}

module_init(create_devices);
module_exit(destroy_devices)