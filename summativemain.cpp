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
    ALLEGRO_FONT *font = al_load_ttf_font("Moon Flower Bold.ttf", 50, 0);
    ALLEGRO_FONT *fontPixel = al_load_ttf_font("Minecraft.ttf", 40, 0);
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    srand(time(0));
    // initializing
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    bool playing = true;
    LevelBG level[10];
    Character player;
    Object lives;
    Item letter;
    int levelNum = 0;
    int phase = 0;
    int hitCounter = 100;
    char letterInfo [18][120];
    int page = 0;
    char buffer[80] = "";
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
    Button levelSelect;
    Button levels[9];
    Button next;
    Button last;
    Button goBack;
    Button openLetter;
    Button finish;
    declareButtons(start, menu, exitGame, resume, nextLevel, levelSelect, levels, next, last, goBack, openLetter, finish);
    ALLEGRO_BITMAP *looseBitmaps[8];
    loadBitmaps(looseBitmaps);
    // setting up components of the game
    getCharacter(player);
    setUp(level, player, lives, levelNum, letter);
    getSetUp(level, letterInfo);

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
            al_draw_scaled_bitmap(looseBitmaps[0],0,0, 620,502,0,0,1240,1004, 0);
            //al_draw_text(font, WHITE, 300, 500, 0, "EXIT THE ROOM TO WIN");
            if (makeButton(start, ev, fontPixel) == true) {
                // creates and reads all the objects and characters from a text file
                phase = 1;
            }
            if (makeButton(levelSelect, ev, fontPixel) == true) {
                // creates and reads all the objects and characters from a text file
                phase = 7;
            }
            al_flip_display();
            break;
        case 1:
            // moves character and enemy
            moveCharacter(player, level[levelNum], compareCollision(player, level[levelNum]), ev, lives);
            // draws background and objects
            drawBG(level[levelNum], lives);
            if (levelNum == 0) {
                if (letter.pickUp == false) {
                    al_draw_bitmap(letter.bitmap, letter.x, letter.y,0);
                    pickUpItem(player, letter);
                } else if (letter.pickUp == true && letter.amount == 1){
                    phase = 8;
                    player.mDown = 0;
                    player.mUp = 0;
                    player.mLeft = 0;
                    player.mRight = 0;
                }
            }
            // puts letter in tool bar section so the user can reread it
            if (letter.pickUp == true){
                if(makeButton(openLetter, ev, fontPixel) == true) {
                    phase = 8;
                }
            }
            al_draw_bitmap(level[0].potion.bitmap, 20, 910,0);
            sprintf(buffer, ": %d", level[levelNum].potion.totalAmount);
            al_draw_text(fontPixel, WHITE, 70, 930, 0, buffer);
            if (level[levelNum].potion.amount == 1) {
                if (level[levelNum].potion.pickUp == false) {
                    al_draw_bitmap(level[levelNum].potion.bitmap, level[levelNum].potion.x,level[levelNum].potion.y,0);
                    pickUpItem(player, level[levelNum].potion);
                }
            }
            for (int i = 0; i < level[levelNum].enemy[0].amount; i++) {
                enemyAnimation(level[levelNum].enemy[i]);
            }
            playerAnimation(player);
            // calculates hitCounter, which determines how long the player remains frozen for
            if (compareCollision(player, level[levelNum]).enemy == true) {
                hitCounter = 0;
            }
            hitCounter++;
            isHit(player, level[levelNum], hitCounter, lives);
            // determines how long the enemy moves for
            for (int i = 0; i <10; i++) {
                level[levelNum].enemy[i].moveTime --;
            }
            moveEnemy(level[levelNum].enemy, level[levelNum], ev);
            drawLives(lives);
            if (makeButton(menu, ev, font) == true) {
                phase = 5;
            }
            al_flip_display();
            // if the user is in front of the door the game will end
            if (endLevel(player, level[levelNum].door)) {
                if (levelNum == 3){
                    phase = 2;
                }
                if (letter.pickUp == false){
                    phase = 6;
                }
                if (level[levelNum].potion.amount == 1 && level[levelNum].potion.pickUp == false) {
                    phase = 6;
                }
                if (levelNum == 8){
                    phase = 2;
                } else if ((level[levelNum].potion.amount == 0)||(level[levelNum].potion.amount == 1 && level[levelNum].potion.pickUp == true&&letter.pickUp == true)) {
                    phase = 3;
                }
            }
            // if the user loses all three lives the game ends
            if (lives.amount == 0) {
                phase = 4;
            }
            break;
        case 2:
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(looseBitmaps[2],150,100,0);
            al_draw_text(fontPixel, WHITE, 350, 350, 0, "YOUR LIFE'S WORK IS ");
            al_draw_text(fontPixel, WHITE, 350, 400, 0, "COMPLETE.  ALL ELEMENTS");
            al_draw_text(fontPixel, WHITE, 350, 450, 0, "OF THE ELIXIR OF LIFE ");
            al_draw_text(fontPixel, WHITE, 350, 500, 0, "HAS BEEN COLLECTED.");
            finish.y = 600;
            if (makeButton(finish, ev, fontPixel) == true) {
                setUp(level, player, lives, levelNum, letter);
                getCharacter(player);
                phase = 0;
            }
            al_flip_display();
            break;
        case 3:
            // next level screen
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(looseBitmaps[2],150,100,0);
            al_draw_text(fontPixel, WHITE, 350, 350, 0, "YOU HAVE SUCCESSFULLY ");
            al_draw_text(fontPixel, WHITE, 350, 400, 0, "COMPLETED THIS ROOM.");
            sprintf(buffer, "YOU HAVE COLLECTED %d/9", level[levelNum].potion.totalAmount);
            al_draw_text(fontPixel, WHITE, 350, 450, 0, buffer);
            al_draw_text(fontPixel, WHITE, 350, 500, 0, "POTIONS SO FAR!");
            if(makeButton(nextLevel, ev, fontPixel) == true) {
                levelNum++;
                getCharacter(player);
                level[levelNum].potion.totalAmount = level[levelNum-1].potion.totalAmount;
                level[levelNum].potion.pickUp = false;
                phase = 1;
            }
            exitGame.y = 700;
            if(makeButton(exitGame, ev, fontPixel) == true) {
                phase = 0;
            }
            al_flip_display();
            break;
        // when the player dies
        case 4:
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(looseBitmaps[4],150,100,0);
            al_draw_text(fontPixel, WHITE, 350, 350, 0, "YOU DIDN'T COMPLETE THE");
            al_draw_text(fontPixel, WHITE, 530, 400, 0, "QUEST");
            al_draw_text(fontPixel, WHITE, 330, 450, 0, "PRESS EXIT TO RETURN TO");
            al_draw_text(fontPixel, WHITE, 500, 500, 0, "THE START");
            if(makeButton(exitGame, ev, fontPixel) == true) {
                phase = 0;
                setUp(level, player, lives, levelNum, letter);
            }
            al_flip_display();
            break;
        case 5:
            // menu
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(looseBitmaps[1], 150, 100, 0);
            exitGame.y = 400;
            if(makeButton(exitGame, ev, fontPixel) == true) {
                phase = 0;
            }
            resume.y = 700;
            if(makeButton(resume, ev, fontPixel) == true) {
                phase = 1;
            }
            al_flip_display();
            break;
        case 6:
            // reminder to let the user know to get the potion
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(looseBitmaps[3], 150, 100, 0);
            al_draw_text(fontPixel, WHITE, 300, 400, 0, "REMEMBER TO GET ALL ITEMS");
            al_draw_text(fontPixel, WHITE, 320, 450, 0, "TO COMPLETE YOUR QUEST");
            al_draw_text(fontPixel, WHITE, 320, 500, 0, "FOR THE ELIXIR OF LIFE");
            resume.y = 600;
            if(makeButton(resume, ev, fontPixel) == true) {
                player.posy += 30;
                player.mUp = 0;
                player.mLeft = 0;
                player.mRight = 0;
                phase = 1;
            }
            al_flip_display();
            break;
        case 7:
            // level menu
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(looseBitmaps[5], 150, 100, 0);
            for (int i = 0; i <9; i++) {
                if(makeButton(levels[i], ev, fontPixel) == true) {
                    levelNum = i;
                    phase = 1;
                }
            }
            if(makeButton(goBack, ev, fontPixel) == true) {
                phase = 0;
            }
            al_flip_display();
            break;
        case 8:
            // premise of the game
            al_draw_bitmap(looseBitmaps[6], 270, 405, 0);
            al_flip_display();
            if(ev.mouse.x >= 270 && ev.mouse.y >= 50 && ev.mouse.x <= 974 && ev.mouse.y <= 885 && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                phase = 9;
            }
            break;
        case 9:
            // letter
            al_draw_tinted_bitmap(looseBitmaps[7], al_map_rgba_f(1,1,1, 0.02), 0,0,0);
            al_draw_bitmap(letter.bitmap2, 270, 50, 0);
            al_draw_bitmap(letter.bitmap3, 220, 340, 0);
            al_draw_bitmap(letter.bitmap3, 260, 270, 0);
            al_draw_bitmap(letter.bitmap3, 300, 300, 0);
            if(makeButton(next, ev, fontPixel) == true) {
                page++;
                page = page % 3;
            }
            if(makeButton(last, ev, fontPixel) == true) {
                page += 2;
                page = page % 3;
            }
            finish.y = 800;
            if(makeButton(finish, ev, fontPixel) == true) {
                phase = 1;
                letter.amount = 0;
                openLetter.click = false;
            }
            flipPages(page, letterInfo, font, fontPixel, letter);
            al_flip_display();
        }
    }
    // destroys everything
    al_destroy_bitmap(player.bitmap);
    al_destroy_display(display);
    return 0;
}
