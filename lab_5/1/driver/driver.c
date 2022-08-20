#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "driver.h"

#define MEMORY_SIZE 1024

typedef struct vchar_dev {
	unsigned char* control_regs;
	unsigned char* status_regs;
	unsigned char* data_regs;
} vchar_dev_t;

struct _vchar_drv {
	dev_t dev_num;
	struct class* dev_class;
	struct device* dev;
	vchar_dev_t* vchar_hw;
	struct cdev* vcdev;
} vchar_drv;

int8_t *buf_tmp;
int32_t *kernel_buf2;
int32_t *kernel_buf8;
int8_t *kernel_buf16;

static int select;
static int binNumber;
static int octNumber;
static int hexNumber;

static void dec_to_bin(int n) {
	int arr[100];
	int i = 0;
	int j = 0;

	while(n > 0) {
		arr[i] = n % 2;
		n = n / 2;
		i++;
	}
	binNumber = 0;
	for(i = i - 1; i >= 0; i--) {
		kernel_buf2[j] = arr[i];
		j++;
		binNumber++;
	}
}

static void dec_to_oct(int n) {
	int arr[100];
	int i = 0;
	int j = 0;

	while(n > 0) {
		arr[i] = n % 8;
		n = n / 8;
		i++;
	}
	octNumber = 0;
	for(i = i - 1; i >= 0; i--) {
		kernel_buf8[j] = arr[i];
		j++;
		octNumber++;
	}
}

static void dec_to_hex(int n) {
	int arr[100];
	int i = 0;
	int j = 0;
	int k = 0;

	while(n > 0) {
		arr[i] = n % 16;
		n = n / 16;
		i++;
	}
	hexNumber = 0;
	for(j = i - 1; j >= 0; j--) {
		kernel_buf16[k] = arr[j];
		k++;
		hexNumber++;
	}
}

/****************************** Device specific - START *****************************/
/* Device initialization function */
int init_vchar_hw(vchar_dev_t* hw) {
	char* buffer;

	buffer = kzalloc(NUM_DEV_REGS * REG_SIZE, GFP_KERNEL);
	if (!buffer) {
		return -ENOMEM;
	}

	hw->control_regs = buffer;
	hw->status_regs = hw->control_regs + NUM_CTRL_REGS;
	hw->data_regs = hw->status_regs + NUM_STS_REGS;

	// Initialize registers
	hw->control_regs[CONTROL_ACCESS_REG] = 0x03;
	hw->status_regs[DEVICE_STATUS_REG] = 0x03;

	return 0;
}

/* Device termination function */
void exit_vchar_hw(vchar_dev_t* hw) {
	kfree(hw->control_regs);
}

/******************************* Device specific - END *****************************/

/******************************** OS specific - START *******************************/
/* Entry points function */
static int vchar_driver_open(struct inode* inode, struct file* file) {
	printk("Handle opened event\n");
	return 0;
}

static int vchar_driver_release(struct inode* inode, struct file* file) {
	printk("Handle closed event\n");
	return 0;
}

static ssize_t vchar_driver_read(struct file *filp, char __user *user_buf, size_t len, loff_t *off) {
	if(select == 3) {
		copy_to_user(user_buf, kernel_buf2, MEMORY_SIZE);
		return binNumber;
	}

	if(select == 4) {
		copy_to_user(user_buf, kernel_buf8, MEMORY_SIZE);
		return octNumber;
	}

	if(select == 5) {
		copy_to_user(user_buf, kernel_buf16, MEMORY_SIZE);
		return hexNumber;
	}

	return -1;
}

static ssize_t vchar_driver_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off) {
	if(len == 4) {
		copy_from_user(kernel_buf2, user_buf, len);
		copy_from_user(kernel_buf8, user_buf, len);
		copy_from_user(kernel_buf16, user_buf, len);

		dec_to_bin((int)* kernel_buf2);
		dec_to_oct((int)* kernel_buf8);
		dec_to_hex((int)* kernel_buf16);
	} else {
		copy_from_user(buf_tmp, user_buf, len);
		if(*buf_tmp == '3') {
			select = 3;
		}
		if(*buf_tmp == '4') {
			select = 4;
		}
		if(*buf_tmp == '5') {
			select = 5;
		}
	}
	return 0;
}

