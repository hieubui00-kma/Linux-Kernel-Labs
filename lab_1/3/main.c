#include<linux/init.h>
#include<linux/module.h>

int getSum(int arr[20][20], int n, int m);
int getNumberOfNegativeElements(int arr[20][20], int n, int m);
int getNumberOfPositiveElements(int arr[20][20], int n, int m);
int getMaxInMainDiagonal(int arr[20][20], int n, int m);
int getMinInMainDiagonal(int arr[20][20], int n, int m);
int getMaxInAuxiliaryDiagonal(int arr[20][20], int n, int m);
int getMinInAuxiliaryDiagonal(int arr[20][20], int n, int m);
int getSumOfEvenNumbers(int arr[20][20], int m, int n);
int getSumOfOddNumbers(int arr[20][20], int m, int n);

static int init_main_module(void) {
    printk(KERN_INFO "Module Init!\n");

    int n = 3, m = 3;
    int arr[20][20] = {{1,2,3}, {5,6,7}, {12,13,14}};
	
	printk(KERN_INFO "Sum: %d\n", getSum(arr, n, m));
	printk(KERN_INFO "Number of negative elements: %d\n", getNumberOfNegativeElements(arr, n, m));
	printk(KERN_INFO "Number of positive elements: %d\n", getNumberOfPositiveElements(arr, n, m));
	printk(KERN_INFO "Max value in main diagonal: %d\n", getMaxInMainDiagonal(arr, n, m));
	printk(KERN_INFO "Min value in main diagonal: %d\n", getMinInMainDiagonal(arr, n, m));
	printk(KERN_INFO "Max value in auxiliary diagonal: %d\n", getMaxInAuxiliaryDiagonal(arr, n, m));
	printk(KERN_INFO "Min value in auxiliary diagonal: %d\n", getMinInAuxiliaryDiagonal(arr, n, m));
	printk(KERN_INFO "Sum of even numbers: %d\n", getSumOfEvenNumbers(arr, n, m));
	printk(KERN_INFO "Sum of odd numbers: %d\n", getSumOfOddNumbers(arr, n, m));
    return 0;
}

int getSum(int arr[20][20], int n, int m) {
    int i, j, sum = 0;

	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			sum += arr[i][j];
		}
	}
	return sum;
}

int getNumberOfNegativeElements(int arr[20][20], int n, int m) {
    int i, j, count = 0;

	for(i = 0; i < n;i++) {
		for(j = 0; j < m; j++) {
			if(arr[i][j] < 0) {
				count++;
			}
		}
	}
	return count;

}

int getNumberOfPositiveElements(int arr[20][20], int n, int m) {
    int i, j, count = 0;

	for(i = 0; i < n;i++) {
		for(j = 0; j < m; j++) {
			if(arr[i][j] > 0) {
				count++;
			}
		}
	}
	return count;

}

int getMaxInMainDiagonal(int arr[20][20], int n, int m) {
    int max = arr[0][0];
	int i = 0;

	for(i = 1; i < n; i++) {
		if(arr[i][i] > max) {
			max = arr[i][i];
		}
	}
	return max;

}

int getMinInMainDiagonal(int arr[20][20], int n, int m) {
    int min = arr[0][0];
	int i = 0;

	for(i = 1; i < n; i++) {
		if(arr[i][i] < min) {
			min = arr[i][i];
		}
	}
	return min;
}

int getMaxInAuxiliaryDiagonal(int arr[20][20], int n, int m) {
    int i, j, max = arr[0][n-1];

	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			if((i + j) == (n - 1) && arr[i][j] > max) {
				max = arr[i][j];
				break;
			}
		}
	}
	return max;
}

int getMinInAuxiliaryDiagonal(int arr[20][20], int n, int m) {
    int i, j, min = arr[0][n-1];

	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			if((i+j) == (n-1) && arr[i][j] < min) {
				min = arr[i][j];
				break;
			}
		}
	}
	return min;
}

int getSumOfEvenNumbers(int arr[20][20], int n, int m) {
	int i,j, sum = 0;

	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			if(arr[i][j] % 2 == 0) {
				sum += arr[i][j];
			}
		}
	}
	return sum;
}

int getSumOfOddNumbers(int arr[20][20], int n, int m) {
	int i,j, sum = 0;

	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			if(arr[i][j] % 2 != 0) {
				sum += arr[i][j];
			}
		}
	}
	return sum;
}

static void exit_main_module(void) {
    printk(KERN_INFO "Module Exit!\n");
}

MODULE_LICENSE("GPL");
module_init(init_main_module);
module_exit(exit_main_module);
