#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>

void initializeAllegro() {
    // Initialize Allegro
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
}

// initializes the sprite names and variable values
void setUp (LevelBG a[], Character &b, Object &c, int &l, Item &le){
    // setting values to variables
    for (int x = 0; x < 9; x++){
        for (int i = 0; i < 10; i++) {
        a[x].chairsF[i].filename = "chairF.png";
        a[x].desks[i].filename = "table.png";
        a[x].enemy[i].filename = "enemy.png";
        a[x].enemy[i].direction = rand()% 4+1;
        a[x].enemy[i].moveTime = rand() % 100+60;
        }
        a[x].potion.filename = "potion.png";
        a[x].door.filename = "door.png";
        a[x].background.filename = "background.png";
        a[x].potion.pickUp = false;
        a[x].potion.totalAmount = 0;
    }
    le.bitmap = al_load_bitmap("letter.png");
    le.bitmap2 = al_load_bitmap("openLetter.png");
    le.bitmap3 = al_load_bitmap("page.png");
    le.pickUp = false;
    le.x = 300;
    le.y = 600;
    le.amount = 1;
    b.filename = "player.png";
    loadCharacterImage(b);
    for (int i=0; i<5; i++) {
        b.frame[i] = al_create_sub_bitmap(b.bitmap, i*145.8, 0, 145.8, 178);
    }
    l = 0;
    c.filename = "heart.png";
    c.frame[0] = al_load_bitmap("blackHeart.png");
    c.amount = 3;
    loadObjectImage(c);
    if (!c.frame[0]){
        printf ("Black hearts bitmap couldn't load");
    }
    if (!le.bitmap){
        printf ("Letter bitmap couldn't load");
    }
    if (!le.bitmap2){
        printf("Open letter bitmap couldn't load");
    }
    if (!le.bitmap3){
        printf("Letter page bitmap couldn't load");
    }
}

// loads character image
void loadCharacterImage(Character &a) {
    a.bitmap = al_load_bitmap(a.filename);
    if (a.bitmap == nullptr) {
        printf ("Image %s couldn't load", a.filename);
    }
}
// loads object image
void loadObjectImage(Object &a) {
    a.bitmap = al_load_bitmap(a.filename);
    if (a.bitmap == nullptr) {
        printf("Image %s couldn't load", a.filename);
    }
}

void getCharacter(Character &a){
    FILE *character;
    if (!character){
        printf("Character file couldn't load");
    }
    character = fopen ("character.txt", "r");
    fscanf(character, "%d", &a.posx);
    fscanf(character, "%d", &a.posy);
    a.mUp = 0;
    a.mDown = 0;
    a.mLeft = 0;
    a.mRight = 0;
}
//gets starting positions and images from one text file for all objects and characters and loads them
void getSetUp(LevelBG a[], char b[][120]) {
    FILE *coordinates;
    if (!coordinates){
        printf("Character file couldn't load");
    }
    coordinates = fopen ("setUp.txt", "r");
    for (int i = 0; i <18; i++){
        fgets( b[i], 120, coordinates);
    }
    for (int x = 0; x < 9; x++) {
        // chairs
        fscanf(coordinates, "%d", &a[x].chairsF[0].amount);
        for (int i = 0; i <a[x].chairsF[0].amount; i++) {
            loadObjectImage(a[x].chairsF[i]);
            fscanf(coordinates, "%d", &a[x].chairsF[i].x);
            fscanf(coordinates, "%d", &a[x].chairsF[i].y);
        }
        //desks
        fscanf(coordinates, "%d", &a[x].desks[0].amount);
        for (int i = 0; i <a[x].desks[0].amount; i++) {
            loadObjectImage(a[x].desks[i]);
            fscanf(coordinates, "%d", &a[x].desks[i].x);
            fscanf(coordinates, "%d", &a[x].desks[i].y);
        }
        //enemy
        fscanf(coordinates, "%d", &a[x].enemy[0].amount);
        for (int i = 0; i <a[x].enemy[0].amount; i++) {
            loadObjectImage(a[x].enemy[i]);
            a[x].enemy[i].frame[0] = al_load_bitmap("enemy.png");
            a[x].enemy[i].frame[1] = al_load_bitmap("enemy2.png");
            fscanf(coordinates, "%d", &a[x].enemy[i].x);
            fscanf(coordinates, "%d", &a[x].enemy[i].y);
        }
        // door
        loadObjectImage(a[x].door);
        fscanf(coordinates, "%d", &a[x].door.x);
        fscanf(coordinates, "%d", &a[x].door.y);
        // potion
        fscanf(coordinates, "%d", &a[x].potion.amount);
        for (int i = 0; i < a[x].potion.amount; i++) {
            a[x].potion.bitmap = al_load_bitmap(a[x].potion.filename);
            fscanf(coordinates, "%d", &a[x].potion.x);
            fscanf(coordinates, "%d", &a[x].potion.y);
        }
        loadObjectImage(a[x].background);
    }
    if (!a[0].potion.bitmap){
        printf("Potion bitmap couldn't load");
    }
    if (!a[1].enemy[0].frame[1]){
        printf("Enemy2 bitmap couldn't load");
    }
    if (!a[1].enemy[0].frame[0]){
        printf("Enemy bitmap couldn't load");
    }
    fclose(coordinates);
}
// draws in the hearts and updates them whenever the player loses a life
void drawLives(Object a) {
    for(int i = 0; i < 3; i++) {
        al_draw_scaled_bitmap(a.frame[0],0,0, 224,192,450+(i*80),900,74,64, 0);
    }
    for(int i = 0; i < a.amount; i++) {
        al_draw_scaled_bitmap(a.bitmap,0,0, 224,192,450+(i*80),900,74,64, 0);
    }
}

// checks everything in allegro to make sure they opened properly
void checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_FONT *fontPixel, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue) {
    // Check if your allegro routines worked successfully.
    if (!font) {
        printf("Moon flower font couldn't load");
    }
    if (!fontPixel) {
        printf("Pixel font couldn't load");
    }// Initialize display
    if (!display) {
        printf("Display couldn't load");
    }
    // Initialize keyboard routines
    if (!al_install_keyboard()) {
        printf("Keyboard couldn't load");
    }
    // need to add image processor
    if (!al_init_image_addon()) {
        printf("Image add on couldn't load");
    }
    if (!timer) {
        printf("Timer couldn't load");
    }
    if (!event_queue) {
        printf("Event queue couldn't load");
    }
}


