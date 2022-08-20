#include<linux/init.h>
#include<linux/module.h>

int getMax(int arr[100],int n);
int getMin(int arr[100], int n);
int getAverage(int arr[100], int n);

static int init_main_module(void) {
    printk(KERN_ALERT "Module Init!\n");

    int arr[10] = {1, 2, 3, 5, 7, 4, 11, 9, 8, 10};
    int n = 10;

    printk(KERN_ALERT "Max: %d \n", getMax(arr, n));
    printk(KERN_ALERT "Min: %d\n", getMin(arr, n));
    printk(KERN_ALERT "Average: %d\n", getAverage(arr, n));
    return 0;
}

int getMax(int arr[100], int n) {
    int i, max = arr[0];

    for (i = 0; i < n; i++) {
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    return max;
}

int getMin(int arr[100], int n) {
    int i, min = arr[0];

    for (i = 0; i < n; i++){
        if (min > arr[i]) {
            min = arr[i];
        }
    }
    return min;
}

int getAverage(int arr[100], int n) {
    int i ,s = 0;

    for (i = 0; i < n; i++) {
       s += arr[i];
    }
    return s/n;
}

static void exit_main_module(void) {
    printk(KERN_ALERT "Module Exit!\n");
}

MODULE_LICENSE("GPL");
module_init(init_main_module);
module_exit(exit_main_module);
