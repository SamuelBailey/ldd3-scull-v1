// Required by basically all Linux devices
#include <linux/module.h>
#include <linux/init.h>

// For registering devices
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL v2");

/*
 *  Globals
 */
dev_t dev;
unsigned int count;
struct cdev * scull_cdev;

/* Function prototypes */

int scull_open(struct inode * pinode, struct file * pfile);
ssize_t scull_read(struct file * pfile, char __user * buffer, size_t length, loff_t * offset);
ssize_t scull_write(struct file * pfile, const char __user * buffer, size_t length, loff_t * offset);
int scull_release(struct inode * pinode, struct file * pfile);

// Some things that I need to register my device
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = scull_open,
	.read = scull_read,
	.write = scull_write,
    .release = scull_release
};

/* Define system call functions */

int scull_open(struct inode * pinode, struct file * pfile)
{
    printk(KERN_ALERT "Opened file\n");
    return 0;
}

ssize_t scull_read(struct file * pfile, char __user * buffer, size_t length, loff_t * offset)
{
    printk(KERN_ALERT "Read %lu bytes\n", length);
    return 0;
}

ssize_t scull_write(struct file * pfile, const char __user * buffer, size_t length, loff_t * offset)
{
    printk(KERN_ALERT "Wrote %lu bytes\n", length);
    return length;
}

int scull_release(struct inode * pinode, struct file * pfile)
{
    printk(KERN_ALERT "Closed file\n");
    return 0;
}


static int __init create_devices(void) {
    int firstminor = 0;
    char* name = "scull";
    count = 1;
    dev = 0;
    scull_cdev = cdev_alloc();

    printk(KERN_ALERT "Printing kernel alert\n");

    alloc_chrdev_region(&dev, firstminor, count, name);

    printk(KERN_ALERT "Created device\n");
    printk(KERN_ALERT "Major version: %u\n", MAJOR(dev));
    printk(KERN_ALERT "Minor version: %u\n", MINOR(dev));

    /* Create a character device object */

    if (scull_cdev != NULL) {
        scull_cdev->ops = &fops;
        scull_cdev->owner = THIS_MODULE;
    } else {
        printk(KERN_WARNING "scull_cdev is NULL\n");
    }

    if (cdev_add(scull_cdev, dev, count) == 0) {
        printk(KERN_WARNING "cdev_add has failed to add to the system\n");
    }

    printk(KERN_DEBUG "created device classes\n");

    
    return 0;
}

/* Comment out 
static void scull_setup_cdev(struct scull_dev* dev, int index) {
    int err, devno = MKDEV(scull_major, scull_minor + index);

}
*/

static void __exit destroy_devices(void) {
    cdev_del(scull_cdev);

    unregister_chrdev_region(dev, count);

    printk(KERN_DEBUG "destroyed devices\n");
}
module_init(create_devices);
module_exit(destroy_devices);
