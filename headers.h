// declaring constants
const int SCREEN_W = 1240;
const int SCREEN_H = 1004;
const float FPS = 200;
#define WHITE        al_map_rgb(255, 255, 255)

// structs
struct storeCollision
{
    bool u;
    bool d;
    bool l;
    bool r;
};
struct Character {
    ALLEGRO_BITMAP *bitmap;               // picture
    int x, y;                             // position on screen
    int right, left, top, bottom; // boundary box
    const char *filename;
};
struct Background {
    ALLEGRO_BITMAP *bitmap;
    int x,y;
    int right, left, top, bottom;
};
struct Object {
    ALLEGRO_BITMAP *bitmap;
    const char *filename;
    int x,y;
    int right, left, top, bottom;
    int amount;
};
struct LevelBG {;
    Background background;
    Object chairsF[10];
    Object desks[10];
    Object door;
    Character enemy;
};

// prototypes
void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font);
void setupLevel(LevelBG &o, Character &b, Character &c);
void moveCharacter(Character &player, LevelBG o, storeCollision c);
bool loadCharacterImage(Character &image);
bool loadObjectImage(Object &image);
storeCollision isBackgroundCollision(Character &a);
storeCollision isObjectCollision(Character &a, Object b);
storeCollision compareCollision(Character &a, Object b[], Object c[], Character d);
void calcBounds(Character &a);
void calcObjectBounds(Object &a);
void calcDoorBounds(Object &a);
void getCharacter(Character &a);
void getObjects(LevelBG &o);
void drawObjects(Object a[]);
void drawObject(Object a);
void drawDesk(Object a[]);
bool endLevel(Character a, Object d);
void moveEnemy(Character &a, LevelBG o, storeCollision c, int &d, int m);
//void calcEnemyBounds(Character &a);












