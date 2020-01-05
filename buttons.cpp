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
    if(ev.mouse.x >= a.x && ev.mouse.y >= a.y && ev.mouse.x <= a.x + al_get_bitmap_width(a.bitmap)/2 && ev.mouse.y <= a.y +147 && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        a.click = true;
    }
    return a.click;
}

void declareButtons (Button &start, Button &menu, Button &exitGame, Button &resume, Button &nextLevel, Button &levelSelect, Button levels[], Button &next, Button &last, Button &goBack, Button &letter, Button &finish) {
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
    exitGame.y = 600;
    exitGame.text = "EXIT GAME";
    exitGame.click = false;

    resume.bitmap = al_load_bitmap("buttonShort.png");
    resume.x = 520;
    resume.y = 600;
    resume.text = "RESUME";
    resume.click = false;

    nextLevel.bitmap = al_load_bitmap("buttonLong.png");
    nextLevel.x = 450;
    nextLevel.y = 600;
    nextLevel.text = "LEXT LEVEL";
    nextLevel.click = false;

    levelSelect.bitmap = al_load_bitmap("buttonLong.png");
    levelSelect.x = 450;
    levelSelect.y = 700;
    levelSelect.text = "QUICK PLAY";
    levelSelect.click = false;
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
    }

    next.bitmap = al_load_bitmap("nextButton.png");
    next.x = 850;
    next.y = 800;
    next.click = false;

    last.bitmap = al_load_bitmap("lastButton.png");
    last.x = 320;
    last.y = 800;
    last.click = false;

    goBack.bitmap = al_load_bitmap("buttonShort.png");
    goBack.x = 520;
    goBack.y = 800;
    goBack.text = "BACK";
    goBack.click = false;

    letter.bitmap = al_load_bitmap("letter.png");
    letter.x = 140;
    letter.y = 920;
    letter.click = false;

    finish.bitmap = al_load_bitmap("buttonShort.png");
    finish.x = 520;
    finish.y = 800;
    finish.text = "FINISH";
    finish.click = false;
}
