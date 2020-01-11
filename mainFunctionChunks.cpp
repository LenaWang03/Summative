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

void setItems(Item &l, Item &s, LevelBG &lev, int ln, Character &play, Object &liv, ALLEGRO_EVENT ev, int &phase, ALLEGRO_FONT *fp, Button b[]) {
    /***************************************HEALING POTION***********************************/
    //draws healing potion
    if (lev.heal.pickUp == false && lev.heal.amount == 1) {
        al_draw_bitmap(lev.heal.bitmap, lev.heal.x, lev.heal.y,0);
        pickUpItem(play, lev.heal, liv);
    }
    /***************************************SPEED POWER UP POTION***********************************/
    // decides where to put the speed power up
    if (ln == 5) {
        if (s.pickUp == false) {
            al_draw_bitmap(s.bitmap, s.x, s.y,0);
            pickUpItem(play, s, liv);
        }
    }
    // changes the speed of the player depending on if the space bar is clicked or not
    if (s.pickUp == true) {
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                play.mLeft = play.mLeft*2;
                play.mUp = play.mUp*2;
                play.mDown = play.mDown*2;
                play.mRight = play.mRight*2;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                play.mLeft = play.mLeft/2;
                play.mUp = play.mUp/2;
                play.mDown = play.mDown/2;
                play.mRight = play.mRight/2;
            }
        }
    }
    /***************************************LETTER***********************************/
    //drawing the letter
    if (l.pickUp == false && ln == 0) {
        al_draw_bitmap(l.bitmap, l.x, l.y,0);
        pickUpItem(play, l, liv);
        // changes the phase to open the letter
    } else if (l.pickUp == true && l.amount == 1 && ln == 0) {
        phase = 8;
        play.mDown = 0;
        play.mUp = 0;
        play.mLeft = 0;
        play.mRight = 0;
    }
    // puts letter in tool bar section so the user can reread it
    if (l.pickUp == true) {
        switchPhase(b[9], phase, ev, fp, 8);
    }
    /***************************************COLLECTING POTION***********************************/
    // draws collecting potion
    if (lev.potion.amount == 1 && lev.potion.pickUp == false) {
        al_draw_bitmap(lev.potion.bitmap, lev.potion.x,lev.potion.y,0);
        pickUpItem(play, lev.potion, liv);
    }
    // goes into an explanation of the different potions by switching pages
    if (lev.potion.pickUp == true && lev.potion.amount == 1 && ln == 0) {
        phase = 6;
    }
}

void movingParts(Character &p, LevelBG &lev, int &aCount, int &hCount, Object &liv, ALLEGRO_FONT *fp, int ln, Item le, ALLEGRO_EVENT ev) {
    /*****************************ANIMATION**************************************/
    //animation of the enemy
    aCount++;
    for (int i = 0; i < lev.enemy[0].amount; i++) {
        enemyAnimation(lev.enemy[i], aCount);
    }
    // animation of the player
    playerAnimation(p, aCount);
    // calculates hitCounter, which determines how long the player remains flashing for
    hCount++;
    if (compareCollision(p, lev).enemy == true) {
        hCount = 0;
    }
    isHit(p, lev, hCount, liv, le, fp, ln, aCount);
    /*************************MOVEMENT*********************************************/
    // moves character
    moveCharacter(p, lev, compareCollision(p, lev), ev, liv);
    // determines how long the enemy moves for
    for (int i = 0; i <10; i++) {
        lev.enemy[i].moveTime --;
    }
    // moves the enemy
    moveEnemy(lev.enemy, lev, ev);
}

void ending(Character &p, LevelBG &lev, Item le, int &phase, ALLEGRO_SAMPLE *lwin, ALLEGRO_SAMPLE *win, ALLEGRO_SAMPLE *lose, int ln, Object &liv) {
    // all possibilities of the game ending
    /**********************ALL POSSIBILITIES WHEN THE USER TRIES TO EXIT THE ROOM THROUGH THE DOOR*******************************/
    if (endLevel(p, lev.door)) {
        // if the user forgot to pick up an item it gives them a reminder to go pick it up
        if ((le.pickUp == false) ||(lev.potion.amount == 1 && lev.potion.pickUp == false) ) {
            phase = 6;
        }
        // if the user is in level select mode and they finish the last level they will be brought back to the level select menu
        if (ln == 8 && lev.potion.totalAmount < 9) {
            phase = 7;
            getCharacter(p);
            al_play_sample(win, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        // if the user wins the whole game it brings them to the winning phase
        } else if (lev.potion.totalAmount == 9) {
            phase = 2;
            al_play_sample(win, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        // if the user completes one level it will bring them to the next level phase
        } else if ((lev.potion.amount == 0)||(lev.potion.amount == 1 && lev.potion.pickUp == true&&le.pickUp == true)) {
            phase = 3;
            al_play_sample(lwin, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
    }

    /***************************THE USER DIES WITHOUT REACHING THE DOOR****************************/
    // if the user loses all three lives the game ends
    if (liv.amount == 0) {
        phase = 4;
        al_play_sample(lose, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    }
}


void flipPages (int p, char l[][120], ALLEGRO_FONT *f, ALLEGRO_FONT *fp, Item le) {
    char b[200] = "";
    al_draw_bitmap(le.bitmap2, 220, 340, 0);
    al_draw_bitmap(le.bitmap2, 260, 270, 0);
    al_draw_bitmap(le.bitmap2, 300, 300, 0);
    switch (p) {
    case 0:
        for (int i = 0; i <6; i++) {
            sprintf(b, "%s", l[i]);
            b[strlen(b)-1]='\0';
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
    sprintf(b, "%d/3", p%3+1);
    al_draw_text(fp, BLACK, 590, 200, 0, b);
}
