const int SCREEN_W = 1240;
const int SCREEN_H = 1004;
const float FPS = 80;
#define WHITE        al_map_rgb(255, 255, 255)

struct Character {
    ALLEGRO_BITMAP *bitmap;               // picture
    int mUp, mDown, mLeft, mRight; // moving units
    int right, left, top, bottom; // boundary box
    int posx, posy;             // position on screen
    const char *filename;
    ALLEGRO_BITMAP *frame[5];
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
};
struct LevelBG {;
    Object background;
    Object chairsF[10];
    Object desks[10];
    Object enemy[10];
    Object door;
    Object petal;
};

struct Button {
    ALLEGRO_BITMAP *bitmap;
    int x,y;
    const char *text;
    bool click;
};
struct Item {
    bool pickUp;
    int amount;
};

void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_FONT *fontPixel, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue);
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
void getObjects(LevelBG &a, int l);
void drawObjects(Object a[]);
void drawObject(Object a);
void stopCollision (Character &player, storeCollision);
void drawBG(LevelBG a, Object &l);
bool endLevel(Character a, Object d);
int playerAnimation (Character a);
void moveEnemy(Object a[], LevelBG &b, ALLEGRO_EVENT &ev);
storeCollision compareEnemyCollision(Object &a, Object b[], Object c[]);
storeCollision isEnemyObjectCollision(Object &a, Object b);
storeCollision isEnemyBackgroundCollision(Object &a);
int enemyAnimation (Object a);
void isHit(Character &a, LevelBG b, int hitCounter, Object &l);
void drawLives(Object a);
bool makeButton (Button a, ALLEGRO_EVENT ev, ALLEGRO_FONT *fontPixel);
void declareButtons(Button &start, Button &menu, Button &exitGame, Button &resume, Button &nextLevel);
bool pickUpPetal (Character a, Object b, Item &c);
int getNewDir(storeCollision c, int curr);




