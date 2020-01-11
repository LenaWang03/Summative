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
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

void printCard (ALLEGRO_FONT *fp, int p, int po, Character pl) {
    char buffer[2] = "";
    switch (p) {
    case 1:
        if (pl.posx ==1070 && pl.posy == 680 && po == 0){
            al_draw_text(fp, WHITE, 50, 200, 0, "USE ARROW KEYS OR WASD KEYS TO MOVE AROUND");
        }
        break;
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
        if (po == 1 && pl.posy > 40){
            al_draw_text(fp, WHITE, 400, 350, 0, "= POTIONS FOR ELIXIR");
            al_draw_text(fp, WHITE, 400, 400, 0, "= HEAL POTION (+1 LIFE)");
            al_draw_text(fp, WHITE, 400, 450, 0, "= SPEED POTION (PRESS");
            al_draw_text(fp, WHITE, 400, 500, 0, "   SPACE BAR TO RUN)");
        }else{
            al_draw_text(fp, WHITE, 285, 400, 0, "REMEMBER TO GET ALL ITEMS");
            al_draw_text(fp, WHITE, 320, 450, 0, "TO COMPLETE YOUR QUEST");
            al_draw_text(fp, WHITE, 320, 500, 0, "FOR THE ELIXIR OF LIFE");
        }
        break;
    case 8:
        al_draw_text(fp, WHITE, 100, 150, 0, "CLICK ON THIS MAIL ICON IN YOUR TOOLBAR TO");
        al_draw_text(fp, WHITE, 50, 200, 0, "READ THE LETTER AGAIN THROUGHOUT THE GAME");
        al_draw_text(fp, WHITE, 480, 700, 0, "CLICK TO OPEN");
        break;
    }
}


