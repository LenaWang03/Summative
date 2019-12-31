
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
}

// initializes the sprite names and gets the starting positions from text files and prints it to the screen
void setupLevel(LevelBG &a, Character &b, Object &c) {
    // setting values to variables
    for (int i = 0; i < 10; i++){
        a.chairsF[i].filename = "chairF.png";
        a.desks[i].filename = "table.png";
        a.enemy[i].filename = "enemy.png";
    }
    a.door.filename = "door.png";
    b.filename = "player.png";
    a.background.filename = "background.png";
    c.filename = "heart.png";
    c.amount = 3;
    // prints everything to the screen
    getCharacter(b);
    getObjects(a);
    loadObjectImage(c);
    loadObjectImage(a.background);
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

// gets starting position and images for character
void getCharacter(Character &a){
    a.mUp = 0;
    a.mDown = 0;
    a.mLeft = 0;
    a.mRight = 0;
    loadCharacterImage(a);
    FILE *coordinates;
    coordinates = fopen ("character.txt", "r");
    fscanf(coordinates, "%d", &a.posx);
    fscanf(coordinates, "%d", &a.posy);
    fclose(coordinates);
    for (int i=0; i<5; i++) {
        a.frame[i] = al_create_sub_bitmap(a.bitmap, i*154 , 0 , 154, 195);
	}
}

//gets starting positions and images from one text file for all objects
void getObjects(LevelBG &a) {
    FILE *coordinates;
    coordinates = fopen ("setUp.txt", "r");
    fscanf(coordinates, "%d", &a.chairsF[0].amount);
    for (int i = 0; i <a.chairsF[0].amount; i++) {
        loadObjectImage(a.chairsF[i]);
        fscanf(coordinates, "%d", &a.chairsF[i].x);
        fscanf(coordinates, "%d", &a.chairsF[i].y);
    }
    fscanf(coordinates, "%d", &a.desks[0].amount);
    for (int i = 0; i <a.desks[0].amount; i++) {
        loadObjectImage(a.desks[i]);
        fscanf(coordinates, "%d", &a.desks[i].x);
        fscanf(coordinates, "%d", &a.desks[i].y);
    }
    fscanf(coordinates, "%d", &a.enemy[0].amount);
    for (int i = 0; i <a.enemy[0].amount; i++) {
        //loadObjectImage(a.enemy[i]);
        loadObjectImage(a.enemy[i]);
        fscanf(coordinates, "%d", &a.enemy[i].x);
        fscanf(coordinates, "%d", &a.enemy[i].y);
        for (int x=0; x<2; x++) {
        a.enemy[i].frame[x] = al_create_sub_bitmap(a.enemy[i].bitmap, x*302.5 , 0 , 302.5, 393);
        }
    }
    loadObjectImage(a.door);
    fscanf(coordinates, "%d", &a.door.x);
    fscanf(coordinates, "%d", &a.door.y);
    fclose(coordinates);
}
// draws in the hearts and updates them whenever the player loses a life
void drawLives(Object a){
    for(int i = 0; i < a.amount; i++){
        al_draw_scaled_bitmap(a.bitmap,0,0, 224,192,900+(i*80),900,74,64, 0);
    }
}

// checks everything in allegro to make sure they opened properly
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue) {
    // Check if your allegro routines worked successfully.
    if (!font) {
        al_show_native_message_box(display, "Error", "Error", "Could not load comic.ttf",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }// Initialize display
	display = al_create_display(SCREEN_W, SCREEN_H);
    al_set_window_title(display, "Summative");

	if (!display) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
       	return -1;
	}

  	// Initialize keyboard routines
	if (!al_install_keyboard()) {
	    al_show_native_message_box(display, "Error", "Error", "failed to initialize the keyboard!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
      	return -1;
   	}

	// need to add image processor
 	if (!al_init_image_addon()) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize image addon!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}
	if (!timer) {
   		al_show_native_message_box(display, "Error", "Error", "Failed to create timer!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
	}
    if (!event_queue) {
		al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

    return 0;
}


