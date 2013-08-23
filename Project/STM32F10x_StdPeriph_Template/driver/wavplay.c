#include "wavplay.h"
#include "tim.h"


WAV_file wav;

uint8_t Check_Ifo(uint8_t* pbuf1,uint8_t* pbuf2)
{
	uint8_t i;
	for(i=0;i<4;i++)if(pbuf1[i]!=pbuf2[i])return 1;//��ͬ
	return 0;//��ͬ
}
uint32_t Get_num(uint8_t* pbuf,uint8_t len)
{
    uint32_t num = 0;
	if(len==2)num=(pbuf[1]<<8)|pbuf[0];
	else if(len==4)num=(pbuf[3]<<24)|(pbuf[2]<<16)|(pbuf[1]<<8)|pbuf[0];
	return num;
}
static uint8_t wav_init(uint8_t* pbuf)//��ʼ������ʾ�ļ���Ϣ
{
	if(Check_Ifo(pbuf,(uint8_t *)"RIFF"))return 1;//RIFF��־����
	wav.wavlen=Get_num(pbuf+4,4);//�ļ����ȣ�����ƫ��4byte
	if(Check_Ifo(pbuf+8,(uint8_t *)"WAVE"))return 2;//WAVE��־����
	if(Check_Ifo(pbuf+12,(uint8_t *)"fmt "))return 3;//fmt��־����
    wav.info_len=Get_num(pbuf+16,4);
	wav.formart=Get_num(pbuf+20,2);//��ʽ���
	wav.CHnum=Get_num(pbuf+22,2);//ͨ����
	wav.SampleRate=Get_num(pbuf+24,4);//������
	wav.speed=Get_num(pbuf+28,4);//��Ƶ��������
	wav.ajust=Get_num(pbuf+32,2);//���ݿ������
	wav.SampleBits=Get_num(pbuf+34,2);//��������λ��
	if(Check_Ifo(pbuf+20+wav.info_len,(uint8_t *)"data"))return 4;//data��־����
	wav.DATAlen=Get_num(pbuf+20+wav.info_len+4,4);//���ݳ���	
	return 0;
}
#include "ff.h"
static FATFS Fatfs;
static FIL fp;
uint8_t wav_buf1[WAV_BUF_LEN];
uint8_t wav_buf2[WAV_BUF_LEN];
bool read_next = false;
uint8_t buf_index = 0;
uint32_t count = 0;
bool play_wav(void)
{
    f_mount(1, &Fatfs);
    f_open(&fp, "1:/rgds.wav", FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
    f_lseek(&fp, 0);
    f_read(&fp, wav_buf1, WAV_BUF_LEN, &count);
//    f_close(&fp);

    if(wav_init(wav_buf1) != 0) return false;
	//���ݲ����ʣ�wav.SampleRate�����ö�ʱ�������ж��н���DAת��
    tim6_init(1000000/wav.SampleRate, 72-1);

	return true;
}
void prepare_data(void)
{
    if(read_next == true) {
        read_next = false;
        if(buf_index == 0) {
            buf_index = 1;
            f_read(&fp, wav_buf2, WAV_BUF_LEN, &count);
            if(count == 0) {
                DMA_Cmd(DMA2_Channel3, DISABLE);
                TIM_Cmd(TIM6, DISABLE);
            }
        }
        else {
            buf_index = 0;
            f_read(&fp, wav_buf1, WAV_BUF_LEN, &count);
            if(count == 0) {
                DMA_Cmd(DMA2_Channel3, DISABLE);
                TIM_Cmd(TIM6, DISABLE);
            }
        }
    }
}
