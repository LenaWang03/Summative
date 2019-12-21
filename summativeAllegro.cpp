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
void drawBG(LevelBG a){
    al_draw_scaled_bitmap(a.background.bitmap,0,0, 620,502,0,0,1240,1004, 0);
    drawObject(a.door);
    drawObjects(a.chairsF);
    drawObjects(a.desks);
    drawObjects(a.enemy);
}

// switches between frames according to a timer when character is moving
int playerAnimation (Character a){
    int frame = 0;
    int flags = 0;
    counter++;
        if (counter % 20 == 19){
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
