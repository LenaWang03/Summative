/*****************************************************************************
 *	Name:     Lena Wang                                                      *
 *	Date:     December 2018   // flashing potion is intentional                                               *
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
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int main(int argc, char *argv[]) {
    // setting up allegro stuff
    initializeAllegro();
    // declaring/ initializing
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
    ALLEGRO_FONT *font = al_load_ttf_font("Moon Flower Bold.ttf", 50, 0);
    ALLEGRO_FONT *fontPixel = al_load_ttf_font("Pixeled.ttf", 30, 0);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_SAMPLE *flipSound = al_load_sample("flip.wav");
    ALLEGRO_SAMPLE *overSound = al_load_sample("over.wav");
    ALLEGRO_SAMPLE *musicSound = al_load_sample("gameMusic.ogg");
    ALLEGRO_SAMPLE *winSound = al_load_sample("win.wav");
    ALLEGRO_SAMPLE *levelWinSound = al_load_sample("levelWin.wav");

    srand(time(0));
    if (!al_reserve_samples(4)){
        printf("failed to reserve samples!\n");
        return -1;
    }
    al_set_window_title(display, "ROOMS OF KHLORIS");
    bool playing = true;
    LevelBG level[10];
    Character player;
    Object lives;
    Item letter;
    int levelNum = 0;
    int phase = 0;
    int page = 0;
    int animationCounter = 0;
    int hitCounter = 100;
    char letterInfo [18][120];
    // making everything opened smoothly
    checkSetup(display, font, fontPixel, timer, event_queue);
    // registering event queues
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    // button stuff
    Button levels[9];
    Button buttons[12];
    setButtons(buttons, levels);
    ALLEGRO_BITMAP *looseBitmaps[9];
    loadBitmaps(looseBitmaps);
    // setting up components of the game
    getCharacter(player);
    setUp(level, player, lives, levelNum, letter);
    getSetUp(level, letterInfo);
    al_play_sample(musicSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

    // start of the game
    al_start_timer(timer);

    while (playing) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        // ways to exit the game mid way through
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            playing = false;
        }
        // start screen
        switch(phase) {
        case 0:
            // draws text for opening screen
            al_draw_scaled_bitmap(looseBitmaps[0],0,0, 620,502,0,0,1240,1004, 0);
            if(makeButton(buttons[0], ev, fontPixel) == true) {
                phase = 1;
                setUp(level, player, lives, levelNum, letter);
                getCharacter(player);
            }
            if(makeButton(buttons[5], ev, fontPixel) == true) {
                setUp(level, player, lives, levelNum, letter);
                phase = 7;
            }
            buttons[2].y = 750;
            if(makeButton(buttons[2], ev, fontPixel) == true) {
                playing = false;
            }
            levelNum = 0;
            hitCounter = 110;
            al_flip_display();
            break;
        case 1:
            // moves character and enemy
            moveCharacter(player, level[levelNum], compareCollision(player, level[levelNum]), ev, lives);
            // draws background and objects
            drawBG(level[levelNum], lives, letter, fontPixel, levelNum);
            if (levelNum == 0) {
                if (letter.pickUp == false) {
                    al_draw_bitmap(letter.bitmap, letter.x, letter.y,0);
                    pickUpItem(player, letter);
                } else if (letter.pickUp == true && letter.amount == 1) {
                    phase = 8;
                    player.mDown = 0;
                    player.mUp = 0;
                    player.mLeft = 0;
                    player.mRight = 0;
                }
            }
            // puts letter in tool bar section so the user can reread it
            if (letter.pickUp == true) {
                switchPhase(buttons[9], phase, ev, fontPixel, 8);
            }
            if (level[levelNum].potion.amount == 1) {
                if (level[levelNum].potion.pickUp == false) {
                    al_draw_bitmap(level[levelNum].potion.bitmap, level[levelNum].potion.x,level[levelNum].potion.y,0);
                    pickUpItem(player, level[levelNum].potion);
                }
            }
            animationCounter++;
            for (int i = 0; i < level[levelNum].enemy[0].amount; i++) {
                enemyAnimation(level[levelNum].enemy[i], animationCounter);
            }
            playerAnimation(player, animationCounter);
            // calculates hitCounter, which determines how long the player remains frozen for
            if (compareCollision(player, level[levelNum]).enemy == true) {
                hitCounter = 0;
            }
            hitCounter++;
            isHit(player, level[levelNum], hitCounter, lives, letter, fontPixel, levelNum, animationCounter);
            // determines how long the enemy moves for
            for (int i = 0; i <10; i++) {
                level[levelNum].enemy[i].moveTime --;
            }
            moveEnemy(level[levelNum].enemy, level[levelNum], ev);
            drawLives(lives);
            switchPhase(buttons[1], phase, ev, fontPixel, 5);
            al_flip_display();
            // if the user is in front of the door the game will end
            if (endLevel(player, level[levelNum].door)) {
                if ((letter.pickUp == false) ||(level[levelNum].potion.amount == 1 && level[levelNum].potion.pickUp == false) ) {
                    phase = 6;
                }
                if (levelNum == 8 && level[levelNum].potion.totalAmount < 9) {
                    phase = 7;
                } else if (level[levelNum].potion.totalAmount == 9) {
                    phase = 2;
                    al_play_sample(winSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                } else if ((level[levelNum].potion.amount == 0)||(level[levelNum].potion.amount == 1 && level[levelNum].potion.pickUp == true&&letter.pickUp == true)) {
                    al_play_sample(levelWinSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    phase = 3;
                }
            }
            // if the user loses all three lives the game ends
            if (lives.amount == 0) {
                phase = 4;
                al_play_sample(overSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            }
            break;
        case 2:
            drawCard(2, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount);
            buttons[10].y = 600;
            if (makeButton(buttons[10], ev, fontPixel) == true) {
                setUp(level, player, lives, levelNum, letter);
                getCharacter(player);
                phase = 0;
            }
            al_flip_display();
            break;
        case 3:
            // next level screen
            drawCard(2, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount);
            if(makeButton(buttons[4], ev, fontPixel) == true) {
                levelNum++;
                getCharacter(player);
                level[levelNum].potion.totalAmount = level[levelNum-1].potion.totalAmount;
                level[levelNum].potion.pickUp = false;
                phase = 1;
            }
            buttons[2].y = 700;
            switchPhase(buttons[2], phase, ev, fontPixel, 0);
            al_flip_display();
            break;
        // when the player dies
        case 4:
            drawCard(4, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount);
            buttons[2].y = 600;
            if(makeButton(buttons[2], ev, fontPixel) == true) {
                phase = 0;
                setUp(level, player, lives, levelNum, letter);
                getCharacter(player);
            }
            animationCounter = 0;
            levelNum = 0;
            al_flip_display();
            break;
        case 5:
            // menu
            drawCard(1, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount);
            buttons[2].y = 500;
            switchPhase(buttons[2], phase, ev, fontPixel, 0);
            buttons[3].y = 600;
            switchPhase(buttons[3], phase, ev, fontPixel, 1);
            al_flip_display();
            break;
        case 6:
            // reminder to let the user know to get the potion
            drawCard(3, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount);
            buttons[3].y = 600;
            if(makeButton(buttons[3], ev, fontPixel) == true) {
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
            drawCard(5, looseBitmaps);
            for (int i = 0; i <9; i++) {
                if(makeButton(levels[i], ev, fontPixel) == true) {
                    if (i == 0) {
                        letter.pickUp = false;
                    } else {
                        letter.pickUp = true;
                    }
                    levelNum = i;
                    phase = 1;
                }
            }
            switchPhase(buttons[8], phase, ev, fontPixel, 0);
            al_flip_display();
            break;
        case 8:
            // premise of the game
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount);
            switchPhase(buttons[11], phase, ev, fontPixel, 9);
            al_flip_display();
            break;
        case 9:
            // letter
            drawCard(8, looseBitmaps);
            if(makeButton(buttons[6], ev, fontPixel) == true) {
                al_play_sample(flipSound, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                page++;
                page = page % 3;
            }
            if(makeButton(buttons[7], ev, fontPixel) == true) {
                al_play_sample(flipSound, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                page += 2;
                page = page % 3;
            }
            buttons[10].y = 800;
            if(makeButton(buttons[10], ev, fontPixel) == true) {
                phase = 1;
                letter.amount = 0;
                buttons[9].click = false;
            }
            flipPages(page, letterInfo, font, fontPixel, letter);
            al_flip_display();
        }
    }
    // destroys everything
    al_destroy_display(display);
    return 0;
}
