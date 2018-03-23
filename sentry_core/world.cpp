 #include "world.h"

#include "stdio.h"
#include "stdlib.h"

#include "xobjectplayerhero.h"

#include "xobjectpress.h"
#include "xobjectghost.h"
#include "xobjectcloud.h"
#include "xobjectbug.h"
#include "xobjectrobik.h"
#include "xobjectcarrier.h"
#include "xobjectinterceptors.h"

#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#include "render.h"

// ---------------------------
/// random level generator parameters (generateMap)
/// Number of air traps at the start of the game.
const int NUMBER_OF_BLIN_ON_START = 3;

/// Number of first-aid kits at the start of the game.
const int NUMBER_OF_MEDKIT_ON_START = 2;

/// Number of dinamite at the start of the game.
const int NUMBER_OF_DINAMITE_ON_START = 5;

/// Number of mushrooms at the start of the game.
const int NUMBER_OF_MUSHROOMS_ON_START = 2;


const XGameSettings *world_pGameSettings = NULL;



// ---------------------------
/// World.
/// Hero character.
XObjectPlayerHero hero;

/// An array of dynamic Press objects.
XObjectPress press[WORLD_MAX_NUM_PRESS];

/// An array of dynamic specimens of the Ghost.
XObjectGhost ghost[WORLD_MAX_NUM_GHOST];

/// Array of dynamic Cloud objects.
XObjectCloud cloud[WORLD_MAX_NUM_CLOUD];

/// An array of dynamic Bugs objects.
XObjectBug bugs[WORLD_MAX_NUM_BUGS];

/// An array of dynamic Robik objects.
XObjectRobik robiks[WORLD_MAX_NUM_ROBIK];

/// An array of dynamic Carrier objects.
XObjectCarrier carrier[WORLD_MAX_NUM_CARRIER];

/// An array of dynamic Interceptors objects.
XObjectInterceptors interceptors[WORLD_MAX_NUM_INTERCEPTORS];

///@{
/// Start Coordinates (X, Y) of the Player Hero on the map.
int WorldHeroStartLocationX = 0;
int WorldHeroStartLocationY = 0;
///@}


int W = 0;   ///< Width of the world in tiles (width).
int H = 0;   ///< Height of the world in tiles (height).
/// Global variable of matrix of static objects of the World.
int matrix[WORLD_MAX_W][WORLD_MAX_H];

int m_level = -1;
int m_levelCount = -1;
std::string m_currentLevelName;

/// The check box indicates whether the current level is complete.
bool m_worldLevelIsComplete = false;

/// The check box displays whether you need to display the game statistics bar.
bool m_displayGameStatistics = false;

/*!
    \brief Shows the size of the world

    \param[out] w  The width of the World is turned.
    \param[out] h  The height of the world is turned.
 */
void getWorldDimensions(int &rW, int &rH)
{
    rW = W;
    rH = H;
}


float worldGetDistance(int x1, int y1, int x2, int y2)
{
    float len_x = fabs(x2 - x1);
    float len_y = fabs(y2 - y1);
    float len = sqrt(len_x * len_x + len_y * len_y);
    return len;
}

void notifyHeroAboutDamage(int damagePoints)
{
    hero.notifyAboutDamage(damagePoints);
}

void notifyHeroAboutHealing(int healPoints)
{
    hero.notifyAboutHealing(healPoints);
}

