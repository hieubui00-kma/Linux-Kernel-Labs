#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

int multiplyTwoVectors(int* vect_1, int* vect_2);
void printMatrix(int* matrix, int row, int col);
int* sumMatrices(int* matrix_1, int* matrix_2, int row, int col);
int* subMatrices(int* matrix_1, int* matrix_2, int row, int col);
int* multiplyMatrices(int* matrix_1, int* matrix_2, int row, int col_1, int col_2);
int getConvolution(int k, int n);

static int __init init_main_module(void) {
	int vect_1[2] = {3, 4};
	int vect_2[2] = {7, 9};

	int k = 3, n = 9;

	int matr_1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int matr_2[9] = {10, 11, 12, 13, 14, 15, 16, 17, 18};

	printk(KERN_INFO "Module Init!\n");
	printk(KERN_INFO "Multiply Two Vectors: %d\n", multiplyTwoVectors(vect_1, vect_2));
	
	printk(KERN_INFO "Matrix 1:");
	printMatrix(matr_1, 3, 3);
	
	printk(KERN_INFO "Matrix 2:");
	printMatrix(matr_2,3,3);
	
	printk(KERN_INFO "Sum of 2 matrices:");
	printMatrix(sumMatrices(matr_1, matr_2, 3, 3), 3, 3);
	
	printk(KERN_INFO "Sub of 2 matrices:");
	printMatrix(subMatrices(matr_1, matr_2, 3, 3), 3, 3);
	
	printk(KERN_INFO "Product of 2 matrices:");
	printMatrix(multiplyMatrices(matr_1, matr_2, 3, 3, 3), 3, 3);

	printk(KERN_INFO "Convolution %d of %d: %d\n", k, n, getConvolution(k, n));
	return 0;
}

int multiplyTwoVectors(int* vect_1, int* vect_2) {
	int i, result = 0;

	for(i = 0; i < 2; i++) {
		result += *(vect_1 + i) * (*(vect_2 + i));
	}
	return result;
}

void printMatrix(int* matrix, int row, int col) {
	int i, j;

	for(i = 0; i< row; i++) {
		for(j = 0; j < col; j++) {
			printk(KERN_INFO "%d ", *(matrix +(i * row + j)));
		}
		printk(KERN_INFO "\n");
	}
}

int* sumMatrices(int* matrix_1, int* matrix_2, int row, int col) {
	int i, j;
	int* sum;

	sum = (int*) kmalloc(row * col * sizeof(int), GFP_KERNEL);
	for(i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {
			*(sum + (i * col + j)) = *(matrix_1 +(i * col + j)) + *(matrix_2 + (i * col + j));
		}
	}
	return sum;
}

int* subMatrices(int* matrix_1, int* matrix_2, int row, int col) {
	int i, j;
	int* sub;

	sub = (int*) kmalloc(row * col * sizeof(int), GFP_KERNEL);
	for(i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {
			*(sub + (i * col + j)) = *(matrix_1 +(i * col + j)) - *(matrix_2 + (i * col + j));
		}
	}
	return sub;
}

int* multiplyMatrices(int* matrix_1, int* matrix_2, int row, int col_1, int col_2) {
	int i, j;
	int* result;

	result = (int*) kmalloc(row * col_2 * sizeof(int), GFP_KERNEL);
	for(i = 0; i < row; i++) {
		for(j = 0; j < col_2; j++) {
			*(result + (i * col_2 + j)) += *(matrix_1 +(i * col_1 + j)) * (*(matrix_2 + (i * col_2 + j)));
		}
	}
	return result;
}

int getFactorial(int n) {
	int i, s = 1;

	for(i = 1; i < n; i++) {
		s *= i;
	}
	return s;
}

int getConvolution(int k, int n) {
	if(k == 0 || k == n) {
		return 1;
	}

	if(k == 1) {
		return n;
	}

	return getFactorial(n) / getFactorial(n - k);
}

static void __exit exit_main_module(void) {
  	printk(KERN_INFO "Module Exit!\n");
}

module_init(init_main_module);
module_exit(exit_main_module);

MODULE_LICENSE("GPL");

