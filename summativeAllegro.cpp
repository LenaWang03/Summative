#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include "allegro5/allegro_image.h"
#include "headers.h"


void initializeAllegro() {
    // Initialize Allegro
    al_init();
    //initialize font addons
    al_init_font_addon();
    al_init_ttf_addon();
}
// loads image for characters
bool loadCharacterImage(Character &image) {
    image.bitmap = al_load_bitmap(image.filename);
    if (image.bitmap == nullptr) {
        printf ("Image %s couldn't load", image.filename);
        return false;
    }
    return true;
}
// loads image for objects
bool loadObjectImage(Object &image) {
    image.bitmap = al_load_bitmap(image.filename);
    if (image.bitmap == nullptr) {
        printf("Image %s couldn't load", image.filename);
        return false;
    }
    return true;
}
// sets the character and the background
void setupLevel(LevelBG &o, Character &b, Character &c) {
    // setting values to variables
    for (int i = 0; i < 10; i++){
        o.chairsF[i].filename = "chairF.png";
        o.desks[i].filename = "table.png";
    }
    o.door.filename = "door.png";
    b.filename = "main.png";
    c.filename = "enemy.png";
    // loading background images
    o.background.bitmap = al_load_bitmap("background.png");
    if (o.background.bitmap == nullptr) {
        printf("Image background couldn't load");
    }
    // prints chairs to the screen
    getCharacter(b);
    getCharacter(c);
    getObjects(o);
    drawObjects(o.chairsF);
    drawObjects(o.desks);
    drawObject(o.door);
}
void getCharacter(Character &a){
    loadCharacterImage(a);
    FILE *coordinates;
    coordinates = fopen ("character.txt", "r");
    fscanf(coordinates, "%d", &a.x);
    fscanf(coordinates, "%d", &a.y);
    fclose(coordinates);
}
// retrieves coordinates and image files
void getObjects(LevelBG &o) {
    FILE *coordinates;
    coordinates = fopen ("setUp.txt", "r");
    fscanf(coordinates, "%d", &o.chairsF[0].amount);
    for (int i = 0; i <o.chairsF[0].amount; i++) {
        loadObjectImage(o.chairsF[i]);
        fscanf(coordinates, "%d", &o.chairsF[i].x);
        fscanf(coordinates, "%d", &o.chairsF[i].y);
    }
    fscanf(coordinates, "%d", &o.desks[0].amount);
    for (int i = 0; i <o.desks[0].amount; i++) {
        loadObjectImage(o.desks[i]);
        fscanf(coordinates, "%d", &o.desks[i].x);
        fscanf(coordinates, "%d", &o.desks[i].y);
    }
    loadObjectImage(o.door);
    fscanf(coordinates, "%d", &o.door.x);
    fscanf(coordinates, "%d", &o.door.y);
    fclose(coordinates);
}

// uses the set clipping dimensions and prints an array of objects to the screen
void drawObjects(Object a[]) {
    for (int i = 0; i <a[0].amount; i++) {
        al_draw_scaled_bitmap(a[i].bitmap,0,0, al_get_bitmap_width(a[i].bitmap),al_get_bitmap_height(a[i].bitmap),a[i].x,a[i].y,al_get_bitmap_width(a[i].bitmap)/3,al_get_bitmap_height(a[i].bitmap)/3, 0);
    }
}

// uses the set clipping dimensions and prints an objects to the screen
void drawObject(Object a) {
    al_draw_scaled_bitmap(a.bitmap,0,0, al_get_bitmap_width(a.bitmap),al_get_bitmap_height(a.bitmap),a.x,a.y,al_get_bitmap_width(a.bitmap)/3.5,al_get_bitmap_height(a.bitmap)/3.5, 0);
}

// checks if everything works, if it doesn't, it gives you an error message
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font) {
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

    return 0;
}

