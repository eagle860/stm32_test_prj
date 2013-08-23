#ifndef MUSIC_H
#define MUSIC_H
#include "public.h"


typedef struct {
    uint8_t beat;
    const uint16_t *file;
    uint16_t len;
} midi_info;


extern midi_info xyz;


void play_midi(midi_info *midi);


#endif //~MUSIC_H
