#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/ctype.h>

int str_length(char *str);
char* str_reverse_by_length(char* str);
char* str_standardized(char *str);
char* str_append(char *str1, char *str2);

static int __init init_main_module(void) {
  char a[16] = "Hoc vien ";
  char b[16] = "Ky thuat Mat Ma";
  char c[128] = "   hOc   vIEn  Ky thUaT MaT mA        ";

	printk(KERN_INFO "Module Init!");
  printk(KERN_INFO "Count '%s' = %d\n", a, str_length(a));
  printk(KERN_INFO "Reverse By Length '%s\' = %s\n", b, str_reverse_by_length(b));
  printk(KERN_INFO "Standardized '%s' = ", c);
	printk(KERN_INFO "%s\n", str_standardized(c));
  printk(KERN_INFO "Append '%s', '%s' = %s\n", a, b, str_append(a,b));
  return 0;
}

int str_length(char *str) {
  int length = 0;

  while (*(str + length) != '\0') {
    length++;
  }
  return length;
}

char* str_reverse_by_length(char* str) {
	int i, length = str_length(str);
	char* result = (char*) kmalloc(length * sizeof(char), GFP_KERNEL);
  
	for (i = 0; i < length; i++)  {
		*(result+i) = *(str + length - 1 - i);
	}
	return result;
}

int is_white_space(char c) {
  if (c == ' ' || c == '\t' || c == '\n') {
    return 1;
  }

  return 0;
}

char* str_append(char *str1, char *str2) {
	int i;
	int length1 = str_length(str1);
	int length2 = str_length(str2);
	int count = length1 + length2;
	char* result = (char*) kmalloc(count * sizeof(char), GFP_KERNEL);

	for(i = 0; i < count; i++){
		if(i < length1) {
			*(result + i) = *(str1 + i);
		}else{
			*(result + i) = *(str2 + (i - length1));
		}
	}
	return result;
}

int SpecialCharacter(char c) {
	if(c == ',' || c == '.' || c == '(' || c == ')' || c == '?' || c == ':' || c == ';' || c == '!') {
		return 1;
	}

	return 0;
}

void str_trim(char *str) {
  int length = str_length(str);
  int i, counter = 0;

  i = length - 1;
  while(is_white_space(str[i])) {
    str[i] = '\0';
    i--;
  }

  length = str_length(str);
  for(i = 0; i < length; i++) {
    if(is_white_space(str[i])) {
      counter++;
    } else {
      break;
    }
  }

  for(i = 0; i <= length - counter; i++) {
    str[i] = str[i + counter];
  }
}

void str_trim_middle(char* str) {
  int i, j;
  int length = str_length(str);

  for(i = 0; i < length; i++) {
    if(is_white_space(str[i]) && is_white_space(str[i + 1])) {
			for(j = i + 1; j < length; j++) {
				str[j] = str[j + 1];
			}
			i--;
			length--;
		} else if(is_white_space(str[i]) && SpecialCharacter(str[i + 1])) {
			for(j = i; j < length; j++) {
				str[j] = str[j + 1];
			}
			i--;
			length--;
		} else if(str[i] == '\t') {
			str[i] = ' ';
		}
	}
}

char* str_standardized(char* str) {
	int i, length;

	str_trim(str);
	str_trim_middle(str);

	length = str_length(str);
  for (i = 1; i < length; i++) {
		if (str[i - 1] == '.' || str[i - 1] == '!' || str[i - 1] == '?') {
			str[i] = toupper(str[i]);
			continue;
		}

		str[i] = tolower(str[i]);
	}
  if (length > 0) {
		str[0] = toupper(str[0]);
	}
  return str;
}

static void __exit exit_main_module(void) {
  printk(KERN_INFO "Module Exit!\n");
}

module_init(init_main_module);
module_exit(exit_main_module);

MODULE_LICENSE("GPL");

