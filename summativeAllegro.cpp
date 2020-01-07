#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
// global variables that need to be declared and initialized outside of the function
int counter = 0;
int curr = 0;
int i = 0;

// draws an array of objects to screen
void drawObjects(Object a[]) {
    for (int i = 0; i <a[0].amount; i++) {
        al_draw_scaled_bitmap(a[i].bitmap,0,0, al_get_bitmap_width(a[i].bitmap),al_get_bitmap_height(a[i].bitmap),a[i].x,a[i].y,al_get_bitmap_width(a[i].bitmap)/3,al_get_bitmap_height(a[i].bitmap)/3, 0);
    }
}

// draws one object to screen
void drawObject(Object a) {
    al_draw_scaled_bitmap(a.bitmap,0,0, al_get_bitmap_width(a.bitmap),al_get_bitmap_height(a.bitmap),a.x,a.y,al_get_bitmap_width(a.bitmap)/3.5,al_get_bitmap_height(a.bitmap)/3.5, 0);
}

// draws everything to the screen at the start of a game
void drawBG(LevelBG a, Object &l, Item le, ALLEGRO_FONT *f, int level) {
    char buffer[10];
    al_draw_scaled_bitmap(a.background.bitmap,0,0, 620,502,0,0,1240,1004, 0);
    drawObject(a.door);
    drawObjects(a.chairsF);
    drawObjects(a.desks);
    drawLives(l);
    al_draw_bitmap(a.potion.bitmap, 20, 910,0);
    sprintf(buffer, ": %d", a.potion.totalAmount);
    al_draw_text(f, WHITE, 70, 900, 0, buffer);
    al_draw_scaled_bitmap(le.bitmap,0,0, 704,480,140,920,352,240, 0);
    sprintf(buffer, "LEVEL: %d", level+1);
    al_draw_text(f, WHITE, 750, 890, 0, buffer);
}

// switches between frames according to a timer when character is moving
int playerAnimation (Character a) {
    int frame = 0;
    int flags = 0;
    counter++;
    if (counter % 20 == 19) {
        curr = curr+1;
    }
    if (a.mRight == 2) {
        frame = curr % 2 +1;
    } else if (a.mLeft == -2) {
        frame = curr % 2 +1;
        flags = ALLEGRO_FLIP_HORIZONTAL;
    } else if (a.mDown == 2 || a.mUp == -2) {
        frame = (curr % 2)+3;
    }
    al_draw_bitmap(a.frame[frame], a.posx, a.posy, flags);
    return 0;
}

int enemyAnimation (Object a) {
    int frame = 0;
    counter ++;
    if (counter % 30 == 29) {
        curr ++;
    }
    frame = curr % 2;
    //al_draw_scaled_bitmap(a.frame[1],0,0, al_get_bitmap_width(a.frame[1]),al_get_bitmap_height(a.frame[1]),a.x,a.y,al_get_bitmap_width(a.frame[1])/3,al_get_bitmap_height(a.frame[1])/3, 0);
    al_draw_scaled_bitmap(a.frame[frame],0,0, al_get_bitmap_width(a.frame[frame]),al_get_bitmap_height(a.frame[frame]),a.x,a.y,al_get_bitmap_width(a.frame[frame])/3,al_get_bitmap_height(a.frame[frame])/3, 0);
    return frame;
}

void isHit(Character &a, LevelBG b, int hitCounter, Object &l, Item le, ALLEGRO_FONT *f, int level) {
    if (hitCounter < 100) {
        i++;
        if (i > 20 && i < 40) {
            drawBG(b, l, le, f, level);
            for (int x = 0; x < b.enemy[0].amount; x++) {
                enemyAnimation(b.enemy[x]);
            }
        } else if (i > 60 && i < 80) {
            drawBG(b, l, le, f, level);
            for (int x = 0; x < b.enemy[0].amount; x++) {
                enemyAnimation(b.enemy[x]);
            }
        } else if (i > 100 && i < 120) {
            drawBG(b, l, le, f, level);
            for (int x = 0; x < b.enemy[0].amount; x++) {
                enemyAnimation(b.enemy[x]);
            }
        } else if (i > 140 && i < 160) {
            drawBG(b, l,le, f, level);
            for (int x = 0; x < b.enemy[0].amount; x++) {
                enemyAnimation(b.enemy[x]);
            }
        } else if (i > 160 && i < 180) {
            drawBG(b, l, le, f, level);
            for (int x = 0; x < b.enemy[0].amount; x++) {
                enemyAnimation(b.enemy[x]);
            }
        } else if (i > 200 && i < 220) {
            drawBG(b, l, le, f, level);
            for (int x = 0; x < b.enemy[0].amount; x++) {
                enemyAnimation(b.enemy[x]);
            }
        }
    }
    if (hitCounter == 2) {
        l.amount--;
        i = 0;
    }
}

void loadBitmaps (ALLEGRO_BITMAP *looseBitmaps[]){
    looseBitmaps[0] = al_load_bitmap("startCard.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[1] = al_load_bitmap("menuCard.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[2] = al_load_bitmap("completeCard.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[3] = al_load_bitmap("reminderCard.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[4] = al_load_bitmap("gameOverCard.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[5] = al_load_bitmap("levelCard.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[6] = al_load_bitmap("letter2.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
    looseBitmaps[7] = al_load_bitmap("black.png");
    if (!looseBitmaps[0]) {
        printf("Error: start card bitmap couldn't load");
    }
}

void drawCard (int cardNum, ALLEGRO_BITMAP *card[]){
    al_draw_tinted_bitmap(card[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
    al_draw_bitmap(card[cardNum], 150, 100, 0);
}


