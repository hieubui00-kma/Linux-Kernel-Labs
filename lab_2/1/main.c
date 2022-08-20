#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

void strrev(char *str, const size_t size);
int ArrayCharToInt(char a[]);
long long power(int base, int exp);
int HexToBin(char hex[65]);
int DecToBin(int dec);
void DecToHex(char hex[65],int dec);
int BinToOct(long long bin);
void OctToHex(char hex[65],int oct);
int HexToOct(char hex[65]);
int BinToDec(long long bin);

static int __init init_main_module(void) {
  printk(KERN_INFO "Module Init!");

  long long bin = 1000110;
  int oct = 127;
  int dec = 198;
  char hex[65] = "AF1";

  printk(KERN_INFO "%s(16) -> %d(2)\n", hex, HexToBin(hex));
  printk(KERN_INFO "%d(10) -> %d(2)\n", dec, DecToBin(dec));

  DecToHex(hex,dec);
  printk(KERN_INFO "%d(10) -> %s(16)\n", dec, hex);

  printk(KERN_INFO "%lld(2) -> %d(8)\n", bin, BinToOct(bin));

  OctToHex(hex, oct);
  printk(KERN_INFO "%d(8) -> %s(16)\n", oct, hex);

  printk(KERN_INFO "%s(16) -> %d(8)\n", hex, HexToOct(hex));

  printk(KERN_INFO "%lld(2) -> %d(10)\n", bin, BinToDec(bin));
  return 0;
}

void strrev(char *str, const size_t size) {
    char temp = '\0';
  size_t i = 0;
  size_t j = size - 1;

  while(i < j){
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
      i++;
      j--;
  }
}

int ArrayCharToInt(char a[]) {
    int c, sign, offset, n;
  if (a[0] == '-') {
    sign = -1;
  }
  if (sign == -1) {
    offset = 1;
  } else {
    offset = 0;
  }
  n = 0;
  for (c = offset; a[c] != '\0'; c++) {
    n = n * 10 + a[c] - '0';
  }
  if (sign == -1) {
    n = -n;
  }
  return n;
}

long long power(int base, int exp) {
    long long result = 1;
    while (exp != 0) {
            result *= base;
            --exp;
    }
    return result;
}

int HexToBin(char hex[65]) {
  int i;
  char bin[65]= "";
  for(i=0; hex[i]!='\0'; i++) {
        switch(hex[i]) {
            case '0':
                strcat(bin, "0000");
                break;
            case '1':
                strcat(bin, "0001");
                break;
            case '2':
                strcat(bin, "0010");
                break;
            case '3':
                strcat(bin, "0011");
                break;
            case '4':
                strcat(bin, "0100");
                break;
           case '5':
                strcat(bin, "0101");
                break;
            case '6':
                strcat(bin, "0110");
                break;
            case '7':
                strcat(bin, "0111");
                break;
            case '8':
                strcat(bin, "1000");
                break;
            case '9':
                strcat(bin, "1001");
                break;
            case 'a':
            case 'A':
                strcat(bin, "1010");
                break;
            case 'b':
            case 'B':
                strcat(bin, "1011");
                break;
            case 'c':
            case 'C':
                strcat(bin, "1100");
                break;
            case 'd':
            case 'D':
                strcat(bin, "1101");
                break;
            case 'e':
            case 'E':
                strcat(bin, "1110");
                break;
            case 'f':
            case 'F':
                strcat(bin, "1111");
                break;
            default:
                printk("Invalid hexadecimal input.");
        }
    }
    return ArrayCharToInt(bin);
}

int DecToBin(int dec){
    int tempDecimal;
    char binary[65] = "";
    int index = 0;

    tempDecimal = dec;
    while(tempDecimal > 0) {
        binary[index] = (tempDecimal % 2) + '0';
        tempDecimal /= 2;
        index++;
    }
    binary[index] = '\0';
    strrev(binary,strlen(binary));
    return ArrayCharToInt(binary);
}

void DecToHex(char hex[65],int dec) {
    char HEXVALUE[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    long long tempDecimal;
    int index, rem;
    
    tempDecimal = dec;
    index = 0;
    while(tempDecimal !=0) {
        rem = tempDecimal % 16;
        hex[index] = HEXVALUE[rem];
        tempDecimal /= 16;
        index++;
    }
    hex[index] = '\0';
    strrev(hex,strlen(hex));
}

int BinToOct(long long bin) {
    int dec, oct = 0, i=1;
    
    dec = BinToDec(bin);
    while (dec != 0) {
        oct += (dec % 8) * i;
        dec /= 8;
        i *= 10;
    }
    return oct;
}

void OctToHex(char hex[65],int oct) {
    int octtobinary[] = {0,1,10,11,100,101,110,111};
    char hexadecimal[10];
    long int binary=0;
    int rem = 0;
    int position = 1;
    int len = 0;
    int i, k = 0;

    while(oct != 0) {
      rem = oct%10;  
      binary = octtobinary[rem]*position + binary;  
      oct = oct/10;  
      position = position*1000;  
    }
    
    while(binary > 0) {
        rem = binary % 10000;  
        switch(rem)  {
            case 0:  
                strcat(hexadecimal, "0");  
               break;  
            case 1:  
                strcat(hexadecimal, "1");  
                break;  
            case 10:  
                strcat(hexadecimal, "2");  
                break;  
            case 11:  
                strcat(hexadecimal, "3");  
                break;  
            case 100:  
                strcat(hexadecimal, "4");  
                break;  
            case 101:  
                strcat(hexadecimal, "5");  
                break;  
            case 110:  
                strcat(hexadecimal, "6");  
                break;  
            case 111:  
                strcat(hexadecimal, "7");  
                break;  
            case 1000:  
                strcat(hexadecimal, "8");  
                break;  
            case 1001:  
                strcat(hexadecimal, "9");  
                break;  
            case 1010:  
                strcat(hexadecimal, "A");  
                break;  
            case 1011:  
                strcat(hexadecimal, "B");  
                break;  
            case 1100:  
                strcat(hexadecimal, "C");  
                break;  
            case 1101:  
                strcat(hexadecimal, "D");  
                break;  
            case 1110:  
                strcat(hexadecimal, "E");  
                break;  
            case 1111:  
                strcat(hexadecimal, "F");  
            break;  
        }  
        len = len + 1;  
        binary /= 10000;  
    }  
    
    for(i = len-1; i >= 0; i--) {
        hex[k] = hexadecimal[i];
        k++;
    }
    hex[len] = '\0';
}

int HexToOct(char hex[65]) {
    int octal, bin;

    bin = HexToBin(hex);
    octal = BinToOct(bin);
    return octal;
}

int BinToDec(long long bin) {
    int dec = 0, i = 0, rem;
    while (bin != 0) {
        rem = bin % 10;
        bin /= 10;
        dec += rem * power(2,i);
        ++i;
    }
    return dec;
}

static void __exit exit_main_module(void) {
  printk(KERN_INFO "Module Exit!\n");
}

module_init(init_main_module);
module_exit(exit_main_module);

MODULE_LICENSE("GPL");
