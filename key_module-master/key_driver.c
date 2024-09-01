/**
 * @file key_driver.c
 * @author Jason Wen (embediot@163.com)
 * @brief 按键模块的驱动层，用来适配不同的单片机引脚
 * @version 0.1
 * @date 2024-06-3
 * 
 * @copyright EmbedIoT Studio Copyright (c) 2024
 * 
 */
#include "stm32f10x.h"
#include "key_driver.h"


#define DETECT_LEVEL        0     //按键按下时的电平状态，0-低电平，1-高电平

//按键ID与引脚绑定结构体
typedef struct key_pinmap
{
    key_id_t key_id;
    GPIO_Module *port;
    unsigned short pin;
}key_pinmap_t;

key_pinmap_t key_pinmap[KEY_ID_MAX] = 
{
    {.key_id = KEY_ID_0, .port = GPIOC, .pin = GPIO_PIN_13},    //K1
    {.key_id = KEY_ID_1, .port = GPIOC, .pin = GPIO_PIN_14},    //K2
    {.key_id = KEY_ID_2, .port = GPIOC, .pin = GPIO_PIN_15},    //K3
    {.key_id = KEY_ID_3, .port = GPIOC, .pin = GPIO_PIN_3},     //K4
};

static void _init(void)
{
    //在这里初始化GPIO引脚
    GPIO_InitType GPIO_InitStructure;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_GPIOD ,ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO,ENABLE);
	GPIO_InitStruct(&GPIO_InitStructure);

	GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE, ENABLE);

    //K1-PC13, K2-PC14, K3-PC15, K4-PC3
    GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

}

static bool _read_pin_state(key_id_t key_id)
{
    //在这里返回按键引脚电平
    int i = 0;
    unsigned char state;

    for(i = 0;i < KEY_ID_MAX;i++){
        if(key_id == key_pinmap[i].key_id)
        state = GPIO_ReadInputDataBit(key_pinmap[i].port, key_pinmap[i].pin);
    }

    if(DETECT_LEVEL)return ((state == Bit_SET) ? true : false);
    else return ((state == Bit_SET) ? false : true);
}


key_driver_t key_driver = 
{
    .detect_level = DETECT_LEVEL,
    .init = _init,
    .read_pin_state = _read_pin_state,
};



