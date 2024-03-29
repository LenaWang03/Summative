#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/******************************INITIALIZE ALLEGRO******************************/
void initializeAllegro() {
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_init_acodec_addon();
    al_install_audio();
}

/*****************************INITIALIZE GAME COMPONENTS*********************************/
void setUp (LevelBG a[], Character &b, Object &c, int &l, Item &le){
    // setting values to variables
    for (int x = 0; x < 9; x++){
        for (int i = 0; i < 30; i++) {
        a[x].chairsF[i].filename = "chairF.png";
        a[x].desks[i].filename = "table.png";
        a[x].enemy[i].filename = "enemy.png";
        a[x].enemy[i].direction = rand()% 4+1;
        a[x].enemy[i].moveTime = rand() % 100+60;
        a[x].enemy[i].sound = al_load_sample("die.wav");
        }
        a[x].potion.filename = "potion.png";
        a[x].door.filename = "door.png";
        a[x].background.filename = "background.png";
        a[x].potion.pickUp = false;
        a[x].potion.totalAmount = 0;
        a[x].potion.amount = 1;
        a[x].heal.filename = "heal.png";
        a[x].heal.pickUp = false;
        a[x].heal.identifier = 3;
    }
    le.filename = ("letter.png");
    le.bitmap2 = al_load_bitmap("page.png");
    le.amount = 1;
    loadItem(le, 300, 600);
    b.filename = "player.png";
    b.sound = al_load_sample("step.wav");
    loadCharacterImage(b);
    for (int i=0; i<5; i++) {
        b.frame[i] = al_create_sub_bitmap(b.bitmap, i*145.8, 0, 145.8, 178);
    }
    l = 0;
    c.filename = "heart.png";
    c.frame[0] = al_load_bitmap("blackHeart.png");
    c.amount = 3;
    loadObjectImage(c);
    if (!le.sound){
        printf ("Letter audio couldn't load");
    }
    if (!c.frame[0]){
        printf ("Black hearts bitmap couldn't load");
    }
    if (!le.bitmap2){
        printf("Open letter bitmap couldn't load");
    }
}

/*****************************INITIALIZE GAME COMPONENTS*********************************/
void loadCharacterImage(Character &a) {
    a.bitmap = al_load_bitmap(a.filename);
    if (a.bitmap == nullptr) {
        printf ("Image %s couldn't load", a.filename);
    }
}

/*****************************INITIALIZE GAME COMPONENTS*********************************/
// loads object image
void loadObjectImage(Object &a) {
    a.bitmap = al_load_bitmap(a.filename);
    if (a.bitmap == nullptr) {
        printf("Image %s couldn't load", a.filename);
    }
}

/*****************************INITIALIZE GAME COMPONENTS*********************************/
void loadItem(Item &a, int x, int y){
    a.bitmap = al_load_bitmap(a.filename);
    if (a.bitmap == nullptr) {
        printf("Image %s couldn't load", a.filename);
    }
    a.x = x;
    a.y = y;
    a.pickUp = false;
    a.sound = al_load_sample("pickUp.ogg");
}

/*****************************INITIALIZE GAME COMPONENTS*********************************/
void getCharacter(Character &a){
    a.mUp = 0;
    a.mDown = 0;
    a.mLeft = 0;
    a.mRight = 0;
    a.posx = 1070;
    a.posy = 680;
}

/*****************************INITIALIZE GAME COMPONENTS*********************************/
//gets starting positions and images from one text file for all objects and characters and loads them
void getSetUp(LevelBG a[], char b[][120]) {
    FILE *coordinates;
    if (!coordinates){
        printf("Coordinates file couldn't load");
    }
    // opens text file
    coordinates = fopen ("setUp.txt", "r");
    // reads in game instructions
    for (int i = 0; i <18; i++){
        fgets( b[i], 120, coordinates);
    }
    //gets starting positions and images from one text file for all objects and characters and loads them for all 9 levels
    for (int x = 0; x < 9; x++) {
        // chairs
        // getting the amount of chairs
        fscanf(coordinates, "%d", &a[x].chairsF[0].amount);
        // getting chair coordinates and loading bitmaps for the amount of chairs there are
        for (int i = 0; i <a[x].chairsF[0].amount; i++) {
            // loads image
            loadObjectImage(a[x].chairsF[i]);
            // gets x and y coordinates
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
            loadItem(a[x].potion, 0, 0);
            fscanf(coordinates, "%d", &a[x].potion.x);
            fscanf(coordinates, "%d", &a[x].potion.y);
        }
        // background image
        loadObjectImage(a[x].background);
        fscanf(coordinates, "%d", &a[x].heal.amount);
        for (int i = 0; i < a[x].heal.amount; i++) {
            loadItem(a[x].heal, 0, 0);
            fscanf(coordinates, "%d", &a[x].heal.x);
            fscanf(coordinates, "%d", &a[x].heal.y);
        }
    }
    // checking bitmap loading for enemy
    if (a[1].enemy[0].amount >0){
        if (!a[1].enemy[0].frame[1]){
            printf("Enemy2 bitmap couldn't load");
        }
        if (!a[1].enemy[0].frame[0]){
            printf("Enemy bitmap couldn't load");
        }
    }
    fclose(coordinates);
}

/*****************************INITIALIZES/ CHECKS ALLEGRO FEATURES/ ADDONS*********************************/
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
    // giving display a name
    al_set_window_title(display, "ROOMS OF KHLORIS");
    // Initialize keyboard routines
    if (!al_install_keyboard()) {
        printf("Keyboard couldn't load");
    }
    // need to add image processor
    if (!al_init_image_addon()) {
        printf("Image add on couldn't load");
    }
    // timer
    if (!timer) {
        printf("Timer couldn't load");
    }
    // event queue
    if (!event_queue) {
        printf("Event queue couldn't load");
    }
    // sound files
    if (!al_reserve_samples(100)) {
        printf("samples couldn't reserve\n");
    }
}