/// Checks whether the valid object code is valid.
bool isValidValue(int value)
{
    if (value >= OBJECT_FIRST && value <= OBJECT_LAST)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/// Checks whether the coordinates are valid.
/// true - everything is fine.
/// false - error.
bool isValidCoord(int x, int y)
{
    if ((x < W) && (y < H) && (x >= 0) && (y >= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/// Sets the item to the map in the specified coordinates.
/// true - everything is fine.
/// false - error setting of the object.
bool setMapValue(int x, int y, int value)
{
    /// The coordinates are correct and invested in the framework.
    if (isValidCoord(x, y) && isValidValue(value))
    {
        matrix[x][y] = value;
        return true; // 1
    }
    else
    {
        printf("Incorrect matrix placement at [%d, %d] value [%d] !!!\n", x, y, value);
        return false; // 0
    }
}

/// Finds the code of the object that is in the specified coordinates.
int getMapValue(int x, int y)
{
    int value = matrix[x][y];
    return value;
}

/// Makes the fence on the map.
void makeKert()
{
    int x, y;

    /// Horizontal lines.
    y = 0;
    for (x = 0; x < W; x++)
    {
        setMapValue(x, y,   OBJECT_WALL);
    }

    y = H - 1;
    for (x = 0; x < W; x++)
    {
        setMapValue(x, y,   OBJECT_WALL);
    }

    /// Vertical lines.
    x = 0;
    for (y = 0; y < H; y++)
    {
        setMapValue(x, y,   OBJECT_WALL);
    }

    x = W - 1;
    for (y = 0; y < H; y++)
    {
        setMapValue(x, y,   OBJECT_WALL);
    }
}

/// Clears everything on the map, filling the space with empty objects.
void clearMap()
{
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            setMapValue(x, y, OBJECT_EMPTY);
        }
    }
}

/// Places the object of the specified type (typeOfObjectToPlace)
/// on the map several times (number - numberOfObjectsToPlace)
void placeObjects(int numberOfObjectsToPlace, int typeOfObjectToPlace)
{
    int numberOfObjectsPlaced = 0;
    while (1)
    {
        if (numberOfObjectsPlaced == numberOfObjectsToPlace)
        {
            break;
        }

        /// Trying to place a blin.
        int x_random = rand() % W;
        int y_random = rand() % H;

        // Checks whether there is a place
        int value = getMapValue(x_random, y_random);
        if (value == 0)
        {
            setMapValue(x_random, y_random, typeOfObjectToPlace);
            numberOfObjectsPlaced ++;
        }
    }
    /// All objects of the given type are successfully placed.
}

void worldDrawLine(int x0, int y0, int x1, int y1)
{
    setMapValue(x0, y0, OBJECT_TRAP);  // 1, 2,
    setMapValue(x1, y1, OBJECT_TRAP);  // 5, 7

    float vectorX = x1 - x0; // 4
    float vectorY = y1 - y0; // 5

    float length = sqrt(vectorX*vectorX + vectorY*vectorY);
    float vectorNormX = vectorX/length; // 4
    float vectorNormY = vectorY/length; // 5

    //
    printf("\n\nNorm vector = %f, %f\n\n", vectorNormX, vectorNormY);

    // 5 - 0.5, 1, 1.5, 2, 2.5, 3
    float mulpiplier = 0;

    float step = 0.25;
    float numSamples = length / step;
    int numSamplesInt = (int)numSamples;
    for (int i = 0; i < numSamplesInt; i++)
    {
        mulpiplier += step;
        //printf("Mnozhnik = %f\n", mulpiplier);

        float pointXv = vectorNormX * mulpiplier;
        float pointYv = vectorNormY * mulpiplier;

        float pointX = pointXv + x0;
        float pointY = pointYv + y0;


        setMapValue(pointX, pointY, OBJECT_WALL);
    }
}

/// Count the number of walls in a straight line between two coordinates.
/// Coordinates are provided inclusively.
int worldGetNumberOfWallsOnLine(int x0, int y0, int x1, int y1, float step)
{
    int objectCount = 0;

    float vectorX = x1 - x0; // 4
    float vectorY = y1 - y0; // 5

    float length = sqrt(vectorX*vectorX + vectorY*vectorY);
    float vectorNormX = vectorX / length; // 4
    float vectorNormY = vectorY / length; // 5

    // 5 - 0.5, 1, 1.5, 2, 2.5, 3
    float multiplier = 0;

    float numSamples = length / step;
    int numSamplesInt = (int)numSamples;

    int lastX = x0;
    int lastY = y0;
    for (int i = 0; i < numSamplesInt; i++)
    {
        float pointXv = vectorNormX * multiplier;
        float pointYv = vectorNormY * multiplier;

        float pointX = pointXv + x0;
        float pointY = pointYv + y0;

        int testX = (int)roundf(pointX);
        int testY = (int)roundf(pointY);

        int value = getMapValue(testX, testY);
        if (((value == OBJECT_WALL) || (value == OBJECT_WEAK_WALL)) && ((lastX != testX) || (lastY != testY)))
        {
            objectCount ++;
            lastX = testX;
            lastY = testY;
        }

        multiplier += step;
    }
    return objectCount;
}

/// Generates test card. It compiles a card from the driver and additional walls.
/// Then decomposes the Keys, the Finish.  Plays all the bombs and first aid kits.
void generateMap()
{
    W = 30+10; //  width
    H = 12+6;   // height

    WorldHeroStartLocationX = 3;
    WorldHeroStartLocationY = 3;

    clearMap();
    //return;

    //worldDrawLine(1, 2, 15, 7);

    makeKert();

    // you need to set the pattern here
    int x, y;

    x = 2;
    y = 2;
    setMapValue(x, y, OBJECT_WALL);

    x = 2;
    y = 3;
    setMapValue(x, y, OBJECT_WALL);

    x = 2;
    y = 4;
    setMapValue(x, y, OBJECT_WALL);

    /// Carries out the preparatory stages to the Press >>
    for (y = 1; y <= 4; y++)
    {
        x = 12;
        setMapValue(x, y, OBJECT_WALL);
    }

    x = 12;
    y = 2;
    setMapValue(x, y, OBJECT_EMPTY);
    // Press <<

    x = 5;
    y = 1;
    setMapValue(x, y, OBJECT_WALL);

    /// Places the Key.
    x = W - 2;
    y = 1;
    setMapValue(x, y, OBJECT_KEY);

    /// Places the Doors.
    x = W - 2;
    y = H - 3;
    setMapValue(x, y, OBJECT_DOOR);

    /// Places the walls for the Doors.
    x = W - 3;
    y = H - 3;
    setMapValue(x, y, OBJECT_WALL);

    x = W - 3;
    y = H - 2;
    setMapValue(x, y, OBJECT_WALL);

    /// Places the Finish line.
    x = W - 2;
    y = H - 2;
    setMapValue(x, y, OBJECT_FINISH);

    /// Adds the walls of the Mausoleum to the Lenin.
    x = 6;
    y = H - 2;
    setMapValue(x, y, OBJECT_WALL);
    x = 8;
    y = H - 2;
    setMapValue(x, y, OBJECT_WALL);

    /// Places the air trap for testing.
    x = 7;
    y = 7;
    setMapValue(x, y, OBJECT_TRAP);

    /// Places dynamite for testing.
    x = 8;
    y = 8;
    setMapValue(x, y, OBJECT_DINAMITE);

    /// Places the Bombs on the map.
    placeObjects(NUMBER_OF_BLIN_ON_START, OBJECT_BLIN);

    /// Places the first-aid kits on the map.
    placeObjects(NUMBER_OF_MEDKIT_ON_START, OBJECT_MEDKIT);

    /// Places the mushrooms on the map.
    placeObjects(NUMBER_OF_MUSHROOMS_ON_START, OBJECT_MUSHROOMS);


    // Init dynamic objects

    /// Init bugs
    int BUG_START_COORD_X = W-2;
    int BUG_START_COORD_Y = 4;
    worldAddOneObjectBug(BUG_START_COORD_X, BUG_START_COORD_Y);
    worldAddOneObjectBug(BUG_START_COORD_X, BUG_START_COORD_Y+1);

    /// Initiates the press.
    const int PRESS_START_LOCATION_X = 12;
    const int PRESS_START_LOCATION_Y = 0;
    worldAddOneObjectPress(PRESS_START_LOCATION_X, PRESS_START_LOCATION_Y, 888881);

    /// Initiates ghosts.
    int GhostStartLocationX = 7;
    int GhostStartLocationY = H-2;
    for (int i = 0; i < 1; i++)
    {
        worldAddOneObjectGhost(GhostStartLocationX, GhostStartLocationY);
    }
}

bool _decodeStaticCodeAndSetMapValue(char staticObjectMapCode, int x, int y)
{
    bool isValid = true;

    // depending on the map code set the static object on a matrix
    switch(staticObjectMapCode)
    {
    case('-'):
        setMapValue(x, y, OBJECT_WALL);
        break;
    case('w'):
        setMapValue(x, y, OBJECT_WEAK_WALL);
        break;
     case('*'):
        setMapValue(x, y, OBJECT_TRAP);
        break;
    case('d'):
        setMapValue(x, y, OBJECT_DINAMITE);
        break;
    case('B'):
        setMapValue(x, y, OBJECT_BLIN);
        break;
    case('D'):
        setMapValue(x, y, OBJECT_DOOR);
        break;
    case('U'):
        setMapValue(x, y, OBJECT_GREEN_DOOR);
        break;
    case('N'):
        setMapValue(x, y, OBJECT_BLUE_DOOR);
        break;
    case('K'):
        setMapValue(x, y, OBJECT_KEY);
        break;
    case('Q'):
        setMapValue(x, y, OBJECT_GREEN_KEY);
        break;
    case('T'):
        setMapValue(x, y, OBJECT_BLUE_KEY);
        break;
    case('F'):
        setMapValue(x, y, OBJECT_FINISH);
        break;
    case('.'):
        setMapValue(x, y, OBJECT_EMPTY);
        break;
    case('A'):
        setMapValue(x, y, OBJECT_MEDKIT);
        break;
    case('H'):
        setMapValue(x, y, OBJECT_MAGIC_WELL);
        break;

    case('S'):
        WorldHeroStartLocationX = x;
        WorldHeroStartLocationY = y;
        setMapValue(x, y, OBJECT_EMPTY);
        break;

    case('M'):
        setMapValue(x, y, OBJECT_MUSHROOMS);
        break;

    case('L'):
        isValid = false;
        [[clang::fallthrough]];
    default:
        setMapValue(x, y, OBJECT_EMPTY);
        break;
    }
    return isValid;
}

bool _loadMapFromStaticData(std::list< std::string > &_vStaticMapData)
{
    int error_counter = 0;

    // Clear map
    clearMap();

    int y = 0;
    for (const auto &line : _vStaticMapData)
    {
        int x = 0;
        for (const char staticObjectMapCode : line)
        {
            if (!_decodeStaticCodeAndSetMapValue(staticObjectMapCode, x, y))
            {
                error_counter++;
            }
            x++;
        }

        y++;
    }

    if (error_counter != 0)
    {
        return false;
    }

    return true;
}

bool _decodeDynamicObjectAndAddToWorld(const std::string & line)
{
    char indicator = 0;

    if (line.empty())
    {
        return true;
    }
    // TODO: skip strings with whitespace

    int scan_result = sscanf(line.c_str(), "%c", &indicator);
    if (scan_result != 1)
    {
        return false;
    }
    //printf("DEBUG: Read indicator from map file <%c>\n", indicator);

    switch(indicator)
    {
    // Loading beetles
    case('J'):
    {
        int x, y;
        sscanf(line.c_str(), "%c-%d,%d", &indicator, &x, &y);
        bool place_result = worldAddOneObjectBug(x, y);
        if (place_result == true)
        {
            printf("DEBUG: Adding Bug in coords: %d, %d\n", x, y);
        }
        else
        {
            printf("Error: Unable to add Bug in coords: %d, %d\n", x, y);
            return false;
        }
        break;
    }

    // Press
    case('P'):
    {
        int x, y, phase;
        sscanf(line.c_str(), "%c-%d,%d-%d", &indicator, &x, &y, &phase);
        bool place_result = worldAddOneObjectPress(x, y, phase);
        if (place_result == true)
        {
            printf("DEBUG: Adding Press in coords: %d, %d. Phase = %d\n", x, y, phase);
        }
        else
        {
            printf("Error: Unable to add Press in coords: %d, %d. Phase = %d\n", x, y, phase);
            return false;
        }
        break;
    }

    case('G'):
    {
        int x, y;
        sscanf(line.c_str(), "%c-%d,%d", &indicator, &x, &y);

        bool place_result = worldAddOneObjectGhost(x, y);
        if (place_result == true)
        {
            printf("DEBUG: Adding Ghost in coords: %d, %d\n", x, y);
        }
        else
        {
            printf("   Error: Unable to add Ghost in coords: %d, %d\n", x, y);
            return false;
        }

        break;
    }

    case('8'):
    {
        int x, y;
        int cloud_w, cloud_h;
        sscanf(line.c_str(), "%c-%d,%d-%dx%d", &indicator, &x, &y, &cloud_w, &cloud_h);

        bool place_result = worldAddOneObjectCloud(x, y, cloud_w, cloud_h);
        if (place_result == true)
        {
            printf("DEBUG: Adding Cloud in coords: %d, %d\n", x, y);
        }
        else
        {
            printf("Error: Unable to add Cloud in coords: %d, %d\n", x, y);
            return false;
        }

        break;
    }

    case('R'):
    {
        int x, y;
        sscanf(line.c_str(), "%c-%d,%d", &indicator, &x, &y);

        bool place_result = worldAddOneObjectRobik(x, y);
        if (place_result == true)
        {
            printf("DEBUG: Adding Robik in coords: %d, %d\n", x, y);
        }
        else
        {
            printf("Error: Unable to add Robik in coords: %d, %d\n", x, y);
            return false;
        }

        break;
    }

    case('C'):
    {
        int x, y;
        sscanf(line.c_str(), "%c-%d,%d", &indicator, &x, &y);

        bool place_result = worldAddOneObjectCarrier(x, y);
        if (place_result == true)
        {
            printf("DEBUG: Adding Carrier in coords: %d, %d\n", x, y);
        }
        else
        {
            printf("Error: Unable to add Carrier in coords: %d, %d\n", x, y);
            return false;
        }

        break;
    }
#if 0
    case('I'):
    {
        int x, y;
        sscanf(line.c_str(), "%c-%d,%d", &indicator, &x, &y);

        bool place_result =  worldAddOneObjectInterceptors(x, y, 0);
        if (place_result == true)
        {
            printf("DEBUG: Adding Interceptors in coords: %d, %d\n", x, y);
        }
        else
        {
            printf("Error: Unable to add Interceptors in coords: %d, %d\n", x, y);
            return false;
        }

        break;
    }
#endif
    case('#'):
    {
        printf("DEBUG: Skipping commented out line\n");
        break;
    }

    default:
        printf("Error: Unknown dynamic object indicator <%c> found on map!\n", indicator);
        return false;
    }
    return true;
}

bool _loadMapDynamicObjects(std::list< std::string > &_vDynamicObjectCreationList)
{
    int error_counter = 0;
    for (const auto &line : _vDynamicObjectCreationList)
    {
        if (!_decodeDynamicObjectAndAddToWorld(line))
        {
            error_counter++;
        }
    }
    if (error_counter != 0)
    {
        return false;
    }

    return true;
}

/// Load the level objects (static and dynamic) from the file by level number.
bool loadMapFromFile(int loadableLevelNumber)
{
    // Opens the map file
    // Extracted height and width
    W = -1;
    H = -1;

    std::string full_path = worldGetFilePathForLevel(loadableLevelNumber);

    const char* cstrFullPath = full_path.c_str();

    // open the file
    std::ifstream infile(cstrFullPath);
    if (!infile.is_open())
    {
        printf("FAIL. Unable to load file <%s> for level %d\n", cstrFullPath, loadableLevelNumber);
        return false;
    }

    printf("DEBUG: Map is loaded from <%s>\n", cstrFullPath);

    std::string line;
    int line_index = 0;

    std::list< std::string > vStaticMapData;
    std::list< std::string > vDynamicObjectCreationList;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);

        if (line_index == 0)
        {
            // Level title
            m_currentLevelName = line;
        }
        else if (line_index == 1)
        {
            // Reads world dimensions from map
            iss >> W;
        }
        else if (line_index == 2)
        {
            // Reads world dimensions from map
            iss >> H;
        }
        else
        {
            if ((line_index >= 3) && (line_index < 3+H))
            {
                vStaticMapData.push_back(line);
            }
            else
            {
                // TODO: level loading flags or dynamic objects
                vDynamicObjectCreationList.push_back(line);
            }
        }

        line_index++;
    }

    infile.close();

    bool isValidStatic = _loadMapFromStaticData(vStaticMapData);
    if (!isValidStatic)
    {
        printf("Error: Loading static data from level\n");
        return false;
    }

    bool isValidDynamic = _loadMapDynamicObjects(vDynamicObjectCreationList);
    if (!isValidDynamic)
    {
        printf("Error: Loading dynamic data from level\n");
        return false;
    }

    m_level = loadableLevelNumber;

    return true;
}


