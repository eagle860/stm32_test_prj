#include "app.h"
#include "driver_conf.h"


/* task priority */
#define STARTUP_TASK_PRIO       4
#define TASK1_PRIO              5
#define TASK2_PRIO              6
#define TASK3_PRIO              7
#define TASK4_PRIO              8

/* task stack size */
#define STARTUP_TASK_STK_SIZE   80
#define TASK1_STK_SIZE          80
#define TASK2_STK_SIZE          80
#define TASK3_STK_SIZE          80
#define TASK4_STK_SIZE          80


static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
static OS_STK task1_stk[TASK1_STK_SIZE];
static OS_STK task2_stk[TASK2_STK_SIZE];
static OS_STK task3_stk[TASK3_STK_SIZE];
static OS_STK task4_stk[TASK4_STK_SIZE];


OS_EVENT * msg_key;     //按键消息邮箱
OS_EVENT * msg_wav;     //播放消息邮箱
OS_EVENT * sem_beep;    //音乐播放信号量
OS_EVENT * q_msg;       //消息队列

void * msg_grp[256];    //消息队列存储地址,最大支持256个消息


//------------------------------------------------------------------------------
//
static
void task1(void *p_arg)
{
    uint8_t *buf_index;
    uint8_t err;
    
    while(1) {
        //ptr = OSQPend(q_msg, 0, &err); //请求消息队列
        buf_index = (uint8_t *)OSMboxPend(msg_wav, 0, &err); //请求消息邮箱
        prepare_data(*buf_index);
        OSTimeDly(500);
    }
}
//------------------------------------------------------------------------------
//
static
void task2(void *p_arg)
{
    uint8_t err;

    while(1) {
        OSSemPend(sem_beep, 0, &err); //请求信号量
        OSTimeDly(10);
    }
}
//------------------------------------------------------------------------------
//串口收发任务
//通过邮箱，收到按键消息后，串口发送一次数据
static
void task3(void *p_arg)
{
    uint8_t *key;
    uint8_t err;

    while(1) {
        key = (uint8_t *)OSMboxPend(msg_key, 0, &err); //请求消息邮箱
        if(1 == *key) {
            play_wav(); //DAC单声道8位WAV
            usart_send((uint8_t *)&wav.volume, sizeof(wav.volume));
        }
        OSTimeDly(10);
    }
}
//------------------------------------------------------------------------------
//按键扫描、ADC采样、呼吸灯任务
static
void task4(void *p_arg)
{
    static uint8_t cnt = 0;
    while(1) {
        //按键扫描
        key_scan();
        if(trg == 1) {
            OSMboxPost(msg_key, (void *)&trg);      //发送按键消息
            OSSemPost(sem_beep);                    //释放信号量
            OSQPost(q_msg, "queue");                //发送队列
        }

        //呼吸灯
        TIM_SetCompare2(TIM3, (++cnt<128)?cnt:(255-cnt));
        
        //ADC采样转换为音量
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        adc_val = ADC_GetConversionValue(ADC1);
        wav.volume = 255 * adc_val / 0xFFF / 26 + 1;

        OSTimeDly(10);
    }
}
//------------------------------------------------------------------------------
static
void startup_task(void *p_arg)
{
    int8_t err;
    OS_CPU_SR  cpu_sr;

    systick_init(OS_TICKS_PER_SEC); /* Initialize the SysTick. */
#if (OS_TASK_STAT_EN > 0)
    OSStatInit(); /* Determine CPU capacity. */
#endif

    msg_key = OSMboxCreate((void*)0);       //创建消息邮箱
    msg_wav = OSMboxCreate((void*)0);       //创建播放消息邮箱
    sem_beep = OSSemCreate(0);              //创建信号量
    q_msg = OSQCreate(&msg_grp[0], 256);    //创建消息队列
    
    OS_ENTER_CRITICAL(); //进入临界区

    err = OSTaskCreate(task1, (void *)0,
                       &task1_stk[TASK1_STK_SIZE-1], TASK1_PRIO);      

    err |= OSTaskCreate(task2, (void *)0,
                       &task2_stk[TASK2_STK_SIZE-1], TASK2_PRIO);

    err |= OSTaskCreate(task3, (void *)0,
                       &task3_stk[TASK3_STK_SIZE-1], TASK3_PRIO);

    err |= OSTaskCreate(task4, (void *)0,
                       &task4_stk[TASK4_STK_SIZE-1], TASK4_PRIO);

    OS_EXIT_CRITICAL(); //退出临界区

    if (OS_ERR_NONE != err)
        while(1);

    OSTaskDel(OS_PRIO_SELF);
}
//------------------------------------------------------------------------------
void app_init(void)
{
    OSInit();
    OSTaskCreate(startup_task,
                 (void *)0,
                 &startup_task_stk[STARTUP_TASK_STK_SIZE - 1],
                 STARTUP_TASK_PRIO);
    OSStart();
}
