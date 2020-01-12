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

/******************************CALCULATE CHARACTER BOUNDS***********************/
// calculates bounds for character - accounts for the hair
void calcBounds(Character &a) {
    a.left = a.posx + 40;
    a.top  = a.posy;
    // subtracts some to account for the hair
    a.right = a.left + al_get_bitmap_width(a.frame[0]) - 80;
    a.bottom = a.top + al_get_bitmap_height(a.frame[0]) -10;
}

/******************************CALCULATE OBJECT BOUNDS***********************/
// calculates bounds for objects, it is different from others because the images are scaled
void calcObjectBounds(Object &a) {
    a.left = a.x;
    a.top  = a.y;
    a.right = a.left + (al_get_bitmap_width(a.bitmap)-(al_get_bitmap_width(a.bitmap)*2/3));
    a.bottom = a.top + (al_get_bitmap_height(a.bitmap)-(al_get_bitmap_height(a.bitmap)*2/3));
}

/******************************BACKGROUND COLLISIONS***********************/
storeCollision isBackgroundCollision(Character &a) {
    storeCollision answer;
    // hard coded the wall restrictions, returns true if there is a collision, false if there is no collision
    if (a.bottom > 845) {
        answer.d = true;
    } else {
        answer.d = false;
    }
    if (a.bottom < 180) {
        answer.u = true;
    }else{
        answer.u = false;
    }
    if (a.right > 1180) {
        answer.r = true;
    }else{
        answer.r = false;
    }
    if (a.left < 60) {
        answer.l = true;
    }else{
        answer.l = false;
    }
    return answer;
}

/******************************OBJECT COLLISION***********************/
storeCollision isObjectCollision(Character &a, Object b) {
    // calculates the bounds by calling another function
    calcObjectBounds(b);
    storeCollision answer;
    // if the character is touching the object it will return true
    if (a.bottom > b.top -8 && a.bottom < b.bottom  - 6 && a.left < b.right - 6 && a.right > b.left + 6) {
        answer.d = true;
    } else {
        answer.d = false;
    }
    if (a.bottom < b.bottom && a.bottom > b.top + 6 && a.left < b.right - 6 && a.right > b.left + 6) {
        answer.u = true;
    } else {
        answer.u = false;
    }
    if (a.left < b.right && a.right > b.right + 6 && a.bottom < b.bottom - 6 &&a.bottom > b.top + 6) {
        answer.l = true;
    } else {
        answer.l = false;
    }
    if (a.right > b.left && a.left < b.left - 6 && a.bottom < b.bottom - 6 &&a.bottom > b.top + 6) {
        answer.r = true;
    } else {
        answer.r = false;
    }
    return answer;
}

/******************************COMBINES ALL THE COLLISIONS***********************/
storeCollision compareCollision(Character a, LevelBG b) {
    // looks at all the collisions and processes them
    storeCollision collisionType;
    storeCollision answer;
    answer.u = false;
    answer.d = false;
    answer.l = false;
    answer.r = false;
    answer.enemy = false;
    // checks the chair collisions
    for (int i = 0; i <b.chairsF[0].amount; i++) {
        collisionType = isObjectCollision(a, b.chairsF[i]);
        answer.u |= collisionType.u;
        answer.d |= collisionType.d;
        answer.r |= collisionType.r;
        answer.l |= collisionType.l;
    }
    // checks desk collision
    for (int i = 0; i <b.desks[0].amount; i++) {
        collisionType = isObjectCollision(a, b.desks[i]);
        answer.u |= collisionType.u;
        answer.d |= collisionType.d;
        answer.r |= collisionType.r;
        answer.l |= collisionType.l;
    }
    // checks background collisions
    collisionType = isBackgroundCollision(a);
    answer.u |= collisionType.u;
    answer.d |= collisionType.d;
    answer.r |= collisionType.r;
    answer.l |= collisionType.l;
    // checks enemy collisions
    for (int i = 0; i <b.enemy[0].amount; i++) {
        collisionType = isObjectCollision(a, b.enemy[i]);
        if ((collisionType.u == true) || (collisionType.d == true) ||(collisionType.r == true) ||(collisionType.l == true)){
            answer.enemy |= true;
        }
    }
    // if any one of the collisions returns true it will make answer return true
    return answer;
}

/******************************STOPS MOVEMENT***********************/
void stopCollision (Character &player, storeCollision c){
    // stops player from moving in a certain direction if there is a collision detected
    // works directly with the compare collision function
    // changes player movement to 0 based on which collision it is
    if (c.u == true){
        player.mUp = 0;
    }
    if (c.d == true){
        player.mDown = 0;
    }
    if (c.l == true){
        player.mLeft = 0;
    }
    if (c.r == true){
        player.mRight = 0;
    }
}
/***************SAME THING BUT INSTEAD OF CHARACTER W/ OBJECT COLLISIONS IT IS ENEMY W/ OBJECT COLLISIONS*****/
//background collisions
storeCollision isEnemyBackgroundCollision(Object &a){
    calcObjectBounds(a);
    storeCollision answer;
    if (a.bottom > 855) {
        answer.d = true;
    } else {
        answer.d = false;
    }
    if (a.bottom < 180) {
        answer.u = true;
    }else{
        answer.u = false;
    }
    if (a.right > 1205) {
        answer.r = true;
    }else{
        answer.r = false;
    }
    if (a.left < 20) {
        answer.l = true;
    }else{
        answer.l = false;
    }
    return answer;
}

// calculates the collisions based on the object bounds
storeCollision isEnemyObjectCollision(Object &a, Object b){
    calcObjectBounds(a);
    calcObjectBounds(b);
    storeCollision answer;
    if (a.bottom > b.top && a.bottom < b.bottom  - 3 && a.left < b.right - 3 && a.right > b.left + 3) {
        answer.d = true;
    } else {
        answer.d = false;
    }
    if (a.bottom < b.bottom && a.bottom > b.top + 3 && a.left < b.right -3 && a.right > b.left + 3) {
        answer.u = true;
    } else {
        answer.u = false;
    }
    if (a.left < b.right && a.right > b.right + 3 && a.bottom < b.bottom - 3 &&a.bottom > b.top + 3) {
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

// looks at all the collisions and processes them
storeCollision compareEnemyCollision(Object &a, Object b[], Object c[]) {
    storeCollision collisionType;
    storeCollision answer;
    answer.u = false;
    answer.d = false;
    answer.l = false;
    answer.r = false;
    for (int i = 0; i <b[0].amount; i++) {
        collisionType = isEnemyObjectCollision(a, b[i]);
        answer.u |= collisionType.u;
        answer.d |= collisionType.d;
        answer.r |= collisionType.r;
        answer.l |= collisionType.l;
    }
    for (int i = 0; i <c[0].amount; i++) {
        collisionType = isEnemyObjectCollision(a, c[i]);
        answer.u |= collisionType.u;
        answer.d |= collisionType.d;
        answer.r |= collisionType.r;
        answer.l |= collisionType.l;
    }
    collisionType = isEnemyBackgroundCollision(a);
    answer.u |= collisionType.u;
    answer.d |= collisionType.d;
    answer.r |= collisionType.r;
    answer.l |= collisionType.l;
    return answer;
}



