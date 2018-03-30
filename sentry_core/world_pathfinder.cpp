#include "pathfinder/pathfinder.h"

#include "world.h"

bool worldIsSolidObstacle(int value)
{
    bool isObstacle = false;

    if ((value == OBJECT_WALL) || (value == OBJECT_WEAK_WALL) ||
        (value == OBJECT_DOOR) || (value == OBJECT_GREEN_DOOR) ||
        (value == OBJECT_BLUE_DOOR))
    {
        isObstacle = true;
    }

    // TODO: check dynamic objects (e.g. Press) for obsturcting path

    return isObstacle;
}

bool worldGetAStarPath(int srcX, int srcY, int dstX, int dstY,
                       std::list< CoordXY > &rvPath,
                       float &rDistance,
                       bool allow_diagonal_moves)
{
    rvPath.clear();
    rDistance = 0;

    // TODO: make "allow_diagonal_moves" work
    int W;
    int H;
    getWorldDimensions(W, H);

    MAPDATA map;
    map.allocate(W, H);
    map.clear(0);

    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            int value = getMapValue(x, y);
            bool isObstacle = worldIsSolidObstacle(value);
            if (isObstacle)
            {
                map.setCost(x, y, CELL_WALL);
            }
            else
            {
                map.setCost(x, y, CELL_CLEAR);
            }
        }
    }

    // find route
    PATHFINDER pathfinder;
    pathfinder.init(&map);

    // intentionally swap dst and src points for pathfinder library
    bool result = pathfinder.find(dstX, dstY, srcX, srcY);
    if (!result)
    {
        return false;  // no path found
    }

    // Path is found - copy path data to return value
    std::list<CELL> pathList = pathfinder.pathList;

    int waypoint_index = -1;
    for (const auto & pathEntry : pathList)
    {
        waypoint_index++;
        if (waypoint_index == 0)
        {
            continue;  // skip source point from movement points
        }

        CoordXY new_path_point;
        new_path_point.x = pathEntry.ix;
        new_path_point.y = pathEntry.iy;

        rvPath.push_back(new_path_point);
    }

    // report calculated path length
    rDistance = pathfinder.getPathLength();

    return true;
}
