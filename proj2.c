#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_NUMBER (100)
#define IOCTL_SET_VALUE _IOW(IOCTL_NUMBER, 1, int)
#define DEVICE_PATH ("/dev/digitModule")

int main(void)
{
    int fd, value;

    fd = open(DEVICE_PATH, O_RDWR);
    if(fd < 0)
    {
        perror("digit module open failure\n");
        return 1;
    }

    printf("Enter number");
    
    for(;;)
    {
        scanf("%d", &value);
        if(value == 4)
        {
            printf("Program exit\n");
            break;
        }

        if (value < 1 || value > 4) {
            printf("Please enter 1~4\n");
            continue;
        }

        value -= 1;

        if(ioctl(fd, IOCTL_SET_VALUE, &value) < 0)
        {
            perror("ioctl failed");
        }
        else
        {
            printf("Sent value %d to kernel\n", value);
        }
        printf("Enter digit ( 1 ~ 4) : ");
    }
    
    close(fd);
    return 0;
}
