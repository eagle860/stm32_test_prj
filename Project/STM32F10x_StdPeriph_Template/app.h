#ifndef APP_H
#define APP_H
#include "public.h"
#include "ucos_ii.h"


extern OS_EVENT * msg_key;     //������Ϣ����
extern OS_EVENT * msg_wav;     //������Ϣ����
extern OS_EVENT * sem_beep;    //���ֲ����ź���
extern OS_EVENT * q_msg;       //��Ϣ����


void app_init(void);


#endif //~APP_H