static struct file_operations fops = {
	.owner		=	THIS_MODULE,
	.open		=	vchar_driver_open,
	.release	=	vchar_driver_release,
	.read		=	vchar_driver_read,
	.write		=	vchar_driver_write,
};

/* Driver initialization function */
static int __init init_vchar_driver(void) {
	int ret = 0;

	/* Allocation device number */
	vchar_drv.dev_num = 0;
	ret = alloc_chrdev_region(&vchar_drv.dev_num, 0, 1, "lab5.1 Bui Minh Hieu");
	if(ret < 0) {
		printk("Failed to register device number dynamically\n");
		goto failed_register_devnum;
	}
	printk("Allocated device number (%d, %d)\n", MAJOR(vchar_drv.dev_num), MINOR(vchar_drv.dev_num));

	/* Create device file */
	vchar_drv.dev_class = class_create(THIS_MODULE, "class_lab5.1_Bui_Minh_Hieu_dev");
	if (vchar_drv.dev_class == NULL) {
		printk("Failed to create a device class\n");
		goto failed_create_class;
	}

	vchar_drv.dev = device_create(vchar_drv.dev_class, NULL, vchar_drv.dev_num, NULL, "lab5.1_Bui_Minh_Hieu_dev");
	if (IS_ERR(vchar_drv.dev)) {
		printk("Failed to create a device\n");
		goto failed_create_device;
	}

	/* Allocate data structure */
	vchar_drv.vchar_hw = kzalloc(sizeof(vchar_dev_t), GFP_KERNEL);
	if (!vchar_drv.vchar_hw) {
		printk("Failed to allocate data structure of the driver\n");
		ret = -ENOMEM;
		goto failed_allocate_structure;
	}

	/* Initialize a virtual character device */
	ret = init_vchar_hw(vchar_drv.vchar_hw);
	if (ret < 0) {
		printk("Failed to initialize a virtual character device\n");
		goto failed_init_hardware;
	}

	/* Register entry points with kernel */
	vchar_drv.vcdev = cdev_alloc();
	if (vchar_drv.vcdev == NULL) {
		printk("Failed to allocate cdev structure\n");
		goto failed_allocate_cdev;
	}
	
	cdev_init(vchar_drv.vcdev, &fops);
	ret = cdev_add(vchar_drv.vcdev, vchar_drv.dev_num, 1);
	if (ret < 0) {
		printk("Failed to add a char device to the system\n");
		goto failed_allocate_cdev;
	}

	buf_tmp			= kmalloc(MEMORY_SIZE, GFP_KERNEL);
	kernel_buf2 	= kmalloc(MEMORY_SIZE, GFP_KERNEL);
	kernel_buf8 	= kmalloc(MEMORY_SIZE, GFP_KERNEL);
	kernel_buf16 	= kmalloc(MEMORY_SIZE, GFP_KERNEL);

	printk("Initialize 'lab5.1 Bui Minh Hieu' driver successfully\n");
	return 0;

failed_allocate_cdev:
	exit_vchar_hw(vchar_drv.vchar_hw);
failed_init_hardware:
	kfree(vchar_drv.vchar_hw);
failed_allocate_structure:
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
failed_create_device:
	class_destroy(vchar_drv.dev_class);
failed_create_class:
	unregister_chrdev_region(vchar_drv.dev_num, 1);
failed_register_devnum:
	return ret;
}

/* Driver termination function */
static void __exit exit_vchar_driver(void) {
	kfree(buf_tmp);
	kfree(kernel_buf2);
	kfree(kernel_buf8);
	kfree(kernel_buf16);

	/* Unregister entry points with kernel */
	cdev_del(vchar_drv.vcdev);

	/* Free device hardware */
	exit_vchar_hw(vchar_drv.vchar_hw);

	/* Free the memory allocated to the driver's data structure */
	kfree(vchar_drv.vchar_hw);

	/* Remove device file */
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
	class_destroy(vchar_drv.dev_class);

	/* Release device number */
	unregister_chrdev_region(vchar_drv.dev_num, 1);

	printk("Exit 'lab5.1 Bui Minh Hieu' driver\n");
}
/********************************* OS specific - END ********************************/

module_init(init_vchar_driver);
module_exit(exit_vchar_driver);

MODULE_LICENSE("GPL");
