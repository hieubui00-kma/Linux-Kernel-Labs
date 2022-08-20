#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

int array[100];
int n = 0;

module_param(n, int, S_IRUSR | S_IWUSR);
module_param_array(array, int, NULL, S_IRUSR | S_IWUSR);

int getGreatestCommonDivisorOfTwoNumber(int a, int b);
int getGreatestCommonDivisorOfArray(void);

static int __init init_main_module(void) {
    printk(KERN_INFO "Module Init!\n");
	printk(KERN_INFO "Greatest Common Divisor Of Array: %d", getGreatestCommonDivisorOfArray());
    return 0;
}

int getGreatestCommonDivisorOfTwoNumber(int a, int b) {
    if (a == b) {
        return a;
    }

    if (a > b) {
		return getGreatestCommonDivisorOfTwoNumber(a - b, b);
	}

    return getGreatestCommonDivisorOfTwoNumber(a, b - a);
}

int getGreatestCommonDivisorOfArray(void) {
    int temp;
    int i = 0;

    if (n == 1) {
        return abs(array[0]);
    }

    temp = abs(array[0]);
	for (i = 0; i < n; i++) {
		temp = getGreatestCommonDivisorOfTwoNumber(temp, abs(array[i]));
	}
	return temp;
}

static void __exit exit_main_module(void) {
    printk(KERN_INFO "Module Exit!\n");
}

module_init(init_main_module);
module_exit(exit_main_module);

MODULE_LICENSE("GPL");