/*!
     \brief initialize the game world

     Adjusts the game world and downloads static and dynamic objects
     the world. Adjusts the player in the initial coordinates of the level.

    \return true if successful.
 */
bool initWorld(const XGameSettings *pGameSettings)
{
    world_pGameSettings = pGameSettings;

    bool f_generateMap = false;

    m_levelCount = worldCountNumberOfLevels();
    printf("DEBUG: Number of levels total = %d\n", m_levelCount);

    if (m_levelCount == 0)
    {
        printf("ERROR: No levels found!\n");
        return false;
    }

    m_level = 1;

    if (f_generateMap)
    {
        generateMap();
    }
    else
    {
        bool isLoaded = loadMapFromFile(m_level);
        if (!isLoaded)
        {
            return false;
        }
    }

    /// Initiates the player
    hero.init(world_pGameSettings, WorldHeroStartLocationX, WorldHeroStartLocationY);
    return true;
}

/// Asking the world to add one Press to the map.
/// It initiates its coordinates and the number of the phase of movement (0-4).
bool worldAddOneObjectPress(int startX, int startY, int phaseNumber)
{
    for (int i = 0; i < WORLD_MAX_NUM_PRESS; i++)
    {
        if (!press[i].isInitialized())
        {
            press[i].init(startX, startY, phaseNumber);
            return true;
        }
    }
    return false;
}

