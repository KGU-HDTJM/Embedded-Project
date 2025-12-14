#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define IOCTL_NUMBER (100)
#define IOCTL_SET_VALUE _IOW(IOCTL_NUMBER, 1, int)
#define CHR_DEV_NAME ("digitModule")

#include "../term.h"
#include "../LED/led.h"

enum digit_mode
{
    DIGIT_0, 
    DIGIT_1,
    DIGIT_2,
    DIGIT_3,
    DIGIT_NON
};

enum digit_mode g_digitCtrl = DIGIT_NON;


void digit_ctrl_handler(void);


static long digit_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
    int value;

    switch(cmd){
        case IOCTL_SET_VALUE:
            if(copy_from_user(&value, (int __user*)arg, sizeof(int)))
            {
                return -EFAULT;
            }
            if(value < DIGIT_0 || value > DIGIT_NON) {return -EINVAL;}
            g_digitCtrl = value;
            digit_ctrl_handler();
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

void digit_ctrl_handler(void)
{
    int i;
    enum led_mode led_mode = get_led_mode();

    if(g_digitCtrl == DIGIT_NON)
    {
        return;
    }

    if(led_mode == LED_MODE_MANUAL)
    {
        switch(g_digitCtrl)
        {
            case DIGIT_0:
                printk(KERN_INFO "toggle 0\n");
                toggle_led(0);
                break;
            case DIGIT_1:
                printk(KERN_INFO "toggle 1\n");
                toggle_led(1);    
                break;
            case DIGIT_2:
                printk(KERN_INFO "toggle 2\n");
                toggle_led(2);    
                break;
            case DIGIT_3:
                printk(KERN_INFO "toggle 3\n");
                toggle_led(3);    
                break;
            default: return;
        }
        return;
    }

    switch(g_digitCtrl)
    {
        case DIGIT_0:
            {
		        // blink mode
		        printk(KERN_INFO "switch: LED_MODE_BLINK\n");
		        set_led_mode(LED_MODE_BLINK);
	        }
        break;
        case DIGIT_1:
        	{
		        // sequential mode
		        printk(KERN_INFO "switch: LED_MODE_SEQUENTIAL\n");
		        set_led_mode(LED_MODE_SEQUENTIAL);
	        }
	    break;
        case DIGIT_2:
            {
		        // manual mode
		        printk(KERN_INFO "switch: LED_MODE_MANUAL\n");
		        set_led_mode(LED_MODE_MANUAL);
	        }
            break;
        case DIGIT_3:
            {
		        // reset button
	        	printk(KERN_INFO "switch: LED_MODE_RESET\n");
		        set_led_mode(LED_MODE_RESET);
	        }
	        break;
        default: return;
    }
}

static const struct file_operations digit_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = digit_ioctl,
};

static int digit_module_init(void)
{
    int registration;

    registration = register_chrdev(DEV_MAJOR_NUMBER_START, CHR_DEV_NAME, &digit_fops);
    if(registration < 0)
    {
        printk(KERN_ALERT "digit module registration failed\n");
        return registration;
    }
    led_init();
    return 0;
}

static void digit_module_exit(void)
{
    printk(KERN_INFO "digit module exit\n");
    unregister_chrdev(DEV_MAJOR_NUMBER_START, CHR_DEV_NAME);
    led_exit();
}

module_init(digit_module_init);
module_exit(digit_module_exit);
MODULE_LICENSE("GPL");