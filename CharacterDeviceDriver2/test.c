#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TEXT_LEN 11
#define ERR_NUM 1

int main()
{
    char buff[TEXT_LEN];
    int fd;
     
    if ((fd = open("/dev/chardev0", O_RDWR)) < 0){
        printf("Failed to open\n");
        return ERR_NUM;
    }
 
    if (write(fd, "HELLOWORLD", TEXT_LEN) < 0){
        printf("Failed to write\n");
        close(fd);
        return ERR_NUM;
    }
 
    if (read(fd, buff, TEXT_LEN) < 0){
        printf("Failed to read\n");
        close(fd);
        return ERR_NUM;
    }else{
        printf("%s\n", buff);
    }
 
    if (close(fd) != 0){
        printf("Failed to close\n");
    }
    
    return 0;
}