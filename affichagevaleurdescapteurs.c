/***********************************************************************
 * Nom: GRILO				Fonction: Enseignant Informatique Indus.
 * Prénom: Christophe		Entreprise: Lycée les Eucalyptus - Nice (06)
 * Date de Création: 15/12/2017		Modification: 23/12/2017
 * ---------------------------------------------------------------------
 * 							TESTFONCTIONS.C
 * ---------------------------------------------------------------------
 * Contient le programme permettant d'afficher les informations 
 * provenant des capteurs de température, d'humidité et de 
 * pression de la SenseHat sur ses leds RGB.
 * ---------------------------------------------------------------------
 * Avant de Compiler faire:
 * sudo raspi-config --> advanced options --> enable i2c
 * sudo apt-get install libi2c-dev i2c-tools
 * Puis compiler par exemple en utilisant:
 * gcc -o testfonction1 MaSenseHat.c affichagevaleurdescapteurs.c fb.c
 * si la librairie sense-master est installée: cd ....libsense-master
 * make
 * sudo make install
 * ********************************************************************/
#include <sense/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "MaSenseHat.h"
// Replace with any of the fonts in "fonts" directory
#include "examples/fonts/5x5_font.h"

//#include "fonts/BMSPA_font.h"
//#define CHAR_WIDTH 8

#define CHAR_WIDTH (sizeof(font) / 96)
#define CHAR_HEIGHT 8
#define SCROLL_DELAY 100
#define LETTER_SPACE 1
#define LETTER_WIDTH (CHAR_WIDTH + LETTER_SPACE)



void draw(char c,int x,int y,sense_color_t color,sense_bitmap_t bitmap);
const unsigned char* character(char c);

int main(int argc,char** argv)
{
	int x;
    
    sense_bitmap_t fb = sense_alloc_fb();
    if (!fb)
    {
        fprintf(stderr,"Could not allocate framebuffer: %s\n",sense_strerror(sense_errno()));
        exit(1);
    }
    sense_color_t color = sense_make_color_rgb(0xC4,0x33,0xFF);
    sense_bitmap_t buffer = sense_alloc_bitmap();
    char word[1000]="Temperature:";
    int idx;
    double temp=temperature();
    double humi=humidite();
    double press=pression();
    
    sprintf(word,"Temperature:%.2fC Pression:%.0fhPa Humidite:%.0f%%",temp,press,humi);
    
    int len = strlen(word);
    for (x=0;x<len*LETTER_WIDTH;++x)
    {
        sense_bitmap_paint(buffer,0);
        idx = x/LETTER_WIDTH;
        draw(word[idx],0 - x % LETTER_WIDTH,0,color,buffer);
        if ((idx + 1) < len)
        {
            draw(word[idx+1],LETTER_WIDTH - x % LETTER_WIDTH,0,color,buffer);
        }
        sense_bitmap_cpy(fb,buffer);
        
        usleep(SCROLL_DELAY*500);
    }
    // clear display
    sense_bitmap_paint(fb,0);
    sense_free_bitmap(fb);
    sense_free_bitmap(buffer);
    return 0;
}

const unsigned char* character(char c){
    c = c & 0x7F;
    if (c < ' ') {
        c = 0;
    } else {
        c -= ' ';
    }
    return font[c];
}

// code apdapted from http://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
void draw(char c,int x,int y,sense_color_t color,sense_bitmap_t bitmap) {
    int i,j;
    const unsigned char* chr = character(c);
    for (j=0 + ((LETTER_SPACE+x)<0?(1+x)*-1:0); j<CHAR_WIDTH && (1+j+x) < SENSE_BITMAP_WIDTH; ++j) {
        for (i=0 + (y<0?y*-1:0); i<CHAR_HEIGHT && (i+y < SENSE_BITMAP_HEIGHT); ++i) {
            if (chr[j] & (1<<i)) {
                sense_bitmap_set_pixel(bitmap,LETTER_SPACE +j+x, i+y,color);
            }
        }
    }
}
