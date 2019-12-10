
/*****************************************************************************
 *	Name:     Lena Wang                                                      *
 *	Date:     December 2018                                                  *
 *                                                                           *
 *	Purpose:                                                                 *
 *	                                                                         *
 *	Usage:                                                                   *
 *	                                                                         *
 *	Revision History:                                                        *
 *	                                                                         *
 *	Known Issues:                                                            *
 *****************************************************************************/
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "headers.h"

int main(int argc, char *argv[]) {
    // initializing variables
    initializeAllegro();
    ALLEGRO_COLOR background = al_map_rgb(0, 0, 0);
    ALLEGRO_DISPLAY *display = nullptr;
    bool playing = true;
    bool gameplay = true;
    ALLEGRO_FONT *font = al_load_ttf_font("Moon Flower Bold.ttf", 36, 0);
    Character player;
    LevelBG one;
    int phase = 1;
    // sets up allegro and
    checkSetup(display, font);
    // sets up beginning of the level
    while (playing) {
        ALLEGRO_KEYBOARD_STATE keyState;
        al_get_keyboard_state(&keyState);
        // lets user exit
        if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)) {
            playing = false;
        }
        switch(phase) {
        case 0:
            al_draw_text(font, WHITE, 100, 100, 0, "THE PLANT KING!");
            al_flip_display();
            if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)) {
                phase = 1;
            }
            break;
        case 1:
            setupLevel(one, player);
            while (gameplay) {
                // moves character and calculates the collision restrictions
                moveCharacter(player, one, compareCollision(player, one.chairsF, one.desks, 2, 2));

                if (endLevel(player, one.door)) {
                    phase = 3;
                    gameplay = false;
                }
                // prints everything to the screen
                al_flip_display();
            }
            printf ("Yay");
            break;

        case 3:
            al_clear_to_color(background);
            al_draw_text(font, WHITE, 100, 100, 0, "Finish, PRESS ENTER");
            al_flip_display();
            if (al_key_down(&keyState, ALLEGRO_KEY_ENTER)) {
            playing = false;
            }
            break;
        }
    }
    //Release the bitmap data and exit with no errors
    al_destroy_bitmap(player.bitmap);
    al_destroy_display(display);
    return 0;
}

