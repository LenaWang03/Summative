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
    if (!al_reserve_samples(4)) {
        printf("Didn't reserve samples!\n");
    }
    al_set_window_title(display, "ROOMS OF KHLORIS");
    bool playing = true;
    LevelBG level[10];
    Character player;
    Object lives;
    Item letter;
    Item speed;
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
    speed.filename = "blue.png";
    loadItem(speed, 330, 530);
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
            hitCounter = 210;
            al_flip_display();
            break;
        case 1:
            // draws background and objects
            drawBG(level[levelNum], lives, letter, fontPixel, levelNum);
            // tells the user that they have to use arrow keys to move until the user moves
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // deals with anything that has to do with picking up items
            setItems(letter, speed, level[levelNum], levelNum, player, lives, ev, phase, fontPixel, buttons);
            // moves and animates the character and enemy
            movingParts(player, level[levelNum], animationCounter, hitCounter, lives, fontPixel, levelNum, letter, ev);
            // if the user clicks the menu button it brings them to the menu phase
            switchPhase(buttons[1], phase, ev, fontPixel, 5);
            ending(player, level[levelNum], letter, phase, levelWinSound, winSound, overSound, levelNum, lives);
            al_flip_display();
            break;
        case 2:
            drawCard(2, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
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
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
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
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
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
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            buttons[2].y = 500;
            switchPhase(buttons[2], phase, ev, fontPixel, 0);
            buttons[3].y = 600;
            switchPhase(buttons[3], phase, ev, fontPixel, 1);
            al_flip_display();
            break;
        case 6:
            // reminder to let the user know to get the potion
            drawCard(3, looseBitmaps);
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            if (levelNum == 0 && level[levelNum].potion.totalAmount == 1){
                al_draw_bitmap(level[0].potion.bitmap, 350, 350, 0);
                al_draw_bitmap(level[2].heal.bitmap, 350, 410, 0);
                al_draw_bitmap(speed.bitmap, 350, 470, 0);
            }
            buttons[3].y = 600;
            if(makeButton(buttons[3], ev, fontPixel) == true) {
                player.posy += 30;
                player.mUp = 0;
                player.mLeft = 0;
                player.mRight = 0;
                player.mDown = 0;
                phase = 1;
                level[levelNum].potion.amount = 0;
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
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
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
