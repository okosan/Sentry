#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <list>
#include "xdynamicobject.h"

class XGameSettings;

const int WORLD_MAX_W = 500;
const int WORLD_MAX_H = 300;

void getWorldDimensions(int &w, int &h);
float worldGetDistance(int x1, int y1, int x2, int y2);


/// The character is a Hero.
void notifyHeroAboutDamage(int damage_points);
void notifyHeroAboutHealing(int heal_points);

/// The object is a "Press".
const int WORLD_MAX_NUM_PRESS = 10;

/// The object is a "Ghost".
const int WORLD_MAX_NUM_GHOST = 10;

/// The object is a "Cloud".
const int WORLD_MAX_NUM_CLOUD = 5;

/// The object is a "Bugs".
const int WORLD_MAX_NUM_BUGS = 10;

/// Maximum number of Robots.
const int WORLD_MAX_NUM_ROBIK = 10;

/// The object is a "Carrier".
const int WORLD_MAX_NUM_CARRIER = 10;

/// The object is an "Interceptors".
const int WORLD_MAX_NUM_INTERCEPTORS = 10;

/// The object is a "Fox".
const int WORLD_MAX_NUM_FOX = 10;

//------------------------------------
const int OBJECT_FIRST      = 0;

const int OBJECT_EMPTY      = 0;
const int OBJECT_WALL       = 1;
const int OBJECT_BLIN       = 2;  // Pancake and it explodes!
const int OBJECT_MEDKIT     = 3;
const int OBJECT_KEY        = 4;
const int OBJECT_DOOR       = 5;
const int OBJECT_FINISH     = 6;
const int OBJECT_TRAP       = 7;
const int OBJECT_GREEN_KEY  = 8;
const int OBJECT_GREEN_DOOR = 9;
const int OBJECT_BLUE_KEY   = 10;
const int OBJECT_BLUE_DOOR  = 11;
const int OBJECT_DINAMITE   = 12;
const int OBJECT_WEAK_WALL  = 13;
const int OBJECT_MAGIC_WELL = 14;
const int OBJECT_MUSHROOMS  = 15;

const int OBJECT_LAST       = 15;

const int BLIN_DAMAGE = 20;
const int MEDKIT_HEALS = 25;
const int MAGIC_WELL_HEALS = 1;
const int MUSHROOMS_HEALS = 10;

struct CoordXY
{
    int x;
    int y;
};

/// Checks whether the  object code is valid.
bool isValidValue(int value);

/// Checks whether the coordinates are valid.
/// true - very well.
/// false - error.
bool isValidCoord(int x, int y);

/// Sets the map object to the specified coordinates.
/// true - very well.
/// false - installation error.
bool setMapValue(int x, int y, int value);

/// Finds the code of the object that is in the specified coordinates.
int getMapValue(int x, int y);

/// Makes the fence on the map.
void makeKert();

/// Clears everything on the map, filling the space with empty objects.
void clearMap();

/// Places the object of the specified type (typeOfObjectToPlace)
/// on the map several times (number  - numberOfObjectsToPlace)
void placeObjects(int numberOfObjectsToPlace, int typeOfObjectToPlace);

/// It will help to find good initial coordinates for the beetle and the ghost.
bool worldGetGoodObjectStartCoords(int &x, int &y);


/// Generates a map. Compiles the map based on the perimeter and additional walls.
/// Then he puts the keys, the Finish. Arranges all the bombs and medkit.
void generateMap();

/// Loads a map from a file. All static objects and map sizes are loaded.
bool loadMapFromFile(int level);
//bool loadMapFromFile(std::string full_file_path);

/// The game world is initialized - it sets up a new game.
bool initWorld(const XGameSettings *pGameSettings);


/// Is the object in the trap zone (input data: ghost coordinates, output - coordinates of the trap)?
/// If an object with coordinates oX, oY falls within the scope of at least one trap, then
/// return true and still set the trap coordinates (trapX, trapY)
bool worldIsObjectInZoneOfTrap(int oX, int oY, int &trapX, int &trapY);

bool worldIsObjectInZoneOfDinamite(int oX, int oY, int &DinamiteX, int &DinamiteY);


/// ----------------------------- Functions of the beetle -----------------------
/// Every cycle of the game the World makes its move. Each object makes its own movements.
bool worldStep();

bool worldIsSolidObstacle(int value);

bool worldGetFirstObjectLocation(int &objectX, int &objectY, int ObjectType);
bool worldGetNearestObjectLocation(int sX, int sY, int &targetX, int &targetY, int ObjectType);

bool worldGetAStarPath(int srcX, int srcY, int dstX, int dstY,
                       std::list< CoordXY > &rvPath,
                       float &rDistance,
                       bool allow_diagonal_moves = true);

/// The World reports the coordinates of the key on the map - used to control the Hero - the Artificial Intelligence
/// true - if the key is on the Map. It also sets the correct values for keyX, keyY.
/// false - if there is no key.
bool worldGetKeyLocation(int &x, int &y);

bool worldGetFinishLocation(int &x, int &y);

bool worldAddOneObjectPress(int startX, int startY, int phaseNumber);
bool worldAddOneObjectGhost(int startX, int startY);
bool worldAddOneObjectCloud(int startX, int startY, int cloudW, int cloudH);
bool worldAddOneObjectBug(int startX, int startY);
bool worldAddOneObjectRobik(int startX, int startY);
bool worldAddOneObjectCarrier(int startX, int startY);
bool worldAddOneObjectInterceptors(int startX, int startY, XDynamicObject *pCarrierOwner);
bool worldAddOneObjectFox(int startX, int startY);

// !!! Game signals section !!!
// Unique function of game mechanics.
// TODO: Find better location later
void signalGameFinished();
void worldNotifyLevelComplete();
void worldNotifyExplosionInArea(int exp_x, int exp_y);
void signalDisplayGameStatistics();


void worldDestroyAllDynamicObjects();
bool worldProgressToNextLevel();


std::string worldGetFilePathForLevel(int loadableLevelNumber);

std::string getCurrentLevelName();

bool isFileExists(std::string filepath);
int worldCountNumberOfLevels();

void worldDrawLine(int x0, int y0, int x1, int y1);
int worldGetNumberOfWallsOnLine(int x0, int y0, int x1, int y1, float step = 0.1f);


#endif // WORLD_H
