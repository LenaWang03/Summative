#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <stdlib.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/******************PRINTS TEXT TO SCREEN*******************************/
void printCard (ALLEGRO_FONT *fp, int p, int po, Character pl) {
    char buffer[2] = "";
    // knows what to print depending on which phase the game is in
    switch (p) {
    case 1:
        if (pl.posx ==1070 && pl.posy == 680 && po == 0){
            al_draw_text(fp, WHITE, 50, 200, 0, "USE ARROW KEYS OR WASD KEYS TO MOVE AROUND");
        }
        break;
    case 2:
        al_draw_text(fp, WHITE, 320, 350, 0, "YOUR LIFE'S WORK IS ");
        al_draw_text(fp, WHITE, 320, 400, 0, "COMPLETE.  ALL ELEMENTS");
        al_draw_text(fp, WHITE, 320, 450, 0, "OF THE ELIXIR OF LIFE ");
        al_draw_text(fp, WHITE, 320, 500, 0, "HAS BEEN COLLECTED.");
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
        al_draw_text(fp, BROWN, 480, 350, 0, "GAME PAUSED");
        break;
    case 6:
        if (pl.posy > 40){
            al_draw_text(fp, WHITE, 400, 350, 0, "= POTIONS FOR ELIXIR");
            al_draw_text(fp, WHITE, 400, 400, 0, "= HEAL POTION (+1 LIFE)");
            al_draw_text(fp, WHITE, 400, 450, 0, "= SPEED POTION (PRESS");
            al_draw_text(fp, WHITE, 400, 500, 0, "   SPACE BAR TO RUN)");
            al_draw_text(fp, WHITE, 200, 800, 0, "CLICK THE POTION ICON IN TOOLBAR");
            al_draw_text(fp, WHITE, 300, 850, 0, "TO SEE COLOUR CODE AGAIN");
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

/****************************LETTER ANIMATION***********************************/
void flipPages (int p, char l[][120], ALLEGRO_FONT *f, ALLEGRO_FONT *fp, Item le) {
    char b[200] = "";
    // first draws the pages in the letter
    al_draw_bitmap(le.bitmap2, 220, 340, 0);
    al_draw_bitmap(le.bitmap2, 260, 270, 0);
    al_draw_bitmap(le.bitmap2, 300, 300, 0);
    // uses a switch statement to know which page it is on
    switch (p) {
    case 0:
        // takes the text from the setUp text file and transfers it to a buffer
        for (int i = 0; i <6; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
            // text is the printed through the buffer
            al_draw_text(f, BLACK, 380, 370+(i*50), 0, b);
        }
        break;
    case 1:
        al_draw_bitmap(le.bitmap2, 220, 340, 0);
        for (int i = 6; i <12; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
            al_draw_text(f, BLACK, 280, 260+((i-3)*50), 0, b);
        }
        break;
    case 2:
        al_draw_bitmap(le.bitmap2, 260, 270, 0);
        for (int i = 12; i <18; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
            al_draw_text(f, BLACK, 300, 40+((i-6)*50), 0, b);
        }
    }
    // prints the page number
    sprintf(b, "%d/3", p%3+1);
    al_draw_text(fp, BLACK, 590, 200, 0, b);
}


