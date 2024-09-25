#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "kplayground"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hedi Masmoudi");
MODULE_DESCRIPTION("Simple read/write kernel module with logging");
MODULE_VERSION("1.1");

static char device_buffer[BUFFER_SIZE];
static int buffer_size = 0;

/* Fonction read */
static ssize_t kplayground_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    int bytes_to_read = buffer_size - *ppos;
    
    if (bytes_to_read <= 0) {
        return 0; // Rien à lire
    }

    if (bytes_to_read > count) {
        bytes_to_read = count;
    }

    if (copy_to_user(buf, device_buffer + *ppos, bytes_to_read)) {
        return -EFAULT;
    }

    *ppos += bytes_to_read;
    
    /* Ajout d'un message dans le journal du noyau pour read */
    printk(KERN_INFO "kplayground: read %d bytes from the device\n", bytes_to_read);

    return bytes_to_read;
}

/* Fonction write */
static ssize_t kplayground_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    int bytes_to_write = count;

    if (*ppos >= BUFFER_SIZE) {
        return -ENOMEM; // Plus d'espace dans le tampon
    }

    if (bytes_to_write > BUFFER_SIZE - *ppos) {
        bytes_to_write = BUFFER_SIZE - *ppos;
    }

    if (copy_from_user(device_buffer + *ppos, buf, bytes_to_write)) {
        return -EFAULT;
    }

    *ppos += bytes_to_write;
    buffer_size = *ppos;

    /* Ajout d'un message dans le journal du noyau pour write */
    printk(KERN_INFO "kplayground: wrote %d bytes to the device\n", bytes_to_write);

    return bytes_to_write;
}

/* Définition des opérations du périphérique */
static const struct file_operations kplayground_fops = {
    .owner = THIS_MODULE,
    .read = kplayground_read,
    .write = kplayground_write,
};

/* Déclaration du périphérique misc */
static struct miscdevice kplayground_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &kplayground_fops,
};

/* Fonction d'initialisation du module */
static int __init kplayground_init(void) {
    int ret;

    ret = misc_register(&kplayground_device);
    if (ret) {
        printk(KERN_ERR "Unable to register kplayground misc device\n");
        return ret;
    }

    printk(KERN_INFO "kplayground module loaded\n");
    return 0;
}

/* Fonction de nettoyage du module */
static void __exit kplayground_exit(void) {
    misc_deregister(&kplayground_device);
    printk(KERN_INFO "kplayground module unloaded\n");
}

module_init(kplayground_init);
module_exit(kplayground_exit);