/// Adds a ghost object to the map in the given coordinates (X, Y).
bool worldAddOneObjectGhost(int startX, int startY)
{
    for (int i = 0; i < WORLD_MAX_NUM_GHOST; i++)
    {
        if (!ghost[i].isInitialized())
        {
            ghost[i].init(startX, startY);
            return true;
        }
    }
    return false;
}


/// Adds one Cloud object to a map in the specified coordinates (X, Y).
bool worldAddOneObjectCloud(int startX, int startY, int cloudW, int cloudH)
{
    for (int i = 0; i < WORLD_MAX_NUM_CLOUD; i++)
    {
        if (!cloud[i].isInitialized())
        {
            cloud[i].init(startX, startY, cloudW, cloudH);
            return true;
        }
    }
    return false;
}


/// Adds one Bugs object to a map in the specified coordinates (X, Y).
bool worldAddOneObjectBug(int startX, int startY)
{
    for (int i = 0; i < WORLD_MAX_NUM_BUGS; i++)
    {
        if (!bugs[i].isInitialized())
        {
            bugs[i].init(startX, startY);
            return true;
        }
    }
    return false;
}

bool worldAddOneObjectRobik(int startX, int startY)
{
    for (int i = 0; i < WORLD_MAX_NUM_ROBIK; i++)
    {
        if (!robiks[i].isInitialized())
        {
            robiks[i].init(startX, startY);
            return true;
        }
    }
    return false;
}

