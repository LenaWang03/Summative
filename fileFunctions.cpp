#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "headers.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>


// calculates the bonds for the character
void calcBounds(Character &a) {
    a.left = a.x + 30;
    a.top  = a.y;
    a.right = a.left + al_get_bitmap_width(a.bitmap) - 60;
    a.bottom = a.top + al_get_bitmap_height(a.bitmap);
}
void calcDoorBounds(Object &a) {
    a.left = a.x;
    a.top  = a.y;
    a.right = a.left + 10;
    a.bottom = a.top + 190;
}
void calcObjectBounds(Object &a) {
    a.left = a.x +5;
    a.top  = a.y +5;
    a.right = a.left + (al_get_bitmap_width(a.bitmap)-(al_get_bitmap_width(a.bitmap)*2/3))-10;
    a.bottom = a.top + (al_get_bitmap_height(a.bitmap)-(al_get_bitmap_height(a.bitmap)*2/3))+5;
}
storeCollision isBackgroundCollision(Character &a) {
    calcBounds(a);
    storeCollision answer;
    if (a.bottom > 855) {
        answer.d = true;
    } else {
        answer.d = false;
    }
    if (a.top < 20) {
        answer.u = true;
    }else{
        answer.u = false;
    }
    if (a.right > 1180) {
        answer.r = true;
    }else{
        answer.r = false;
    }
    if (a.left < 50) {
        answer.l = true;
    }else{
        answer.l = false;
    }
    return answer;
}

storeCollision isObjectCollision(Character &a, Object b) {
    calcBounds(a);
    calcObjectBounds(b);
    storeCollision answer;
    if (a.bottom > b.top && a.bottom < b.bottom  - 3&& a.left < b.right - 3 && a.right > b.left + 3) {
        answer.d = true;
    } else {
        answer.d = false;
    }
    if (a.bottom < b.bottom && a.bottom > b.top + 3 && a.left < b.right -3 && a.right > b.left + 3) {
        answer.u = true;
    } else {
        answer.u = false;
    }
    if (a.left < b.right && a.right > b.right + 3 && a.bottom < b.bottom - 3&&a.bottom > b.top + 3) {
        answer.l = true;
    } else {
        answer.l = false;
    }
    if (a.right > b.left && a.left < b.left - 3&& a.bottom < b.bottom - 3&&a.bottom > b.top + 3) {
        answer.r = true;
    } else {
        answer.r = false;
    }
    return answer;
}

storeCollision compareCollision(Character &a, Object b[], Object c[]) {
    storeCollision collisionType;
    storeCollision answer;
    answer.u = false;
    answer.d = false;
    answer.l = false;
    answer.r = false;
    for (int i = 0; i <b[0].amount; i++) {
        collisionType = isObjectCollision(a, b[i]);
        answer.u |= collisionType.u;
        answer.d |= collisionType.d;
        answer.r |= collisionType.r;
        answer.l |= collisionType.l;
    }
    for (int i = 0; i <c[0].amount; i++) {
        collisionType = isObjectCollision(a, c[i]);
        answer.u |= collisionType.u;
        answer.d |= collisionType.d;
        answer.r |= collisionType.r;
        answer.l |= collisionType.l;
    }
    collisionType = isBackgroundCollision(a);
    answer.u |= collisionType.u;
    answer.d |= collisionType.d;
    answer.r |= collisionType.r;
    answer.l |= collisionType.l;

    return answer;
}

void moveCharacter(Character &player, LevelBG o, storeCollision c) {
    ALLEGRO_KEYBOARD_STATE keyState;
    // Waits a certain amount of time depending on what the FPS was set to
    al_rest(1/FPS);
    al_get_keyboard_state(&keyState);
    // Change the coordinates of the image if the corresponding key is pressed down
    if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && !c.r)
        player.x += 2;
    if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && !c.l)
        player.x -= 2;
    if (al_key_down(&keyState, ALLEGRO_KEY_UP) && !c.u)
        player.y -= 2;
    if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && !c.d)
        player.y += 2;
    al_draw_scaled_bitmap(o.background.bitmap,0,0, 620,502,0,0,1240,1004, 0);
    drawObject(o.door);
    drawObjects(o.chairsF);
    drawObjects(o.desks);
    al_draw_bitmap(player.bitmap,player.x,player.y, 0);
}

bool endLevel(Character a, Object d) {
    calcDoorBounds(d);
    if (a.bottom > d.top && a.bottom < d.bottom && a.left < d.right && a.right > d.left ) {
        return true;
    }
    return false;
}

void moveEnemy(Character &a, LevelBG o, storeCollision c) {
    srand(time(0));
    // Waits a certain amount of time depending on what the FPS was set to
    al_rest(1/FPS);
    // Change the coordinates of the image if the corresponding key is pressed down
    if (!c.r){
        a.x += 2;
    }
    if (!c.l){
        a.x -= 2;
    }
    if (!c.u)
        a.y -= 2;
    if (!c.d)
        a.y += 2;
    al_draw_scaled_bitmap(a.bitmap,0,0, al_get_bitmap_width(a.bitmap),al_get_bitmap_height(a.bitmap),a.x,a.y,al_get_bitmap_width(a.bitmap)/3,al_get_bitmap_height(a.bitmap)/3, 0);
}



