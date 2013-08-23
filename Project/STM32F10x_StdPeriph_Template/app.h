#ifndef APP_H
#define APP_H
#include "public.h"
#include "ucos_ii.h"


extern OS_EVENT * msg_key;     //按键消息邮箱
extern OS_EVENT * msg_wav;     //播放消息邮箱
extern OS_EVENT * sem_beep;    //音乐播放信号量
extern OS_EVENT * q_msg;       //消息队列


void app_init(void);


#endif //~APP_H
