/*****************************************************************************
 *	Name:     Lena Wang                                                      *
 *	Date:     December 2018   // flashing potion is intentional                                               *
 *****************************************************************************/

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int main(int argc, char *argv[]) {

    /**********************DECLARING/ INITIALIZING*****************************/

    initializeAllegro();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
    ALLEGRO_FONT *font = al_load_ttf_font("Moon Flower Bold.ttf", 50, 0);
    ALLEGRO_FONT *fontPixel = al_load_ttf_font("Pixeled.ttf", 30, 0);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    srand(time(0));
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
    // buttons
    Button levels[9];
    Button buttons[13];
    setButtons(buttons, levels);
    // sounds
    ALLEGRO_SAMPLE *looseSound[5];
    looseSound[0]= al_load_sample("flip.wav");
    looseSound[1] = al_load_sample("over.wav");
    looseSound[2] = al_load_sample("gameMusic.ogg");
    looseSound[3] = al_load_sample("win.wav");
    looseSound[4] = al_load_sample("levelWin.wav");
    // bitmaps
    ALLEGRO_BITMAP *looseBitmaps[9];
    loadBitmaps(looseBitmaps);

    /*************************************SETTING UP GAME************************/
    //getting character positions and initializing character variables
    getCharacter(player);
    //initializing and setting items, objects, etc info
    setUp(level, player, lives, levelNum, letter);
    speed.filename = "blue.png";
    loadItem(speed, 330, 530);
    //gets coordinates and amounts from a text file
    getSetUp(level, letterInfo);
    //starts the game background music
    al_play_sample(looseSound[2], VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    // starting the timer
    al_start_timer(timer);

    /****************************START OF THE GAME*************************************/
    while (playing) {
        // setting up allegro events
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        // ways to exit the game mid way through
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            playing = false;
        }

        // uses a switch statement to go in and out of the different phases/ menu screens that the game has
        switch(phase) {
        /****************************START MENU**********************************/
        case 0:
            // draws text for opening screen
            al_draw_scaled_bitmap(looseBitmaps[0],0,0, 620,502,0,0,1240,1004, 0);
            // creates and calls any buttons used
            callButtons(player, level[levelNum], level, buttons, ev, fontPixel, levelNum, phase, letter, playing, page, looseSound[0], lives, levels);
            // initializes some game variables that might have previously been used by user again
            levelNum = 0;
            animationCounter = 0;
            hitCounter = 210;
            speed.pickUp = false;
            al_flip_display();
            break;

        /****************************GAMEPLAY SCREEN**********************************/
        case 1:
            // draws background and objects
            drawBG(level[levelNum], lives, fontPixel, levelNum, letter);
            // tells the user that they have to use arrow keys to move until the user moves
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // deals with anything that has to do with picking up items
            setItems(letter, speed, level[levelNum], levelNum, player, lives, ev, phase, fontPixel, buttons);
            // moves and animates the character and enemy
            movingParts(player, level[levelNum], animationCounter, hitCounter, lives, fontPixel, levelNum, ev, letter);
            // if the user clicks the menu button it brings them to the menu phase
            switchPhase(buttons[1], phase, ev, fontPixel, 5);
            // gives out all the possibilities of the game ending
            ending(player, level[levelNum], letter, phase, looseSound, levelNum, lives);
            al_flip_display();
            break;

        /****************************GAME FNISHED SCREEN**********************************/
        case 2:
            // draws the background
            drawCard(2, looseBitmaps);
            // prints out the text on screen
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // creates the finish button that brings user back to the start menu
            buttons[10].y = 600;
            switchPhase(buttons[10], phase, ev, fontPixel, 0);
            al_flip_display();
            break;

        /****************************END OF LEVEL SCREEN*********************************/
        case 3:
            // draws the background
            drawCard(2, looseBitmaps);
            // prints out the text on screen
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // creates and calls any buttons used
            callButtons(player, level[levelNum], level, buttons, ev, fontPixel, levelNum, phase, letter, playing, page, looseSound[0], lives, levels);
            al_flip_display();
            break;

        /****************************GAME OVER SCREEN**********************************/
        case 4:
            // draws the background
            drawCard(4, looseBitmaps);
            // prints out the text on screen
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // creates the finish button that brings the user back to the start menu
            buttons[2].y = 600;
            switchPhase(buttons[2], phase, ev, fontPixel, 0);
            al_flip_display();
            break;

        /****************************MENU ACCESSIBLE DURING GAMEPLAY**********************************/
        case 5:
            // draws the background
            drawCard(1, looseBitmaps);
            // prints out the text on screen
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // exit button that brings user back to start menu
            buttons[2].y = 450;
            switchPhase(buttons[2], phase, ev, fontPixel, 0);
            // resume button that allows user to continue playing
            buttons[3].y = 550;
            switchPhase(buttons[3], phase, ev, fontPixel, 1);
            al_flip_display();
            break;

        /****************************INFORMATION SCREEN**********************************/
        case 6:
            // draws the background
            drawCard(3, looseBitmaps);
            // prints out the text on screen
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // creates graphic that lets user know what types of potions there are and what they do
            if (player.posy > 40){
                al_draw_bitmap(level[0].potion.bitmap, 350, 350, 0);
                al_draw_bitmap(level[2].heal.bitmap, 350, 410, 0);
                al_draw_bitmap(speed.bitmap, 350, 470, 0);
                level[levelNum].potion.amount = 0;
                buttons[12].click = false;
            }
            // creates and calls any buttons used
            callButtons(player, level[levelNum], level, buttons, ev, fontPixel, levelNum, phase, letter, playing, page, looseSound[0], lives, levels);
            al_flip_display();
            break;

        /****************************LEVEL SELECT MENU**********************************/
        case 7:
            // draws the background
            drawCard(5, looseBitmaps);
            // creates and calls any buttons used
            callButtons(player, level[levelNum], level, buttons, ev, fontPixel, levelNum, phase, letter, playing, page, looseSound[0], lives, levels);
            hitCounter = 210;
            al_flip_display();
            break;

        /****************************LETTER SCREEN**********************************/
        case 8:
            // prints out the text on screen
            printCard(fontPixel, phase, level[levelNum].potion.totalAmount, player);
            // creates button that allows user to access the information in letter
            switchPhase(buttons[11], phase, ev, fontPixel, 9);
            al_flip_display();
            break;

        /****************************OPEN LETTER W/ INFO SCREEN**********************************/
        case 9:
            // draws the background
            drawCard(8, looseBitmaps);
            // creates and calls any buttons used
            callButtons(player, level[levelNum], level, buttons, ev, fontPixel, levelNum, phase, letter, playing, page, looseSound[0], lives, levels);
            // animation of the text and pages flipping
            flipPages(page, letterInfo, font, fontPixel, letter);
            al_flip_display();
        }
    }
    // destroys everything
    al_destroy_display(display);
    return 0;
}