/// Adds one Carrier object to a map in the specified coordinates (X, Y).
bool worldAddOneObjectCarrier(int startX, int startY)
{
    for (int i = 0; i < WORLD_MAX_NUM_CARRIER; i++)
    {
        if (!carrier[i].isInitialized())
        {
            carrier[i].init(startX, startY);
            return true;
        }
    }
    return false;
}

/// Adds one Interceptor object to a map in the specified coordinates (X, Y).
bool worldAddOneObjectInterceptors(int startX, int startY, XDynamicObject *pCarrierOwner)
{
    for (int i = 0; i < WORLD_MAX_NUM_INTERCEPTORS; i++)
    {
        if (!interceptors[i].isInitialized())
        {
            interceptors[i].init(startX, startY, pCarrierOwner);
            return true;
        }
    }
    return false;
}

/// Is the object in the trap area (input: ghost coordinates, output - trap coordinates).
/// If the object with coordinates oX, oY falls into the zone of at least one trap, then
/// return true, and still set trap coordinates (trapX, trapY).
/// checks on the map in coordinates X, Y.
/// A trap was found, but does the intersection of the action zone intersect?.
///if x, y are the coordinates of the trap, it is possible to measure the distance between the specimen-ghost (oX, oY) and the trap (x, y).
/// returns trap coordinates
bool worldIsObjectInZoneOfTrap(int oX, int oY, int &trapX, int &trapY)
{
    // to go through all coordinates, we will seek traps
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            int value = getMapValue(x, y); // Find out what's on the map in coordinates X, Y
            if (value == OBJECT_TRAP) // A trap was found, but does the intersection of the action zone intersect?
            {
                // then the coordinates x, y are the coordinates of the trap
                // it means you can measure the distance between the ghost object (oX, oY) and the trap (x, y)
                float distance = worldGetDistance (oX, oY, x, y);
                if (distance < 2.0)
                {
                    // so, the action zone intersects, return the trap coordinates
                    trapX = x;
                    trapY = y;
                    return true;
                }
            }
        }
    }

    // if we got to the end of the map, and we did not find a snap trap - that's "there is no snap trap"
    trapX = -1;
    trapY = -1;
    return false;
}

