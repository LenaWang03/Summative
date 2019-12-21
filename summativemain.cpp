
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
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>


int main(int argc, char *argv[]) {
    // setting up allegro stuff
    initializeAllegro();
    // declaring
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_FONT *font = al_load_ttf_font("Moon Flower Bold.ttf", 100, 0);
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    Character player;
    srand(time(0));
    // initializing
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    bool playing = true;
    ALLEGRO_COLOR background = al_map_rgb(0, 0, 0);
    LevelBG level[10];
    int levelNum = 0;
    int phase = 0;
    int direction = rand()% 4+1;
    int moveTime = rand() % 100+60;
    // making everything opened smoothly
    checkSetup(display, font, timer, event_queue);
    // registering event queues
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // start of the game
    al_start_timer(timer);
    while (playing) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        // ways to exit the game mid way through
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            playing = false;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            playing = false;
        }
        // start screen
        switch(phase) {
        case 0:
            al_draw_text(font, WHITE, 320, 400, 0, "THE PLANT KING!");
            al_draw_text(font, WHITE, 280, 500, 0, "EXIT THE ROOM TO WIN");
            al_flip_display();
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                phase = 1;
                // creates and reads all the objects and characters from a text file
                setupLevel(level[levelNum], player);
            }
            break;
        case 1:
            // moves character and enemy
            moveCharacter(player, level[levelNum], compareCollision(player, level[levelNum].chairsF, level[levelNum].desks), ev);
            moveTime --;
            moveEnemy(level[levelNum].enemy, level[levelNum], direction, moveTime, ev);
            // if the user is a certain distance away from the door the game will end
            if (endLevel(player, level[levelNum].door)) {
                phase = 3;
            }
            break;
        case 3:
            // end screen
            al_clear_to_color(background);
            al_draw_text(font, WHITE, 300, 400, 0, "Finish, PRESS ENTER");
            al_flip_display();
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                playing = false;
            }
        }
    }
    // destroys everything
    al_destroy_bitmap(player.bitmap);
    al_destroy_display(display);
    return 0;
}
