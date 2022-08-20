#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>

#define BUF_SIZE 1024

typedef enum hash
{
    MD5 = 0,
    SHA1,
    SHA256
} hash_t;

typedef struct userspace
{
    uint8_t plaintext[BUF_SIZE];
    hash_t tfm;
} userspace_t;

struct user
{
    char username[10];
    char password[32];
} * pUser;

int main(void)
{
    int dev, ret;
    char digest[BUF_SIZE];
    dev = open("/dev/hashkm", O_RDWR);
    if (dev < 0)
    {
        perror("Failed to open the device");
        return errno;
    }

    FILE *fp;
    char uName[10], pwd[32];
    int i;
    char c;

    pUser = (struct user *)malloc(sizeof(struct user));

    while (1)
    {
        printf("1. Create New account\n2. Login Through An Existing Account\n3. Exit\n");
        scanf("%d", &i);
        switch (i)
        {
        case 1:
            do
            {
                if ((fp = fopen("user.dat", "a+")) == NULL)
                {
                    if ((fp = fopen("user.dat", "w+")) == NULL)
                    {
                        puts("Could not open file\n");
                        exit(1);
                    }
                }

                getchar();
                fputs("Ssername: ", stdout);
                if (fgets(pUser->username, sizeof(pUser->username), stdin) == NULL)
                {
                    puts("Error while reading from stdin");
                    return -1;
                }
                char plaintext[BUF_SIZE];
                fputs("Password: ", stdout);
                if (fgets(plaintext, sizeof(plaintext), stdin) == NULL)
                {
                    puts("Error while reading from stdin");
                    return -1;
                }

                char type[8];
                fputs("Choose a hashing algorith(MD5,SHA1,SHA256): ", stdout);
                if (fgets(type, sizeof(type), stdin) == NULL)
                {
                    puts("Error while reading from stdin");
                    return -1;
                }

                strtok(plaintext, "\n");
                strtok(type, "\n");
                for (char *p = type; *p; ++p)
                    *p = toupper((unsigned char)*p);

                userspace_t u;
                strncpy((char *)u.plaintext, plaintext, BUF_SIZE);
                if (!strcmp(type, "MD5"))
                    u.tfm = MD5;
                else if (!strcmp(type, "SHA1"))
                    u.tfm = SHA1;
                else if (!strcmp(type, "SHA256"))
                    u.tfm = SHA256;
                else
                {
                    puts("Algorithm not supported");
                    return -1;
                }
                ret = write(dev, &u, sizeof(userspace_t));
                if (ret < 0)
                {
                    perror("Error while sending data to kernel space");
                    return errno;
                }

                ret = read(dev, digest, BUF_SIZE);
                if (ret < 0)
                {
                    perror("Error while reading data from kernel space");
                    return errno;
                }

                size_t bytes_to_print = (u.tfm == SHA256) ? 32 : (u.tfm == SHA1 ? 20 : 16);
                printf("Original string: \"%s\", %s digest: \"", u.plaintext, type);
                for (size_t i = 0; i < bytes_to_print; i++)
                {
                    printf("%02x", (unsigned char)digest[i]);
                    sprintf(&pUser->password[2 * i], "%02x", (unsigned char)digest[i]);
                }
                puts("\"");

                fwrite(pUser, sizeof(struct user), 1, fp);
                printf("Add another account? (Y/N): ");
                scanf(" %c", &c);
            } while (c == 'Y' || c == 'y');
            break;
        case 2:
            if ((fp = fopen("user.dat", "r+")) == NULL)
            {
                if ((fp = fopen("user.dat", "w+")) == NULL)
                {
                    printf("Could not open file\n");
                    exit(1);
                }
            }

            getchar();
            fputs("Username: ", stdout);
            if (fgets(uName, sizeof(uName), stdin) == NULL)
            {
                puts("Error while reading from stdin");
                return -1;
            }

            char plaintext[BUF_SIZE];
            fputs("Password: ", stdout);
            if (fgets(plaintext, sizeof(plaintext), stdin) == NULL)
            {
                puts("Error while reading from stdin");
                return -1;
            }

            char type[8];
            fputs("Choose a hashing algorith(MD5,SHA1,SHA256): ", stdout);
            if (fgets(type, sizeof(type), stdin) == NULL)
            {
                puts("Error while reading from stdin");
                return -1;
            }

            strtok(plaintext, "\n");
            strtok(type, "\n");
            for (char *p = type; *p; ++p)
                *p = toupper((unsigned char)*p);

            userspace_t u;
            strncpy((char *)u.plaintext, plaintext, BUF_SIZE);
            if (!strcmp(type, "MD5"))
                u.tfm = MD5;
            else if (!strcmp(type, "SHA1"))
                u.tfm = SHA1;
            else if (!strcmp(type, "SHA256"))
                u.tfm = SHA256;
            else
            {
                puts("Algorithm not supported");
                return -1;
            }

            ret = write(dev, &u, sizeof(userspace_t));
            if (ret < 0)
            {
                perror("Error while sending data to kernel space");
                return errno;
            }

            ret = read(dev, digest, BUF_SIZE);
            if (ret < 0)
            {
                perror("Error while reading data from kernel space");
                return errno;
            }

            size_t bytes_to_print = (u.tfm == SHA256) ? 32 : (u.tfm == SHA1 ? 20 : 16);
            printf("Original string: \"%s\", %s digest: \"", u.plaintext, type);
            for (size_t i = 0; i < bytes_to_print; i++)
            {
                printf("%02x", (unsigned char)digest[i]);
                sprintf(&pwd[2 * i], "%02x", (unsigned char)digest[i]);
            }
            puts("\"");

            while (fread(pUser, sizeof(struct user), 1, fp) == 1)
            {
                printf("\nUser:%s+Password:%s\n", pUser->username, pUser->password);
                if (strcmp(pUser->username, uName) == 0)
                {
                    printf("Match username\n");
                    if (strncmp(pUser->password, pwd, bytes_to_print) == 0)
                    {
                        printf("Match password\n");
                    }
                }
            }
            break;

        case 3:
            exit(1);
            break;
        default:
            printf("Enter Valid option");
            break;
        }
        free(pUser);
        fclose(fp);
    }

    return 0;
}