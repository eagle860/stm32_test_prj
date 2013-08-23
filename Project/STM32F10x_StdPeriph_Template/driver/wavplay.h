#ifndef __WAV_H
#define __WAV_H	
#include "public.h"


#define WAV_BUF_LEN     512

typedef struct 
{
    uint8_t volume;
	u8 chRIFF[4];
	u32 wavlen;
	u8 chWAV[4];
	u8 chFMT[4];
    uint32_t info_len;
	u16 formart;
	u16 CHnum;
	u32 SampleRate;
	u32 speed;
	u16 ajust;
	u16 SampleBits;
	u8 chDATA[4];
	u32 DATAlen;
} WAV_file;


extern WAV_file wav;
extern uint8_t wav_buf1[WAV_BUF_LEN];
extern uint8_t wav_buf2[WAV_BUF_LEN];


bool play_wav(void);
void prepare_data(uint8_t buf_num);

#endif
