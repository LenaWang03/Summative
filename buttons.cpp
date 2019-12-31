#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>

bool makeButton (Button a, ALLEGRO_EVENT ev, ALLEGRO_FONT *font){
    al_draw_bitmap(a.bitmap, a.x, a.y, 0);
    al_draw_text(font, WHITE, a.x + 120, a.y+20, 0, a.text);
    if(ev.mouse.x >= a.x && ev.mouse.y >= a.y && ev.mouse.x <= a.x + al_get_bitmap_width(a.bitmap) && ev.mouse.y <= a.y +147 && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        a.click = true;
    }
    return a.click;
}

void declareButtons (Button &start) {
    // getting button stuff
    start.bitmap = al_load_bitmap("buttonShort.png");
    start.x = 400;
    start.y = 600;
    start.text = "start";
    start.click = false;
}
