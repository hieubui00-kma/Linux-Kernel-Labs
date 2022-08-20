#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

char buffer_writer[BUFFER_SIZE];
char buffer_reader[BUFFER_SIZE];
int8_t w_buf[27];
int8_t khv[26];
int k,i,j;
char x,ktt[26];

int main() {
	int fd;
	char choice;
 
	fd = open("/dev/lab6_Bui_Minh_Hieu_dev", O_RDWR);
	if (fd < 0) {
		printf("Cannot open device file!\n");
		return -1;
	}
 
	while (1) {
        printf("*************************************************\n");
		printf("1. Nhap vao mot xau.\n");
		printf("2. Ma hoa va luu xau dung ma dich chuyen.\n");
		printf("3. Ma hoa va luu xau dung ma thay the.\n");
		printf("4. Ma hoa va luu xau dung ma hoan vi.\n");
		printf("5. Doc va giai ma xau ma dung ma dich chuyen.\n");
		printf("6. Doc va giai ma xau ma dung ma dung ma thay the.\n");
		printf("7. Doc va giai ma xau ma dung ma dung ma hoan vi.\n");
		printf("8. Thoat chuong trinh.\n");
		printf("*************************************************\n");
        printf("Nhap lua chon: ");
        scanf(" %c", &choice);

        switch (choice) {  
            case '1':
                *buffer_writer = 1;
				printf("Nhap vao xau: ");
				scanf("%s", buffer_writer + 1);
				write(fd, buffer_writer, strlen(buffer_writer) + 1);
				printf("\n");
                break;

            case '2':
                *buffer_writer = 2;
				printf("Nhap khoa K: ");
				scanf("%s", buffer_writer + 1);
 				write(fd, buffer_writer, strlen(buffer_writer) + 1);
				read(fd, buffer_reader, BUFFER_SIZE);
				printf("Du lieu sau khi duoc ma hoa: %s\n", buffer_reader);
                break;

            case '3': 
                *buffer_writer = 3;
				printf("\nNhap vao day ki tu dung lam ma thay the: ");
				scanf("%s", buffer_writer + 1);
 				write(fd, buffer_writer, strlen(buffer_writer) + 1);
 				read(fd, buffer_reader, BUFFER_SIZE);
				printf("\nDu lieu sau khi duoc ma hoa: %s\n", buffer_reader);
                break;

			case '4':
				*buffer_writer = 4;
				printf("\nNhap vao day so dung lam ma hoan vi: ");
				for(int i = 1; i <= 26; i++){
					scanf(" %hhd", buffer_writer + i);
				}
 				write(fd, buffer_writer, sizeof(int8_t) * 27);
				read(fd, buffer_reader, BUFFER_SIZE);
				printf("\nDu lieu sau khi duoc ma hoa: %s\n", buffer_reader);
				break;

			case '5':
				*buffer_writer = 5;
				write(fd, buffer_writer, strlen(buffer_writer) + 1);
				read(fd, buffer_reader, BUFFER_SIZE);
				printf("\nNhap khoa K de giai ma: ");
				scanf("%ls", &k);
				printf("\nDu lieu sau khi duoc giai ma:");
				i = 0;
				while(buffer_reader[i] != '\0'){
					x = buffer_reader[i++] - 97 + 26 - k;
					x %= 26;
					x += 97;
					printf("%c", x);
				}
				printf("\n");
				break;

			case '6':
				*buffer_writer = 6;
				write(fd, buffer_writer, strlen(buffer_writer)+1);
				read(fd, buffer_reader, 1024);
				printf("Nhap vao day ki tu de giai ma: ");
				scanf(" %s", ktt);
				printf("Du lieu sau khi duoc giai ma");
				i=0;
				while(buffer_reader[i] != '\0'){
					x = buffer_reader[i];
					for(j = 0; j< 26; j++){
						if(ktt[j] == x){
							x = j + 97;
							printf("%c",x);
							break;
						}
					}
					i++;
				}
				printf("\n");
				break;

			case '7':
				*buffer_writer = 7;
				write(fd, buffer_writer, strlen(buffer_writer)+1);
				read(fd, buffer_reader, 1024);
				printf("Nhap vao day so de giai ma: ");
				for(int i = 0; i < 26; i++){
					scanf(" %hhd", &khv[i]);
				}
				printf("Du lieu sau khi duoc giai ma:");
				i = 0;
				while(buffer_reader[i] != '\0'){
					x = buffer_reader[i];
					x -= 96;
					for(j = 0; j < 26; j++){
						if(khv[j] == x){
							x = j + 97;
							printf("%c",x);
							break;
						}
					}
					i++;
				}
				printf("\n");
				break;

            case '8': 
                close(fd); 
                exit(1);
                break;

            default:
                printf("Nhap lai lua cho = %c\n", choice);
                break;    
        }
    }
    printf("End of the program\n");
    return 0;
}



