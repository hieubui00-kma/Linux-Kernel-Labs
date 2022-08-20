#include<linux/init.h>
#include<linux/module.h>

int sum(int arr[100], int n);
int ucln(int a, int b);

static int init_main_module(void) {
    printk(KERN_ALERT "Module Init!\n");

    int arr[5] = {1, 2, 6, 7, 8};
    int n = 5;
    int a = 4;
    int b = 18;

    printk(KERN_ALERT "\n Sum: %d", sum(arr, n));
    printk(KERN_ALERT "\n UCLN(%d, %d): %d\n", a, b, ucln(a,b));
    return 0;
}

int sum(int arr[100], int n) {
    int i, sum = 0;

    for(i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

int ucln(int a, int b) {
    if(b == 0) {
        return a;
    }
    return ucln(b, a % b);
}

static void exit_main_module(void) {
    printk(KERN_ALERT "Module Exit!\n");
}

MODULE_LICENSE("GPL");
module_init(init_main_module);
module_exit(exit_main_module);
