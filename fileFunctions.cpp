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
void moveCharacter(Character &player, LevelBG b, storeCollision c, ALLEGRO_EVENT &ev) {
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
        drawBG(b);
        // animates and prints the player to the screen every tick
        playerAnimation(player);
        al_flip_display();
    }
}

//moves the enemy randomly
void moveEnemy(Object a[], LevelBG b, int &d, int m, ALLEGRO_EVENT &ev) {
    storeCollision c;
    for (int i = 0; i < a[0].amount; i++) {
        // checks the collisions
        c = compareEnemyCollision(a[i], b.chairsF, b.desks);
        // changes direction if there is a collision
        if ((d ==1 && c.r) || (d == 2 && c.l) || (d == 3 && c.u) || (d ==4 && c.d)) {
            d = (rand()%4)+1;
        }
        // changes direction so it isn't always moving in one direction if it can
        if (m == 0) {
            m = rand() % 800+60;
            d = (rand()%4)+1;
        }
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // applies movement to the enemy if there are no collision
            if (d ==1 && !c.r) {
                a[i].x += 1;
            } else if (d == 2 && !c.l) {
                a[i].x -= 1;
            } else if (d == 3 && !c.u) {
                a[i].y -= 1;
            } else if (d ==4 && !c.d) {
                a[i].y += 1;
            }
        }
        // draws image to display
        al_draw_scaled_bitmap(a[i].bitmap,0,0, al_get_bitmap_width(a[i].bitmap),al_get_bitmap_height(a[i].bitmap),a[i].x,a[i].y,al_get_bitmap_width(a[i].bitmap)/3,al_get_bitmap_height(a[i].bitmap)/3, 0);
    }
}

// ends the level if the player is a certain distance away from the exit
bool endLevel(Character a, Object d) {
    calcBounds(a);
    calcObjectBounds(d);
    if (a.bottom > d.top && (a.bottom < d.bottom +30) && a.left < d.right && a.right > d.left ) {
        return true;
    }
    return false;
}
