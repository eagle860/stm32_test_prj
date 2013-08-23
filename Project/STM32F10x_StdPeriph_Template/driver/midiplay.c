#include "midiplay.h"


enum TONE_L{
    L0 = 0,
    L1 = 264,
    L2 = 297,
    L3 = 330,
    L4 = 352,
    L5 = 396,
    L6 = 440,
    L7 = 495
};
enum TONE_M{
    M0 = 0,
    M1 = 528,
    M2 = 594,
    M3 = 660,
    M4 = 704,
    M5 = 792,
    M6 = 880,
    M7 = 990
};
enum TONE_H{
    H0 = 0,
    H1 = 1056,
    H2 = 1188,
    H3 = 1320,
    H4 = 1408,
    H5 = 1584,
    H6 = 1760,
    H7 = 1980
};

//Ð¡Ñà×Ó
const uint16_t xyz_t[] = {M3,M5,H1,M6,M5,M5,M5,M5, M3,M5,M6,H1,M5,M5,M5,M5,
                          H1,H1,H1,H3,H2,H2,H1,H1, H2,H1,M6,H1,M5,M5,M5,M5,
                          M3,M3,M3,M5,M6,M6,M5,M6, H1,H1,H2,M5,M6,M6,M6,M6,
                          M3,M3,M1,M1,M2,M2,M2,M2, M2,M2,M2,M3,M5,M5,M5,M5,
                          H1,H1,M2,M3,M5,M5,M5,M5, M3,M3,M3,M1,M6,M6,M5,M5,
                          M3,M2,M1,M1,M2,M2,M2,M2, M2,M2,M2,M3,M5,M5,M5,M5,
                          H1,H1,H1,H3,H2,H2,H2,H1, H2,H1,M5,M6,H1,H1,H1,H1};

midi_info xyz = {2, xyz_t, sizeof(xyz_t)};


static void play_note(uint8_t beat, uint16_t note)
{
    uint32_t n_us;
    uint32_t n_loop;
    uint32_t i;

    if(note == 0) {
        n_loop = 1000/beat;
        for(i = 0; i < n_loop; i++) {
            delay_us(1000);
        }
    }
    else {
        n_us = (1000000+(note/2))/note;
        n_loop = 500000/beat/n_us; //½ÚÅÄ
        for(i = 0; i < n_loop; i++) {
            SOUND_ON();
            delay_us(n_us);
            SOUND_OFF();
            delay_us(n_us);
        }
    }
}
void play_midi(midi_info *midi)
{
    uint16_t i;
    for(i = 0; i < midi->len; i++) {
        play_note(midi->beat, midi->file[i]);
    }
}
