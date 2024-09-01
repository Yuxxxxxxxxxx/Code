#include <rtthread.h>
#include "stm32f10x.h"
#include "key_module.h"

key_manager_t key_manager;    //定义一个按键管理器对象

static void on_press_handler(key_t *key)
{
	static unsigned int press_count = 0;
	press_count++;
}

static void on_release_handler(key_t *key)
{
	static unsigned int release_count = 0;
	release_count++;
}

static void on_single_click_handler(key_t *key)
{
	static unsigned int single_click = 0;
	single_click++;
}

static void on_double_click_handler(key_t *key)
{
	static unsigned int double_click = 0;
	double_click++;
}

static void on_long_press_handler(key_t *key)
{
	static unsigned int long_press = 0;
	long_press++;
}

static void key_module_thread(void *parameter)
{
	key_module_init(&key_manager);    //初始化按键管理器

	//设置各种按键状态的回调函数
	key_module_set_event_handler(&key_manager, KEY_PRESSED, on_press_handler);
	key_module_set_event_handler(&key_manager, KEY_RELEASED, on_release_handler);
	key_module_set_event_handler(&key_manager, KEY_SINGLE_CLICK, on_single_click_handler);
	key_module_set_event_handler(&key_manager, KEY_DOUBLE_CLICK, on_double_click_handler);
	key_module_set_event_handler(&key_manager, KEY_LONG_PRESS, on_long_press_handler);

	while(1){
		key_module_ticks_update(&key_manager);
		key_module_update(&key_manager);
		rt_thread_delay(1);
	}
}

int main(void)
{
	rt_thread_t key_thread = RT_NULL;

	key_thread = rt_thread_create("key_thread",key_module_thread,
								RT_NULL, 512, 20, 20);
	
	if(RT_NULL != key_thread)rt_thread_startup(key_thread);
	else return -1;

	while(1){
		rt_thread_delay(100);
	}

	return 0;
}
