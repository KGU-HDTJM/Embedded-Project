#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/delay.h>
#include<linux/timer.h>
#include<linux/fs.h>
#include<linux/gpio.h>
#include<asm/uaccess.h>
#include<asm/io.h>

#define HIGH (1)
#define LOW  (0)

#define LED_TURN_ON_ALL (0)
#define LED_TURN_ON_ONE (1)
#define LED_TURN_OFF(2)

int g_LedFlag = 0;
int g_ManualFlag = 0;

int g_Led[4]	 = {23, 24, 25, 1};
int g_Switch[4]  = {4, 17, 27, 22};
static struct timer_list g_Timer;
 


irqreturn_t irq_handler(int irq, void* dev_id)
{
	printk(KERN_INFO "debug%d\n", irq);

	
	if(g_ManualFlag = 0)
	{
		switch(irq)
		{	
			case 60:
				if(timer_pending(&g_Timer)
				{
					del_timer(&g_Timer);
				}
				timer_setup(&g_Timer, timer_cb, 0);
				g_Timer.expires = jiffies + HZ * 2;
				add_timer(&g_Timer);
			
				g_LedFlag = LED_TURN_ON_ALL;

				break;
			case 61:
				if(timer_pending(&g_Timer);
				{
					del_timer(&g_Timer);
				}
				timer setup(&g_Timer, timer_cb, 0);
				g_Timer.expires = jiffies + HZ * 2;
				add_timer(&g_Timer);

				g_LedFlag = LED_TURN_ON_ONE;

				break;	
			case 62:
				if(timer_pending(&g_Timer);
				{
					del_timer(&g_Timer);
				}
				g_LedFlag = LED_TURN_OFF;
				g_ManualFlag = 1;
			case 63:
				g_LedFlag = LED_TURN_OFF;
				g_ManualFlag = 0;
		}
	}
	else
	{
		int i, ret;
		for(i = 0; i < 4; ++i)
		{
			ret = gpio_direction_output(g_Led[i], LOW);
		}		

		switch(irq)
		{	
			case 60:
				ret = gpio_direction_output(g_Led[0], HIGH);
				break;
			case 61:
				ret = gpio_direction_output(g_Led[1], HIGH);
				break;
			case 62:
				ret = gpio_direction_output(g_Led[2], HIGH);
				break;
			case 63:
				g_ManualFlag = 0;
		}
	}
}

static void timer_cb(struct timer_list* timer)
{
	int i, ret, flag;

	flag = g_LedFlag;	

	if(flag = LED_TURN_ON_ALL)
	{	
		for(i = 0; i < 4; i++)
			ret = gpio_direction_ouput(g_Led[i], HIGH);

		flag = LED_TURN_OFF;
	}
	else if(flag = LED_TURN_ON_ONE)
	{
		static int index = 0;
		ret = gpio_direction_output(g_Led[index % 3], HIGH);
		flag = LED_TURN_OFF;
	}
	else
	{
		for(i = 0; i < 4; ++i)
		{
			ret = gpio_direction_ouput(g_Led[i], LOW);
		}
		flag = g_LedFlag;
	}
	
	timer->expires = jiffies + HZ * 2;
	add_timer(timer);
}

static int switch_1_init(void){
	int ret, res,i;

	printk(KERN_INFO "led_and_switch_module_init\n");
	for(i = 0; i < 4; i++)
	{
		ret = gpio_request(g_Led[i], "LED");
		res = gpio_request(g_Switch[i], "sw");
		res = request_irq(gpio_to_irq(g_Switch[i]), (irq_handler_t)irq_handler, IRQF_TRIGGER_RISING, "IRQ", (void*)(irq_handler));

		if(ret < 0)
			printk(KERN_INFO "led_module gpio_request failed\n");
		if(ret1 < 0)
			printk(KERN_INFO "switch_module gpio_request failed\n");
	}

	for(i = 0; i < 4; i++)
	{
		
	}
}

static void switch_1_exit(void)
{
	int i;
	printk(KERN_INFO "led_module_exit\n");
	for(i = 0; i < 4; ++i)
	{
		gpio_free(g_Led[i]);
		gpio_free(g_Switch[i]);
	}
}


module_init(switch_1_init);
module_exit(switch_1_exit);
MODULE_LICENSE("GPL");
