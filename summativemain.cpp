
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
    ALLEGRO_FONT *fontPixel = al_load_ttf_font("Minecraft.ttf", 40, 0);
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    // initializing
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    bool playing = true;
    ALLEGRO_COLOR background = al_map_rgb(0, 0, 0);
    LevelBG level[10];
    Character player;
    Object lives;
    int levelNum = 0;
    int phase = 0;
    int hitCounter = 100;
    // making everything opened smoothly
    checkSetup(display, font, fontPixel, timer, event_queue);
    // registering event queues
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    // button stuff
    Button start;
    Button menu;
    Button exitGame;
    Button resume;
    Button nextLevel;
    ALLEGRO_BITMAP *menuCard;
    menuCard = al_load_bitmap("menuCard.png");
    declareButtons(start, menu, exitGame, resume, nextLevel);
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
            // draws text for opening screen
            al_clear_to_color(background);
            al_draw_text(font, WHITE, 340, 400, 0, "THE PLANT KING!");
            al_draw_text(font, WHITE, 300, 500, 0, "EXIT THE ROOM TO WIN");
            if (makeButton(start, ev, fontPixel) == true) {
                // creates and reads all the objects and characters from a text file
                setupLevel(level[levelNum], player, lives, levelNum);
                phase = 1;
            }
            al_flip_display();
            break;
        case 1:
            // moves character and enemy
            moveCharacter(player, level[levelNum], compareCollision(player, level[levelNum]), ev, lives);
            // calculates hitCounter, which determines how long the player remains frozen for
            if (compareCollision(player, level[levelNum]).enemy == true){
                hitCounter = 0;
            }
            hitCounter++;
            isHit(player, level[levelNum], hitCounter, lives);
            // determines how long the enemy moves for
            for (int i = 0; i <10; i++){
                level[levelNum].enemy[i].moveTime --;
            }
            moveEnemy(level[levelNum].enemy, level[levelNum], ev);
            drawLives(lives);
            if (makeButton(menu, ev, font) == true) {
                phase = 5;
            }
            al_flip_display();
            // if the user is a certain distance away from the door the game will end
            if (endLevel(player, level[levelNum].door)) {
                phase = 3;
            }
            // if the user loses all three lives the game ends
            if (lives.amount == 0){
                phase = 4;
            }
            break;
        case 3:
            // end screen
            al_clear_to_color(background);
            al_draw_text(font, WHITE, 300, 400, 0, "Finish");
            if(makeButton(nextLevel, ev, fontPixel) == true){
                levelNum++;
                setupLevel(level[levelNum], player, lives, levelNum);
                phase = 1;
            }
            al_flip_display();
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                playing = false;
            }
            break;
        // when the player dies
        case 4:
            al_clear_to_color(background);
            al_draw_text(font, WHITE, 300, 400, 0, "Lol your dead");
            al_flip_display();
            break;
        // when player hits the menu button
        case 5:
            al_draw_bitmap(menuCard, 150, 100, 0);
            if(makeButton(exitGame, ev, fontPixel) == true){
                phase = 0;
            }else if(makeButton(resume, ev, fontPixel) == true){
                phase = 1;
            }
            al_flip_display();
            break;
        }
    }
    // destroys everything
    al_destroy_bitmap(player.bitmap);
    al_destroy_display(display);
    return 0;
}