/// Each cycle of the game World takes its step. In this case, each object of the World makes its movements.
bool worldStep()
{
    // --- Press ------------------------
    /// the press makes its move
    for (int i = 0; i < WORLD_MAX_NUM_PRESS; i++)
    {
        press[i].stepAI();
    }

    int currentHeroX, currentHeroY;
    hero.getXY(currentHeroX, currentHeroY);

    for (int i = 0; i < WORLD_MAX_NUM_PRESS; i++)
    {
        if (press[i].isCollidedWithPress(currentHeroX, currentHeroY))
        {
            notifyHeroAboutDamage(PRESS_DAMAGE);  // Inform the Hero that he has been damaged
        }
    }

    // --- Ghost ------------------------
    /// the Ghost makes its move
    for (int i = 0; i < WORLD_MAX_NUM_GHOST; i++)
    {
        ghost[i].stepAI();
    }

    for (int i = 0; i < WORLD_MAX_NUM_GHOST; i++)
    {
        int ghostX, ghostY;
        ghost[i].getXY(ghostX, ghostY);


        float distance = worldGetDistance(currentHeroX, currentHeroY, ghostX, ghostY);
        bool isGhostShouldBeVisible1 = (distance <= 4.0f);

        int numberOfWalls = worldGetNumberOfWallsOnLine(currentHeroX, currentHeroY, ghostX, ghostY);
        bool isGhostShouldBeVisible2 = (numberOfWalls == 0);

        bool isGhostVisible = isGhostShouldBeVisible1 && isGhostShouldBeVisible2;

        ghost[i].setVisibility(isGhostVisible);

        //printf("*** Number of walls between hero and GHOST [%d] = %d\n\n", i, numberOfWalls);
    }

    for (int i = 0; i < WORLD_MAX_NUM_GHOST; i++)
    {
        if (ghost[i].isCollided(currentHeroX, currentHeroY))
        {
            notifyHeroAboutDamage(GHOST_DAMAGE);
        }
    }

    // --- Cloud ------------------------
    /// the Cloud makes its move
    for (int i = 0; i < WORLD_MAX_NUM_CLOUD; i++)
    {
        cloud[i].stepAI();
    }

    for (int i = 0; i < WORLD_MAX_NUM_CLOUD; i++)
    {
        if (cloud[i].isCollided(currentHeroX, currentHeroY))
        {
            notifyHeroAboutDamage(CLOUD_DAMAGE);
        }
    }

    // --- Bug ------------------------
    if (true)
    {
        /// the Bug makes its move
        for (int i = 0; i < WORLD_MAX_NUM_BUGS; i++)
        {
            bugs[i].stepAI();
        }
    }

    // --- Robik ------------------------
    if (true)
    {
        /// the Robik makes its move
        for (int i = 0; i < WORLD_MAX_NUM_ROBIK; i++)
        {
            robiks[i].stepAI();
        }
    }

    if (m_worldLevelIsComplete)
    {
        m_worldLevelIsComplete = false;

        /// Perform all necessary actions to move to a new level.
        bool result = worldProgressToNextLevel();
        if (!result)
        {
            printf("Error: World was scheduled for level update but unable to do so.\n");
            return false;
        }
    }

    // --- Carrier ------------------------
    /// the Carrier makes its move
    for (int i = 0; i < WORLD_MAX_NUM_CARRIER; i++)
    {
        carrier[i].stepAI();
    }

    for (int i = 0; i < WORLD_MAX_NUM_CARRIER; i++)
    {
        if (carrier[i].isCollided(currentHeroX, currentHeroY))
        {
            notifyHeroAboutDamage(CARRIER_DAMAGE);
        }
    }

    // --- Interceptors ------------------------
    /// Interceptors make their course
    for (int i = 0; i < WORLD_MAX_NUM_INTERCEPTORS; i++)
    {
        interceptors[i].stepAI();
    }

    for (int i = 0; i < WORLD_MAX_NUM_INTERCEPTORS; i++)
    {
        if (interceptors[i].isCollided(currentHeroX, currentHeroY))
        {
            notifyHeroAboutDamage(INTERCEPTORS_DAMAGE);
        }
    }
    return true;
}

