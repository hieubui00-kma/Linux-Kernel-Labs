#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <crypto/hash.h>
#include <linux/slab.h>
#include <linux/types.h>

#define DEV_NAME "hashkm"
#define DEV_CLASS "hashlkm"
#define BUF_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("lab71");
MODULE_VERSION("1.0");

typedef enum hash
{
    MD5 = 0,
    SHA1,
    SHA256
}
hash_t;

typedef struct userspace
{
    u8 plaintext[BUF_SIZE];
    hash_t tfm;
} userspace_t;

static int dev_num;
static struct class *hashkm_class = NULL;
static struct device *hashkm_dev = NULL;
static size_t open_count = 0;
static userspace_t userspace;
static char digest[BUF_SIZE];

static int hashkm_open(struct inode *, struct file *);
static int hashkm_close(struct inode *, struct file *);
static ssize_t hashkm_read(struct file *, char *, size_t, loff_t *);
static ssize_t hashkm_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fo = {
    .owner = THIS_MODULE,
    .open = hashkm_open,
    .release = hashkm_close,
    .read = hashkm_read,
    .write = hashkm_write,
};

static int __init init(void) {
    printk(KERN_INFO "Bat dau!");
    dev_num = register_chrdev(0, DEV_NAME, &fo);
    if (dev_num < 0)
	{
		printk(KERN_INFO "%s: Error register major number\n", DEV_NAME);
		return dev_num;
	}
    printk(KERN_INFO "%s: Successfully registered(major number: %d)\n", DEV_NAME, dev_num);
    hashkm_class = class_create(THIS_MODULE, DEV_CLASS);
	if (IS_ERR(hashkm_class))
	{
		unregister_chrdev(dev_num, DEV_NAME);
		printk(KERN_INFO "%s: unable to register device class\n", DEV_NAME);
		return PTR_ERR(hashkm_class);
	}

    printk(KERN_INFO "%s: Device class successfully register\n", DEV_NAME);

	hashkm_dev = device_create(hashkm_class, NULL, MKDEV(dev_num, 0), NULL, DEV_NAME);
	if (IS_ERR(hashkm_dev))
	{
		class_destroy(hashkm_class);
		unregister_chrdev(dev_num, DEV_NAME);
		pr_alert("%s: failed to create a new device\n", DEV_NAME);
		return PTR_ERR(hashkm_dev);
	}

	printk(KERN_INFO "%s: device driver successfully created\n", DEV_NAME);
	return 0;
}

static void __exit exitt(void)
{
	device_destroy(hashkm_class, MKDEV(dev_num, 0));
	class_unregister(hashkm_class);
	class_destroy(hashkm_class);
	unregister_chrdev(dev_num, DEV_NAME);
	printk(KERN_INFO "%s: Module successfully unloaded\n", DEV_NAME);
    printk(KERN_INFO "Ket thuc!");
}

static int hashkm_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "%s: This device has been opened %d time(s)\n", DEV_NAME, (int)++open_count);

	return 0;
}

static int hashkm_close(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "%s: device closed successfully\n", DEV_NAME);

	return 0;
}

static ssize_t hashkm_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	size_t bytes_not_copied = 0;
	size_t bytes_to_copy = (userspace.tfm == SHA256) ? 32 : (userspace.tfm == SHA1 ? 20 : 16);

	bytes_not_copied = copy_to_user(buffer, digest, bytes_to_copy);
	if (bytes_not_copied)
	{
		pr_warn("%s: Failed to send result to userspace\n", DEV_NAME);
		return -EFAULT;
	}

	return 0;
}

static ssize_t hashkm_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	size_t bytes_not_copied = 0;
	struct crypto_shash *tfm;
	struct shash_desc *desc;
	int err;

	bytes_not_copied = copy_from_user(&userspace, buffer, sizeof(userspace_t));
	if (bytes_not_copied)
	{
		printk(KERN_INFO "%s: error while copying %zu bytes from userspca\n", DEV_NAME, bytes_not_copied);
		return -EFAULT;
	}

	switch (userspace.tfm)
	{
	case MD5:
		tfm = crypto_alloc_shash("md5", 0, 0);
		break;
	case SHA1:
		tfm = crypto_alloc_shash("sha1", 0, 0);
		break;
	case SHA256:
		tfm = crypto_alloc_shash("sha256", 0, 0);
		break;
	default:
		printk(KERN_INFO "%s: hashing algorithm not recognized\n", DEV_NAME);
		return -EFAULT;
	}

	if (IS_ERR(tfm))
	{
		printk(KERN_INFO "%s: Hashing algorithm not supported\n", DEV_NAME);
		return -EFAULT;
	}

	desc = kmalloc(sizeof(struct shash_desc) + crypto_shash_descsize(tfm), GFP_KERNEL);
	if (!desc)
	{
		pr_err("%s: failed to allocate memory(kmalloc)\n", DEV_NAME);
		return -ENOMEM;
	}
	desc->tfm = tfm;

	err = crypto_shash_init(desc);
	if (err)
	{
		pr_err("%s: failed to initialize shash\n", DEV_NAME);
		goto out;
	}

	err = crypto_shash_update(desc, userspace.plaintext, strlen(userspace.plaintext));
	if (err)
	{
		pr_err("%s: failed to execute hashing function\n", DEV_NAME);
		goto out;
	}

	err = crypto_shash_final(desc, digest);
	if (err)
	{
		pr_err("%s: Failed to complete hashing function\n", DEV_NAME);
		goto out;
	}

	crypto_free_shash(tfm);
	kfree(desc);

	pr_info("%s: String successfully hashed. Read from this device to get the result\n", DEV_NAME);

	return 0;

out: 
	crypto_free_shash(tfm);
	kfree(desc);
	return err;
}
module_init(init);
module_exit(exitt);


