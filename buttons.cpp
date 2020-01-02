#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>

bool makeButton (Button a, ALLEGRO_EVENT ev, ALLEGRO_FONT *fontPixel){
    al_draw_scaled_bitmap(a.bitmap,0,0, al_get_bitmap_width(a.bitmap),al_get_bitmap_height(a.bitmap),a.x,a.y,al_get_bitmap_width(a.bitmap)/2,al_get_bitmap_height(a.bitmap)/2, 0);
    al_draw_text(fontPixel, WHITE, a.x + 25, a.y+18, 0, a.text);
    if(ev.mouse.x >= a.x && ev.mouse.y >= a.y && ev.mouse.x <= a.x + al_get_bitmap_width(a.bitmap) && ev.mouse.y <= a.y +147 && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        a.click = true;
    }
    return a.click;
}

void declareButtons (Button &start, Button &menu, Button &exitGame, Button &resume, Button &nextLevel) {
    start.bitmap = al_load_bitmap("buttonShort.png");
    start.x = 500;
    start.y = 600;
    start.text = "START";
    start.click = false;

    menu.bitmap = al_load_bitmap("menu.png");
    menu.x = 1100;
    menu.y = 900;
    menu.click = false;

    exitGame.bitmap = al_load_bitmap("buttonLong.png");
    exitGame.x = 450;
    exitGame.y = 400;
    exitGame.text = "EXIT GAME";
    exitGame.click = false;

    resume.bitmap = al_load_bitmap("buttonShort.png");
    resume.x = 500;
    resume.y = 500;
    resume.text = "RESUME";
    resume.click = false;

    nextLevel.bitmap = al_load_bitmap("buttonLong.png");
    nextLevel.x = 450;
    nextLevel.y = 600;
    nextLevel.text = "LEXT LEVEL";
    nextLevel.click = false;
}
