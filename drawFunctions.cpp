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

// global variables that need to be declared and initialized outside of the function
int curr = 0;
int i = 0;

/***********************************************DRAW OBJECTS***********************************************/
void drawObjects(Object a[]) {
    for (int i = 0; i <a[0].amount; i++) {
        al_draw_scaled_bitmap(a[i].bitmap,0,0, al_get_bitmap_width(a[i].bitmap),al_get_bitmap_height(a[i].bitmap),a[i].x,a[i].y,al_get_bitmap_width(a[i].bitmap)/3,al_get_bitmap_height(a[i].bitmap)/3, 0);
    }
}

/***********************************************DRAW OBJECT***********************************************/
void drawObject(Object a) {
    al_draw_scaled_bitmap(a.bitmap,0,0, al_get_bitmap_width(a.bitmap),al_get_bitmap_height(a.bitmap),a.x,a.y,al_get_bitmap_width(a.bitmap)/3.5,al_get_bitmap_height(a.bitmap)/3.5, 0);
}

/***********************************************DRAW BACKGROUND***********************************************/
void drawBG(LevelBG a, Object &l, ALLEGRO_FONT *f, int level, Item le) {
    // draws everything besides the moving parts
    char buffer[10];
    // draws background bitmap
    al_draw_scaled_bitmap(a.background.bitmap,0,0, 620,502,0,0,1240,1004, 0);
    // draws door
    drawObject(a.door);
    // draws chair
    drawObjects(a.chairsF);
    // desk
    drawObjects(a.desks);
    // draws hearts
    drawLives(l);
    // draws letter icon
    al_draw_scaled_bitmap(le.bitmap,0,0, 704,480,140,920,352,240, 0);
    // draws potion icon
    al_draw_bitmap(a.potion.bitmap, 20, 910,0);
    // draws text
    sprintf(buffer, ": %d", a.potion.totalAmount);
    al_draw_text(f, WHITE, 70, 900, 0, buffer);
    sprintf(buffer, "LEVEL: %d", level+1);
    al_draw_text(f, WHITE, 750, 890, 0, buffer);
}

// draws in the hearts and updates them whenever the player loses a life
void drawLives(Object a) {
    for(int i = 0; i < 3; i++) {
        al_draw_scaled_bitmap(a.frame[0],0,0, 224,192,450+(i*80),900,74,64, 0);
    }
    for(int i = 0; i < a.amount; i++) {
        al_draw_scaled_bitmap(a.bitmap,0,0, 224,192,450+(i*80),900,74,64, 0);
    }
}

// switches between frames according to a timer when character is moving
int playerAnimation (Character a, int counter) {
    int frame = 0;
    int flags = 0;
    // for every 19 frames it changes the player frame
    if (counter % 20 == 19) {
        curr ++;
    }
    // decides the frame
    if (a.mRight >0) {
        frame = curr % 2 +1;
    } else if (a.mLeft < 0) {
        frame = curr % 2 +1;
        flags = ALLEGRO_FLIP_HORIZONTAL;
    } else if (a.mDown >0 || a.mUp < 0) {
        frame = (curr % 2)+3;
    }
    //draws the frame
    al_draw_bitmap(a.frame[frame], a.posx, a.posy, flags);
    return 0;
}

// animates the enemy
int enemyAnimation (Object a, int counter) {
    int frame = 0;
    // changes enemy frame every 29 frames
    if (counter % 30 == 29) {
        curr ++;
    }
    frame = curr % 2;
    // draws the frame
    al_draw_scaled_bitmap(a.frame[frame],0,0, al_get_bitmap_width(a.frame[frame]),al_get_bitmap_height(a.frame[frame]),a.x,a.y,al_get_bitmap_width(a.frame[frame])/3,al_get_bitmap_height(a.frame[frame])/3, 0);
    return frame;
}

// loads loose bitmaps
void loadBitmaps (ALLEGRO_BITMAP *looseBitmaps[]) {
    looseBitmaps[0] = al_load_bitmap("startCard.png");
    //gives error message if it can't find the bitmap
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[1] = al_load_bitmap("menuCard.png");
    if (!looseBitmaps[1]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[2] = al_load_bitmap("completeCard.png");
    if (!looseBitmaps[2]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[3] = al_load_bitmap("reminderCard.png");
    if (!looseBitmaps[3]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[4] = al_load_bitmap("gameOverCard.png");
    if (!looseBitmaps[4]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[5] = al_load_bitmap("levelCard.png");
    if (!looseBitmaps[5]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[6] = al_load_bitmap("letter2.png");
    if (!looseBitmaps[6]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[7] = al_load_bitmap("black.png");
    if (!looseBitmaps[7]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[8] = al_load_bitmap("openLetter.png");
    if (!looseBitmaps[8]) {
        printf("Error: start card bitmap couldn't load");
    }
}

// draws the background for the screens and menus
void drawCard (int cardNum, ALLEGRO_BITMAP *card[]) {
    // this makes the background fade to black
    al_draw_tinted_bitmap(card[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
    // this draws which menu card it will print
    if (cardNum == 8) {
        al_draw_bitmap(card[cardNum], 270, 50, 0);
    } else {
        al_draw_bitmap(card[cardNum], 150, 100, 0);
    }
}

// animates what happens when the character is hit
void isHit(Character &a, LevelBG b, int hitCounter, Object &l, ALLEGRO_FONT *f, int level, int counter, Item le) {
    // subtracts a life
    if (hitCounter == 2) {
        al_play_sample(b.enemy[0].sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        l.amount--;
        i = 0;
    }
    // makes the player flash
    if (hitCounter < 200) {
        i++;
        for (int j = 0; j <20; j++) {
            if (i > (j*40)+20 && i < (j*40)+40) {
                drawBG(b, l, f, level, le);
                for (int x = 0; x < b.enemy[0].amount; x++) {
                    enemyAnimation(b.enemy[x], counter);
                }
            }
        }
    }
}

/*void loadSound(ALLEGRO_SAMPLE* s[]){
    s[0]= al_load_sample("flip.wav");
    s[1] = al_load_sample("over.wav");
    s[2] = al_load_sample("gameMusic.ogg");
    s[3] = al_load_sample("win.wav");
    s[4] = al_load_sample("levelWin.wav");
}*/



