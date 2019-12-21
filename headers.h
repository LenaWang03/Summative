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
};
struct Object {
    ALLEGRO_BITMAP *bitmap;
    const char *filename;
    int x,y;
    int right, left, top, bottom;
    int amount;
};
struct LevelBG {;
    Object background;
    Object chairsF[10];
    Object desks[10];
    Object enemy[10];
    Object door;
};


void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *event_queue);
void setupLevel(LevelBG &a, Character &b);
bool loadCharacterImage(Character &a);
void moveCharacter(Character &player, LevelBG b, storeCollision c, ALLEGRO_EVENT &ev);
void calcBounds(Character &a);
void calcObjectBounds(Object &a);
bool loadCharacterImage(Character &a);
bool loadObjectImage(Object &a);
storeCollision isBackgroundCollision(Character &a);
storeCollision isObjectCollision(Character &a, Object b);
storeCollision compareCollision(Character &a, Object b[], Object c[]);
void getCharacter(Character &a);
void getObjects(LevelBG &a);
void drawObjects(Object a[]);
void drawObject(Object a);
void stopCollision (Character &player, storeCollision);
void drawBG(LevelBG a);
bool endLevel(Character a, Object d);
void calcEnemyBounds(Object &a);
int playerAnimation (Character a);
void moveEnemy(Object a[], LevelBG b, int &d, int m, ALLEGRO_EVENT &ev);
storeCollision compareEnemyCollision(Object &a, Object b[], Object c[]);
storeCollision isEnemyObjectCollision(Object &a, Object b);
storeCollision isEnemyBackgroundCollision(Object &a);






