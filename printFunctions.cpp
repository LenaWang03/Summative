#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <stdlib.h>

void printCard (ALLEGRO_FONT *fp, int p, int po) {
    char buffer[2] = "";
    switch (p) {
    case 2:
        al_draw_text(fp, WHITE, 350, 350, 0, "YOUR LIFE'S WORK IS ");
        al_draw_text(fp, WHITE, 350, 400, 0, "COMPLETE.  ALL ELEMENTS");
        al_draw_text(fp, WHITE, 350, 450, 0, "OF THE ELIXIR OF LIFE ");
        al_draw_text(fp, WHITE, 350, 500, 0, "HAS BEEN COLLECTED.");
        break;
    case 3:
        al_draw_text(fp, WHITE, 350, 350, 0, "YOU HAVE SUCCESSFULLY ");
        al_draw_text(fp, WHITE, 350, 400, 0, "COMPLETED THIS ROOM.");
        sprintf(buffer, "YOU HAVE COLLECTED %d/9", po);
        al_draw_text(fp, WHITE, 350, 450, 0, buffer);
        al_draw_text(fp, WHITE, 350, 500, 0, "POTIONS SO FAR!");
        break;
    case 4:
        al_draw_text(fp, WHITE, 350, 350, 0, "YOU DIDN'T COMPLETE THE");
        al_draw_text(fp, WHITE, 530, 400, 0, "QUEST");
        al_draw_text(fp, WHITE, 330, 450, 0, "PRESS EXIT TO RETURN TO");
        al_draw_text(fp, WHITE, 500, 500, 0, "THE START");
        break;
    case 5:
        al_draw_text(fp, BROWN, 490, 350, 0, "GAME PAUSED");
        break;
    case 6:
        al_draw_text(fp, WHITE, 300, 400, 0, "REMEMBER TO GET ALL ITEMS");
        al_draw_text(fp, WHITE, 320, 450, 0, "TO COMPLETE YOUR QUEST");
        al_draw_text(fp, WHITE, 320, 500, 0, "FOR THE ELIXIR OF LIFE");
        break;
    case 8:
        al_draw_text(fp, WHITE, 170, 200, 0, "THIS WILL APPEAR IN YOUR INVENTORY");
        al_draw_text(fp, WHITE, 140, 250, 0, "CLICK ON IT IF YOU WANT TO READ IT AGAIN");
        break;
    }
}

void flipPages (int p, char l[][120], ALLEGRO_FONT *f, ALLEGRO_FONT *fp, Item le) {
    char b[200] = "";
            al_draw_bitmap(le.bitmap3, 220, 340, 0);
            al_draw_bitmap(le.bitmap3, 260, 270, 0);
            al_draw_bitmap(le.bitmap3, 300, 300, 0);
    switch (p) {
    case 0:
        for (int i = 0; i <6; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
            al_draw_text(f, BLACK, 380, 370+(i*50), 0, b);
        }
        break;
    case 1:
        al_draw_bitmap(le.bitmap3, 220, 340, 0);
        for (int i = 6; i <12; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
            al_draw_text(f, BLACK, 280, 260+((i-3)*50), 0, b);
        }
        break;
    case 2:
        al_draw_bitmap(le.bitmap3, 260, 270, 0);
        for (int i = 12; i <18; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
            al_draw_text(f, BLACK, 300, 40+((i-6)*50), 0, b);
        }
    }
    sprintf(b, "%d/3", p%3+1);
    al_draw_text(fp, BLACK, 590, 230, 0, b);
}
