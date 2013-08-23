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


OS_EVENT * msg_key;     //������Ϣ����
OS_EVENT * msg_wav;     //������Ϣ����
OS_EVENT * sem_beep;    //���ֲ����ź���
OS_EVENT * q_msg;       //��Ϣ����

void * msg_grp[256];    //��Ϣ���д洢��ַ,���֧��256����Ϣ


//------------------------------------------------------------------------------
//
static
void task1(void *p_arg)
{
    uint8_t *buf_index;
    uint8_t err;
    
    while(1) {
        //ptr = OSQPend(q_msg, 0, &err); //������Ϣ����
        buf_index = (uint8_t *)OSMboxPend(msg_wav, 0, &err); //������Ϣ����
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
        OSSemPend(sem_beep, 0, &err); //�����ź���
        OSTimeDly(10);
    }
}
//------------------------------------------------------------------------------
//�����շ�����
//ͨ�����䣬�յ�������Ϣ�󣬴��ڷ���һ������
static
void task3(void *p_arg)
{
    uint8_t *key;
    uint8_t err;

    while(1) {
        key = (uint8_t *)OSMboxPend(msg_key, 0, &err); //������Ϣ����
        if(1 == *key) {
            play_wav(); //DAC������8λWAV
            usart_send((uint8_t *)&wav.volume, sizeof(wav.volume));
        }
        OSTimeDly(10);
    }
}
//------------------------------------------------------------------------------
//����ɨ�衢ADC����������������
static
void task4(void *p_arg)
{
    static uint8_t cnt = 0;
    while(1) {
        //����ɨ��
        key_scan();
        if(trg == 1) {
            OSMboxPost(msg_key, (void *)&trg);      //���Ͱ�����Ϣ
            OSSemPost(sem_beep);                    //�ͷ��ź���
            OSQPost(q_msg, "queue");                //���Ͷ���
        }

        //������
        TIM_SetCompare2(TIM3, (++cnt<128)?cnt:(255-cnt));
        
        //ADC����ת��Ϊ����
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

    msg_key = OSMboxCreate((void*)0);       //������Ϣ����
    msg_wav = OSMboxCreate((void*)0);       //����������Ϣ����
    sem_beep = OSSemCreate(0);              //�����ź���
    q_msg = OSQCreate(&msg_grp[0], 256);    //������Ϣ����
    
    OS_ENTER_CRITICAL(); //�����ٽ���

    err = OSTaskCreate(task1, (void *)0,
                       &task1_stk[TASK1_STK_SIZE-1], TASK1_PRIO);      

    err |= OSTaskCreate(task2, (void *)0,
                       &task2_stk[TASK2_STK_SIZE-1], TASK2_PRIO);

    err |= OSTaskCreate(task3, (void *)0,
                       &task3_stk[TASK3_STK_SIZE-1], TASK3_PRIO);

    err |= OSTaskCreate(task4, (void *)0,
                       &task4_stk[TASK4_STK_SIZE-1], TASK4_PRIO);

    OS_EXIT_CRITICAL(); //�˳��ٽ���

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
