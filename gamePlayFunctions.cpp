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

/*************************************MOVES MAIN CHARACTER**********************************/
void moveCharacter(Character &p, LevelBG b, storeCollision c, ALLEGRO_EVENT &ev, Object l) {
    calcBounds(p);
    // depending on the keyboard, the amount the character will move by is changed
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_W) {
            p.mUp = -2;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN || ev.keyboard.keycode == ALLEGRO_KEY_S) {
            p.mDown = 2;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT || ev.keyboard.keycode == ALLEGRO_KEY_A) {
            p.mLeft = -2;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT || ev.keyboard.keycode == ALLEGRO_KEY_D) {
            p.mRight = 2;
        }
    // once player stops pressing the button the amount it is moving by will change to 0
    } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_W) {
            p.mUp = 0;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN || ev.keyboard.keycode == ALLEGRO_KEY_S) {
            p.mDown = 0;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT || ev.keyboard.keycode == ALLEGRO_KEY_A) {
            p.mLeft = 0;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT || ev.keyboard.keycode == ALLEGRO_KEY_D) {
            p.mRight = 0;
        }
    // executes the movement every frame
    } else if (ev.type == ALLEGRO_EVENT_TIMER) {
        // makes the amount the player moves 0 in the direction that the collision is
        stopCollision(p, c);
        // applies the movement to the player
        p.posx += p.mLeft;
        p.posx += p.mRight;
        p.posy += p.mUp;
        p.posy += p.mDown;
    }
}

/*************************************GETS DIRECTION**********************************/
int getEnemyDirection(storeCollision c, int curr) {
    int ans = curr;
    // generates a random number between 1 to 4 for a direction until the direction it gets doesn't conflict with a collision
    while(ans == 0 || (ans == 1 && c.u) || (ans == 2 && c.r) || (ans == 3 && c.d) || (ans == 4 && c.l)) {
        ans = rand()%4+1;
    }
    return ans;
}
/*************************************MOVE ENEMY**********************************/
void moveEnemy(Object a[], LevelBG &b, ALLEGRO_EVENT &ev) {
    storeCollision c;
    for (int i = 0; i < a[0].amount; i++) {
        // checks the collisions
        c = compareEnemyCollision(a[i], b.chairsF, b.desks);
        // changes direction so it isn't always moving in one direction if it can
        if (b.enemy[i].moveTime == 0) {
            b.enemy[i].moveTime = (rand() % 800)+60;
            b.enemy[i].direction = getEnemyDirection(c, 0);
        }
        // changes direction if there is a collision
        b.enemy[i].direction = getEnemyDirection(c, b.enemy[i].direction);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // applies movement to the enemy if there are no collision
            if (b.enemy[i].direction == 1 && !c.u) {
                a[i].y -= 1;
            } else if (b.enemy[i].direction == 2 && !c.r) {
                a[i].x += 1;
            } else if (b.enemy[i].direction == 3 && !c.d) {
                a[i].y += 1;
            } else if (b.enemy[i].direction == 4 && !c.l) {
                a[i].x -= 1;
            }
        }
    }
}

/*************************************ENDS LEVEL**********************************/
bool endLevel(Character a, Object d) {
    calcBounds(a);
    calcObjectBounds(d);
    // ends the level if the player is a certain distance away from the exit
    if (a.bottom > d.top && (a.bottom < d.bottom +20) && a.left < d.right -30 && a.right > d.left +30) {
        return true;
    }
    return false;
}

/*************************************DETECTS ITEM PICKUP**********************************/
bool pickUpItem (Character a, Item &b, Object &l) {
    b.left = b.x;
    b.top  = b.y;
    b.right = b.left + al_get_bitmap_width(b.bitmap);
    b.bottom = b.top + al_get_bitmap_height(b.bitmap);
    // knows if the player is touching the item
    if ((a.bottom > b.top) && (a.bottom < b.bottom) && (a.right > b.left) && a.left < b.right) {
        // things it executes when the player collects the item
        b.pickUp = true;
        b.totalAmount++;
        if (l.amount < 3 && b.identifier == 3) {
            l.amount ++;
        }
        // plays pick up sound
        al_play_sample(b.sound, VOLUME, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    }
    return b.pickUp;
}



