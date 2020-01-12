#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/*********************INITIALIZING BUTTONS*************************/
void setButtons (Button b[], Button levels[]) {
    // sets the sound effect for all buttons
    b[0].sound = al_load_sample("start.ogg");
    for (int x = 1; x < 13; x++){
         b[x].sound = al_load_sample("click.wav");
    }
    b[0].bitmap = al_load_bitmap("buttonShort.png"); // image
    b[0].x = 500; // x position
    b[0].y = 550; // y position
    b[0].text = "START"; // text on button
    b[0].click = false; // if the button is clicked or not

    b[1].bitmap = al_load_bitmap("menu.png");
    b[1].x = 1100;
    b[1].y = 900;
    b[1].click = false;

    b[2].bitmap = al_load_bitmap("buttonLong.png");
    b[2].x = 450;
    b[2].y = 600;
    b[2].text = "EXIT GAME";
    b[2].click = false;

    b[3].bitmap = al_load_bitmap("buttonShort.png");
    b[3].x = 520;
    b[3].y = 600;
    b[3].text = "RESUME";
    b[3].click = false;

    b[4].bitmap = al_load_bitmap("buttonLong.png");
    b[4].x = 450;
    b[4].y = 600;
    b[4].text = "NEXT LEVEL";
    b[4].click = false;

    b[5].bitmap = al_load_bitmap("buttonLong.png");
    b[5].x = 450;
    b[5].y = 650;
    b[5].text = "SELECT ROOM";
    b[5].click = false;

    levels[0].x = 370;
    levels[0].y = 370;
    levels[0].text = ("1");
    levels[1].x = 595;
    levels[1].y = 370;
    levels[1].text = ("2");
    levels[2].x = 820;
    levels[2].y = 370;
    levels[2].text = ("3");
    levels[3].x = 370;
    levels[3].y = 495;
    levels[3].text = ("4");
    levels[4].x = 595;
    levels[4].y = 495;
    levels[4].text = ("5");
    levels[5].x = 820;
    levels[5].y = 495;
    levels[5].text = ("6");
    levels[6].x = 370;
    levels[6].y = 620;
    levels[6].text = ("7");
    levels[7].x = 595;
    levels[7].y = 620;
    levels[7].text = ("8");
    levels[8].x = 820;
    levels[8].y = 620;
    levels[8].text = ("9");
    for (int i = 0; i < 9; i++){
            levels[i].bitmap = al_load_bitmap("levelButton.png");
            levels[i].click = false;
            levels[i].sound = al_load_sample("click.wav");
    }

    b[6].bitmap = al_load_bitmap("nextButton.png");
    b[6].x = 850;
    b[6].y = 800;
    b[6].click = false;

    b[7].bitmap = al_load_bitmap("lastButton.png");
    b[7].x = 320;
    b[7].y = 800;
    b[7].click = false;

    b[8].bitmap = al_load_bitmap("buttonShort.png");
    b[8].x = 520;
    b[8].y = 800;
    b[8].text = "BACK";
    b[8].click = false;

    b[9].bitmap = al_load_bitmap("letter.png");
    b[9].x = 140;
    b[9].y = 920;
    b[9].click = false;

    b[10].bitmap = al_load_bitmap("buttonShort.png");
    b[10].x = 520;
    b[10].y = 800;
    b[10].text = "FINISH";
    b[10].click = false;

    b[11].bitmap = al_load_bitmap("letter2.png");
    b[11].x = 470;
    b[11].y = 405;
    b[11].click = false;

    b[12].bitmap = al_load_bitmap("potion2.png");
    b[12].x = 20;
    b[12].y = 910;
    b[12].click = false;
}

/************************MAKE BUTTON***************************/
bool makeButton (Button a, ALLEGRO_EVENT ev, ALLEGRO_FONT *fontPixel){
    // draws button to screen
    al_draw_scaled_bitmap(a.bitmap,0,0, al_get_bitmap_width(a.bitmap),al_get_bitmap_height(a.bitmap),a.x,a.y,al_get_bitmap_width(a.bitmap)/2,al_get_bitmap_height(a.bitmap)/2, 0);
    al_draw_text(fontPixel, WHITE, a.x + 25, a.y-10, 0, a.text);
    // detecting if it is clicked or not
    if(ev.mouse.x >= a.x && ev.mouse.y >= a.y && ev.mouse.x <= a.x + al_get_bitmap_width(a.bitmap)/2 && ev.mouse.y <= a.y +al_get_bitmap_height(a.bitmap)/2 && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        a.click = true;
        al_play_sample(a.sound, VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    }
    //seeing if there was an error in loading the sound or bitmaps
    if (!a.bitmap){
        printf("Error: %s button couldn't load", a.text);
    }
    if (!a.sound){
        printf("Button sound didn't work! %d\n", a.y);
    }
    return a.click;
}

/*************************CHANGES THE PHASE*******************************/
void switchPhase(Button &b, int &p, ALLEGRO_EVENT ev, ALLEGRO_FONT *fp, int change){
    // creates the button
    if(makeButton(b, ev, fp) == true) {
        //plays the sound
        al_play_sample(b.sound, VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        //changes the phase
        p = change;
    }
}
