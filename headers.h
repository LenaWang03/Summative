#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

const int SCREEN_W = 1240;
const int SCREEN_H = 1004;
const float FPS = 80;
const float VOLUME = 1.0;

// colour definitions
#define WHITE        al_map_rgb(255, 255, 255)
#define BLACK        al_map_rgb(0, 0, 0)
#define BROWN        al_map_rgb(104,76,60)

/****************************************DECLARING STRUCTS**************************************/
struct Character {
    ALLEGRO_BITMAP *bitmap;         // picture
    int mUp, mDown, mLeft, mRight; // moving units
    int right, left, top, bottom; // boundary box
    int posx, posy;              // position on screen
    const char *filename;
    ALLEGRO_BITMAP *frame[5];
    ALLEGRO_SAMPLE *sound;
};
struct storeCollision
{
    bool u;
    bool d;
    bool l;
    bool r;
    bool enemy;
};

struct Object {
    ALLEGRO_BITMAP *bitmap;
    const char *filename;
    int x,y;
    int right, left, top, bottom;
    int amount;
    ALLEGRO_BITMAP *frame[2];
    int direction;
    int moveTime;
    ALLEGRO_SAMPLE *sound;
};

struct Item {
    const char *filename;
    bool pickUp;
    int amount;
    ALLEGRO_BITMAP *bitmap;
    ALLEGRO_BITMAP *bitmap2;
    int right, left, top, bottom;
    int x,y;
    int totalAmount;
    ALLEGRO_SAMPLE *sound;
    int identifier;

};
struct LevelBG {;
    Object background;
    Object chairsF[30];
    Object desks[30];
    Object enemy[30];
    Object door;
    Item potion;
    Item heal;
};

struct Button {
    ALLEGRO_BITMAP *bitmap;
    int x,y;
    const char *text;
    bool click;
    ALLEGRO_SAMPLE *sound;
};

/******************************SETUP**********************************/
void initializeAllegro();
void checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_FONT *fontPixel, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue);
void setUp (LevelBG a[], Character &b, Object &c, int &l, Item &le);
void getCharacter(Character &a);
void getSetUp(LevelBG a[], char b[][120]);
void loadCharacterImage(Character &a);
void loadObjectImage(Object &a);
void loadItem(Item &a, int x, int y);

/******************************COLLISION DETECTION**********************************/
void calcBounds(Character &a);
void calcObjectBounds(Object &a);
storeCollision isBackgroundCollision(Character &a);
storeCollision isObjectCollision(Character &a, Object b);
storeCollision compareCollision(Character a, LevelBG b);
storeCollision compareEnemyCollision(Object &a, Object b[], Object c[]);
storeCollision isEnemyObjectCollision(Object &a, Object b);
storeCollision isEnemyBackgroundCollision(Object &a);

/******************************GAME PLAY**********************************/
void moveCharacter(Character &p, LevelBG b, storeCollision c, ALLEGRO_EVENT &ev, Object l);
void stopCollision (Character &player, storeCollision);
void moveEnemy(Object a[], LevelBG &b, ALLEGRO_EVENT &ev);
int getEnemyDirection(storeCollision c, int curr);
bool pickUpItem(Character a, Item &b, Object &l);
bool endLevel(Character a, Object d);

/******************************DRAW**********************************/
void drawObjects(Object a[]);
void drawObject(Object a);
void drawBG(LevelBG a, Object &l, ALLEGRO_FONT *f, int level, Item le);
void drawLives(Object a);
int playerAnimation (Character a, int counter);
int enemyAnimation (Object a, int counter);
void loadBitmaps (ALLEGRO_BITMAP *looseBitmaps[]);
void drawCard (int cardNum, ALLEGRO_BITMAP *card[]);
void isHit(Character &a, LevelBG b, int hitCounter, Object &l, ALLEGRO_FONT *f, int level, int counter, Item le);


/******************************PRINT**********************************/
void printCard (ALLEGRO_FONT *fp, int phase, int po, Character p);
void flipPages (int p, char l[][120], ALLEGRO_FONT *fp, ALLEGRO_FONT *f, Item le);

/******************************BUTTONS**********************************/
void setButtons (Button b[], Button levels[]);
bool makeButton (Button a, ALLEGRO_EVENT ev, ALLEGRO_FONT *fontPixel);
void switchPhase(Button &b, int &p, ALLEGRO_EVENT ev, ALLEGRO_FONT *fp, int change);

/******************************CHUNCKS OF MAIN**********************************/
void setItems(Item &l, Item &s, LevelBG &lev, int ln, Character &play, Object &liv, ALLEGRO_EVENT ev, int &phase, ALLEGRO_FONT *fp, Button buttons[]);
void movingParts(Character &p, LevelBG &lev, int &aCount, int &hCount, Object &liv, ALLEGRO_FONT *fp, int ln, ALLEGRO_EVENT ev, Item le);
void ending(Character &p, LevelBG &lev, Item &le, int &phase, ALLEGRO_SAMPLE*s[], int ln, Object &liv);
void callButtons(Character &p, LevelBG &levl, LevelBG lev[], Button b[], ALLEGRO_EVENT ev, ALLEGRO_FONT *fp, int &ln, int &ph, Item &le, bool &pl, int &pa, ALLEGRO_SAMPLE* f, Object &liv, Button lb[]);





