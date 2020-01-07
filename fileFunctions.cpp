#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <stdlib.h>
// moves main character

void moveCharacter(Character &player, LevelBG b, storeCollision c, ALLEGRO_EVENT &ev, Object l) {
    calcBounds(player);
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
            player.mUp = -2;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
            player.mDown = 2;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
            player.mLeft = -2;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            player.mRight = 2;
        }
    } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
            player.mUp = 0;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
            player.mDown = 0;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
            player.mLeft = 0;
        }
        if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            player.mRight = 0;
        }
    } else if (ev.type == ALLEGRO_EVENT_TIMER) {
        // makes the amount the player moves 0 in the direction that the collision is
        stopCollision(player, c);
        // applies the movement to the player
        player.posx += player.mLeft;
        player.posx += player.mRight;
        player.posy += player.mUp;
        player.posy += player.mDown;
        // animates and prints the player to the screen every tick
    }
}
int getEnemyDirection(storeCollision c, int curr){
    int ans = curr;
    while(ans == 0 || (ans == 1 && c.u) || (ans == 2 && c.r) || (ans == 3 && c.d) || (ans == 4 && c.l))
    {
        ans = rand()%4+1;
    }
    return ans;
}
//moves the enemy randomly
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

// ends the level if the player is a certain distance away from the exit
bool endLevel(Character a, Object d) {
    calcBounds(a);
    calcObjectBounds(d);
    if (a.bottom > d.top && (a.bottom < d.bottom +20) && a.left < d.right -30 && a.right > d.left +30) {
        return true;
    }
    return false;
}

bool pickUpItem (Character a, Item &b){
    b.left = b.x;
    b.top  = b.y;
    b.right = b.left + al_get_bitmap_width(b.bitmap);
    b.bottom = b.top + al_get_bitmap_height(b.bitmap);
    if ((a.bottom > b.top) && (a.bottom < b.bottom) && (a.right > b.left) && a.left < b.right) {
        b.pickUp = true;
        b.totalAmount++;
    }
    return b.pickUp;
}

