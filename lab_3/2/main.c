#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>

void ChuyenVi(char* XauRo, char* XauMa, int k){
	int i = 0;

	while(XauRo[i] != '\0'){
		if(XauRo[i] >= 97 && XauRo[i] <= 122){
			XauMa[i] = ((((XauRo[i] - 97) + k) % 26) + 97);
		}
		else if (XauRo[i] >= 65 && XauRo[i] <= 90){
			XauMa[i] = ((((XauRo[i] - 65) + k) % 26) + 65);
		}
		else{
			XauMa[i] = XauRo[i];
		}
		i++;
	}
}

void ThayThe(char* XauRo, char* XauMa, char* k){
	int i = 0;

	while(XauRo[i]!='\0'){
		if(XauRo[i] >= 97 && XauRo[i] <= 122){
			XauMa[i] = *(k + (XauRo[i] - 97));
		}
		else if(XauRo[i] >= 65 && XauRo[i] <= 90){
			XauMa[i] = *(k + (XauRo[i] - 65));
		}
		else{
			XauMa[i] = XauRo[i];
		}
		i++;
	}
}

int findMin(char* temp) {
	int j, min, index;

	min = temp[0];
	index = 0;
	for (j = 0; temp[j] != '\0'; j++) {
		if(temp[j] < min) {
			min = temp[j];
			index = j;
		}
	}
	temp[index] = 123;
	return index;
}

int cipher(int i, int r, int l, char arr[22][22], char* XauMa) {
	int j;

	for (j = 0; j < r; j++) { {
			XauMa[l++] = arr[j][i];
		}
	}
	return l;
}

void HoanViToanCuc(char* XauRo, char* XauMa, char* k){
	char arr[22][22];
	char temp[55];
	int index;
	int l = 0, flag = 0, klen, i, j, row, column;

	strcpy(temp, k);
	klen = strlen(k);
	for (i = 0;; i++) {
		if (flag == 1) {
			break;
		}
		for(j = 0; k[j] != '\0'; j++) {
			if(XauRo[l] == '\0') {
				flag = 1;
				arr[i][j] = '-';
			} else {
				arr[i][j] = XauRo[l++];
			}
		}
	}
	row = i;
	column = j;
	
	l = 0;
	for (i = 0; i < klen; i++) {
		index = findMin(temp);
		l = cipher(index, row, l, arr, XauMa);
	}
	XauMa[l] = '\0';
}

static int __init init_main_module(void) {
	char XauRo[30] = "Bui Minh Hieu";

	char* XauMaChuyenVi = (char*) kmalloc(30 * sizeof(char), GFP_KERNEL);
	int k1 = 1;
	
	char* XauMaThayThe = (char*) kmalloc(30 * sizeof(char), GFP_KERNEL);
	char* k2 = "phqgiumeaylnofdxjkrcvstzwb";
	
	char* XauMaHoanViToanCuc = (char*) kmalloc(30 * sizeof(char), GFP_KERNEL);
	char* k3 = "CT3B";

	printk(KERN_INFO "Module Init!\n");

  printk(KERN_INFO "Xau Ro : %s\n",XauRo);
	printk(KERN_INFO "Thuc hien ma hoa chuyen vi:\n");
	ChuyenVi(XauRo, XauMaChuyenVi, k1);	
	printk("Xau Ma chuyen vi: %s\n", XauMaChuyenVi);

	printk(KERN_INFO "\nThuc hien ma hoa thay the");
	ThayThe(XauRo, XauMaThayThe, k2);
	printk("Xau Ma thay the: %s\n",XauMaThayThe);
	
	printk(KERN_INFO "\nThuc hien ma hoa hoan vi toan cuc");
	HoanViToanCuc(XauRo, XauMaHoanViToanCuc, k3);
	printk("Xau Ma hoan vi toan cuc: %s\n", XauMaHoanViToanCuc);
	return 0;
}

static void __exit exit_main_module(void) {
  	printk(KERN_INFO "Module Exit!\n");
}

module_init(init_main_module);
module_exit(exit_main_module);

MODULE_LICENSE("GPL");

