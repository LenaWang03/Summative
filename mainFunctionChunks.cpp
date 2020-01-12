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

/******************************************ITEMS*****************************/
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
    // puts potion button in the tool bar in case you want to know the colour codes for the potion again
    switchPhase(b[12], phase, ev, fp, 6);

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

/*****************************ANIMATION**************************************/
void movingParts(Character &p, LevelBG &lev, int &aCount, int &hCount, Object &liv, ALLEGRO_FONT *fp, int ln, ALLEGRO_EVENT ev, Item le) {
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
        if (hCount > 200){
            hCount = 0;
        }
    }
    isHit(p, lev, hCount, liv, fp, ln, aCount, le);
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

/**********************ALL POSSIBILITIES WHEN THE USER TRIES TO EXIT THE ROOM THROUGH THE DOOR*******************************/
void ending(Character &p, LevelBG &lev, Item &le, int &phase, ALLEGRO_SAMPLE* s[], int ln, Object &liv) {
    if (endLevel(p, lev.door)) {
        // if the user forgot to pick up an item it gives them a reminder to go pick it up
        if ((le.pickUp == false) ||(lev.potion.pickUp == false) ) {
            phase = 6;
            // if the user is in level select mode and they finish the last level they will be brought back to the level select menu
        } else if (ln == 8 && lev.potion.totalAmount < 9) {
            phase = 7;
            getCharacter(p);
            al_play_sample(s[3], VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            // if the user wins the whole game it brings them to the winning phase
        } else if (lev.potion.totalAmount == 9) {
            phase = 2;
            al_play_sample(s[3], VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            // if the user completes one level it will bring them to the next level phase
        } else if ((lev.potion.amount == 0)||(lev.potion.amount == 1 && lev.potion.pickUp == true&&le.pickUp == true)) {
            phase = 3;
            al_play_sample(s[4], VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
    }

    /***************************THE USER DIES WITHOUT REACHING THE DOOR****************************/
    // if the user loses all three lives the game ends
    if (liv.amount == 0) {
        phase = 4;
        al_play_sample(s[1], VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    }
}

/******************************BUTTONS********************************************/
void callButtons(Character &p, LevelBG &levl, LevelBG lev[], Button b[], ALLEGRO_EVENT ev, ALLEGRO_FONT *fp, int &ln, int &ph, Item &le, bool &pl, int &pa, ALLEGRO_SAMPLE* f, Object &liv, Button lb[]) {
    // depends on phase to know which set of buttons to print
    switch (ph) {
    case 0:
        // creates the start game button
        if(makeButton(b[0], ev, fp) == true) {
            setUp(lev, p, liv, ln, le);
            getCharacter(p);
            ph = 1;
        }
        // creates the level select button
        if(makeButton(b[5], ev, fp) == true) {
            setUp(lev, p, liv, ln, le);
            ph = 7;
        }
        // creates the exit game button
        b[2].y = 750;
        if(makeButton(b[2], ev, fp) == true) {
            pl = false;
        }
        break;
    case 3:
        // creates the next level button that proceeds the user to the next level
        if(makeButton(b[4], ev, fp) == true) {
            ln++;
            getCharacter(p);
            lev[ln].potion.totalAmount = lev[ln-1].potion.totalAmount;
            lev[ln].potion.pickUp = false;
            ph = 1;
        }
        // creates the exit button that brings user to the start menu
        b[2].y = 700;
        switchPhase(b[2], ph, ev, fp, 0);
        break;
    case 6:
        // lets user exit the screen once they've read the information
        b[3].y = 600;
        if(makeButton(b[3], ev, fp) == true) {
            // lets the character get out of the doorway to continue the level
            if (p.posy < 40) {
                p.posy += 30;
            }
            // so user doesn't keep moving in the direction they were going in before
            p.mUp = 0;
            p.mLeft = 0;
            p.mRight = 0;
            p.mDown = 0;
            ph = 1;
        }
        break;
    case 7:
        // creates the level buttons
        for (int i = 0; i <9; i++) {
            if(makeButton(lb[i], ev, fp) == true) {
                if (i == 0) {
                    le.pickUp = false;
                } else {
                    le.pickUp = true;
                }
                getCharacter(p);
                ln = i;
                ph = 1;
            }
        }
        // creates button to let user go back to the start menu
        switchPhase(b[8], ph, ev, fp, 0);
        break;
    case 9:
        // creates button that lets the user flip forwards through the pages
        if(makeButton(b[6], ev, fp) == true) {
            al_play_sample(f, VOLUME*2, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            pa++;
            pa = pa % 3;
        }
        // creates button that lets the user flip backwards through the pages
        if(makeButton(b[7], ev, fp) == true) {
            al_play_sample(f, VOLUME*2, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            pa += 2;
            pa = pa % 3;
        }
        // lets user exit after they finish reading the letter
        b[10].y = 800;
        if(makeButton(b[10], ev, fp) == true) {
            ph = 1;
            le.amount = 0;
            b[9].click = false;
        }
        break;
    }
}