/*!
    \brief Issues the coordinates of the first found static object of a given type on the map.
    Passes through all coordinates of the map, looks for the object and returns the coordinates of the first one found.
    \param[out] objectX  The coordinate of the X object found.
    \param[out] objectY  The coordinate of the Y object found.
    \param[in] ObjectType  Type of static object for map search.
    \return true if the object of the given type is found on the map and the returned coordinates are true, false - otherwise.
*/
bool worldGetFirstObjectLocation(int &objectX, int &objectY, int ObjectType)
{
    // Passes through all coordinates, looking for an object.
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            int value = getMapValue(x, y); // Finds what's on the map in coordinates X, Y.
            if (value == ObjectType) // The key is found -return true.
            {
                objectX = x;
                objectY = y;
                return true;
            }
        }
    }

    // Object not found - return false.
    objectX = -1;
    objectY = -1;
    return false;
}


/*!
    \brief Finds the key coordinates on the current (downloaded) map.

     reports the coordinates of the key on the map - used to control the Hero by Artificial Intelligence.
     Makes the correct values keyX, keyY.

    \param[out] x  Coordinate of the found X key.
    \param[out] y  Coordinate of the found Y key.
    \return true if the key is found on the map and the returned coordinate is true, false - if the key is not present.
 */
bool worldGetKeyLocation(int &x, int &y)
{
    // TODO: works only for Red Key
    return worldGetFirstObjectLocation(x, y, OBJECT_KEY);
}

///true - if the finish is on the map. It also displays the correct values of X, Y.
// false - if there is no finish.
bool worldGetFinishLocation(int &x, int &y)
{
    return worldGetFirstObjectLocation(x, y, OBJECT_FINISH);
}


bool isGameFinished = false;

/// !!! Signals section !!!
// Unique function of game mechanics.
// TODO: Find better location later
void signalGameFinished()
{
    isGameFinished = true;
}

/// Handle level completion - just set flag and next cycle it will be considered
void worldNotifyLevelComplete()
{
    m_worldLevelIsComplete = true;
}

/// Processor of the reaction of the World to the area of explosion in specific coordinates
void worldNotifyExplosionInArea(int exp_x, int exp_y)
{
    for (int i = 0; i < WORLD_MAX_NUM_BUGS; i++)
    {
        if (bugs[i].isInitialized() && (!bugs[i].isKilled()))
        {
            int bugX, bugY;
            bugs[i].getXY(bugX, bugY);
            if ((bugX == exp_x) && (bugY == exp_y))
            {
                bugs[i].kill();
            }
        }
    }

}

void signalDisplayGameStatistics()
{
    m_displayGameStatistics = true;
}


