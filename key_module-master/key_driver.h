/**
 * @file key_driver.h
 * @author Jason Wen (embediot@163.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-3
 * 
 * @copyright EmbedIoT Studio Copyright (c) 2024
 * 
 */
#ifndef _KEY_DRIVER_H_
#define _KEY_DRIVER_H_

//按键ID编号，如4按键，则编号0~3
//通常用这个ID编号，在用户层与驱动层进行按键绑定
typedef enum
{
    KEY_ID_0 = 0,
    KEY_ID_1,
    KEY_ID_2,
    KEY_ID_3,

    KEY_ID_MAX,
}key_id_t;

typedef struct key_driver
{
    unsigned char detect_level;    //按下时的电平状态，0-低电平，1-高电平

    void (*init)(void);
    bool (*read_pin_state)(key_id_t key_id);    //读取按键引脚状态
}key_driver_t;

extern key_driver_t key_driver;

#endif

