#include <linux/module.h>
#include <linux/fs.h>

struct _vchar_drv {
	dev_t dev_num;
} vchar_drv;

static int __init init_main_driver(void) {
	int ret = 0;

	vchar_drv.dev_num = MKDEV(235, 0);
	ret = register_chrdev_region(vchar_drv.dev_num, 1, "lab4.2 Bui Minh Hieu");
	if(ret < 0){
		printk("Failed to register device number statically\n");
		return ret;
	}

	printk("Initialize vchar driver successfully\n");
	return 0;
}

static void __exit exit_main_driver(void) {
	unregister_chrdev_region(vchar_drv.dev_num, 1);

	printk("Exit main driver\n");
}

module_init(init_main_driver);
module_exit(exit_main_driver);

MODULE_LICENSE("GPL");
