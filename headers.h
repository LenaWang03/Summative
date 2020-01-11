#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
const int SCREEN_W = 1240;
const int SCREEN_H = 1004;
const float FPS = 80;
#define WHITE        al_map_rgb(255, 255, 255)
#define BLACK        al_map_rgb(0, 0, 0)
#define BROWN        al_map_rgb(104,76,60)


struct Character {
    ALLEGRO_BITMAP *bitmap;               // picture
    int mUp, mDown, mLeft, mRight; // moving units
    int right, left, top, bottom; // boundary box
    int posx, posy;             // position on screen
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

void initializeAllegro();
void checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_FONT *fontPixel, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue);
void setUp (LevelBG a[], Character &b, Object &c, int &l, Item &le);
void setupLevel(LevelBG &a, Character &b, Object &c, int l, Item &p);
void moveCharacter(Character &player, LevelBG b, storeCollision c, ALLEGRO_EVENT &ev, Object l);
void calcBounds(Character &a);
void calcObjectBounds(Object &a);
void loadCharacterImage(Character &a);
void loadObjectImage(Object &a);
storeCollision isBackgroundCollision(Character &a);
storeCollision isObjectCollision(Character &a, Object b);
storeCollision compareCollision(Character a, LevelBG b);
void getCharacter(Character &a);
void getSetUp(LevelBG a[], char b[][120]);
void drawObjects(Object a[]);
void drawObject(Object a);
void stopCollision (Character &player, storeCollision);
void drawBG(LevelBG a, Object &l, Item le, ALLEGRO_FONT *f, int level);
bool endLevel(Character a, Object d);
int playerAnimation (Character a, int counter);
void moveEnemy(Object a[], LevelBG &b, ALLEGRO_EVENT &ev);
storeCollision compareEnemyCollision(Object &a, Object b[], Object c[]);
storeCollision isEnemyObjectCollision(Object &a, Object b);
storeCollision isEnemyBackgroundCollision(Object &a);
int enemyAnimation (Object a, int counter);
void isHit(Character &a, LevelBG b, int hitCounter, Object &l, Item le, ALLEGRO_FONT *f, int level, int counter);
void drawLives(Object a);
bool makeButton (Button a, ALLEGRO_EVENT ev, ALLEGRO_FONT *fontPixel);
void declareButtons(Button &start, Button &menu, Button &exitGame, Button &resume, Button &nextLevel, Button &levelSelect, Button levels[], Button &next, Button &last, Button &goBack, Button &letter, Button &finish);
bool pickUpItem(Character a, Item &b, Object &l);
int getEnemyDirection(storeCollision c, int curr);
void flipPages (int p, char l[][120], ALLEGRO_FONT *fp, ALLEGRO_FONT *f, Item le);
void loadBitmaps (ALLEGRO_BITMAP *looseBitmaps[]);
void drawCard (int cardNum, ALLEGRO_BITMAP *card[]);
void printCard (ALLEGRO_FONT *fp, int phase, int po, Character p);
void setButtons (Button b[], Button levels[]);
void switchPhase(Button b, int &p, ALLEGRO_EVENT ev, ALLEGRO_FONT *fp, int change);
void loadItem(Item &a, int x, int y);
void setItems(Item &l, Item &s, LevelBG &lev, int ln, Character &play, Object &liv, ALLEGRO_EVENT ev, int &phase, ALLEGRO_FONT *fp, Button buttons[]);
void movingParts(Character &p, LevelBG &lev, int &aCount, int &hCount, Object &liv, ALLEGRO_FONT *fp, int ln, Item le, ALLEGRO_EVENT ev);
void ending(Character &p, LevelBG &lev, Item le, int &phase, ALLEGRO_SAMPLE *lwin, ALLEGRO_SAMPLE *win, ALLEGRO_SAMPLE *lose, int ln, Object &liv);