/// [Maintenance of the World].
void worldDestroyAllDynamicObjects()
{
    //XObjectPlayerHero hero;
    /*
    XObjectPress press[WORLD_MAX_NUM_PRESS];
    XObjectGhost ghost[WORLD_MAX_NUM_GHOST];
    XObjectCloud cloud[WORLD_MAX_NUM_CLOUD];
    XObjectBug bugs[WORLD_MAX_NUM_BUGS];
    XObjectRobik robiks[WORLD_MAX_NUM_ROBIK];
    */

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_PRESS; i++)
    {
        // Will the destructor be called ????
        press[i] = XObjectPress(); // we rewrite each object with an empty de-institutionalized object
    }

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_GHOST; i++)
    {
        ghost[i] = XObjectGhost();
    }

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_CLOUD; i++)
    {
        cloud[i] = XObjectCloud();
    }

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_BUGS; i++)
    {
        bugs[i] = XObjectBug();
    }

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_ROBIK; i++)
    {
        robiks[i] = XObjectRobik();
    }

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_CARRIER; i++)
    {
        carrier[i] = XObjectCarrier();
    }

    // -----
    for (int i = 0; i < WORLD_MAX_NUM_INTERCEPTORS; i++)
    {
        interceptors[i] = XObjectInterceptors();
    }
}

/// [Game Mechanics].
/// The function is responsible for the advent of a new level.
/// TODO: Describe in detail what this feature does.
bool worldProgressToNextLevel()
{
    signalDisplayGameStatistics();

    if (m_level >= m_levelCount)
    {
        // you need to finish the game with the message instead of uploading a new level
        signalGameFinished();
    }
    else
    {
        // Cleans dynamic objects.
        clearMap();
        worldDestroyAllDynamicObjects();

        m_level++;
        bool isLoaded = loadMapFromFile(m_level);
        if (!isLoaded)
        {
            // saveGame(); // TODO: add game saving here
            printf("Error: Next level could not be loaded.\n");
            return false;
        }

        // Reinstall the hero.
        hero.reinitForNextLevel(WorldHeroStartLocationX, WorldHeroStartLocationY);
    }
    return true;
}

std::string internal_GetFullFilePathForLevel_Dir(int loadableLevelNumber, const std::string &map_directory)
{
    std::string rFullFilePath;
    std::string map_filename;

    char buffer[500];
    sprintf(buffer, "map_level%d.txt", loadableLevelNumber);

    // Improve to maintain an arbitrary number of levels
    map_filename = std::string(buffer);

    rFullFilePath = map_directory + "/" + map_filename;
    return rFullFilePath;
}

/*!
 \brief [Download Resources] Generates the expected full path to the level file.
 Generates the expected full path to the level file.
 Check the path should be done separately.

 \param[in] loadableLevelNumber  The number of the level for which to generate the path.
 \return Returns the file path even if such a file does not exist.
 */
std::string worldGetFilePathForLevel(int loadableLevelNumber)
{
    // first run - identify if running from debug
    static bool is_debug_identification_complete = false;
    static int start_index_for_directory = -1;

    const std::string map_directory_normal("levels/");
    const std::string map_directory_debug("../../Sentry/levels/");

    const std::string *possible_directories[] = {&map_directory_normal, &map_directory_debug};

    if (!is_debug_identification_complete)
    {
        // identify mode - which directory index is valid?
        for (int i = 0; i < 2; i++)
        {
            std::string fullLevel1FilePath = internal_GetFullFilePathForLevel_Dir(1, *(possible_directories[i]));
            bool exists = isFileExists(fullLevel1FilePath);
            if (exists)
            {
                start_index_for_directory = i;
                break;
            }
        }

        is_debug_identification_complete = true;
    }

    if (start_index_for_directory == -1)
    {
        printf("DEBUG: error, levels not found!\n");
        exit(1);
    }

    const std::string &lookup_dir = *(possible_directories[start_index_for_directory]);
    std::string rFullFilePath = internal_GetFullFilePathForLevel_Dir(loadableLevelNumber, lookup_dir);

    return rFullFilePath;
}

std::string getCurrentLevelName()
{
    return m_currentLevelName;
}

///[Download Resources]. Determines whether a file exists.
/// \ return Returns true if the file exists.
bool isFileExists(std::string filepath)
{
    // open the file
    FILE * pFile = fopen(filepath.c_str(), "r");
    if (pFile == NULL)
    {
        return false;
    }
    fclose(pFile);
    return true;
}

/// [Download Resources]. Count the number of levels available to the game at the time of launch.
/// \ return Returns the number of levels, or 0, if levels are not found.
int worldCountNumberOfLevels()
{
    const int MAX_LEVEL = 99999;

    for (int i = 1; i <= MAX_LEVEL; i++)
    {
        std::string levelFilePath = worldGetFilePathForLevel(i);
        bool levelExists = isFileExists(levelFilePath);
        if (!levelExists)
        {
            return i-1;
        }
        else
        {
            continue;
        }
    }

    return MAX_LEVEL;
}
