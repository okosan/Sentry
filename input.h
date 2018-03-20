#ifndef INPUT_H
#define INPUT_H


const int ACTION_DO_NOTHING           = 1;  //  action "do nothing"
const int ACTION_UP                   = 2;  //  action "Move Up"
const int ACTION_DOWN                 = 3;  //  action "Move Down"
const int ACTION_LEFT                 = 4;  //  action "Move Left"
const int ACTION_RIGHT                = 5;  //  action "Move Right"
const int ACTION_DROP_KEY             = 6;  //  action "Drop Red Key at current position"
const int ACTION_DROP_TRAP            = 7;  //  action "Drop Trap at current position"
const int ACTION_TRADE                = 8;  //  action "Trade with Robik"

const int ACTION_TOGGLE_INVINCIBLE    = 9;  //  the action for switching of the invulnerability mode
const int ACTION_FORCE_LEVEL_COMPLETE = 10; //  instantly win the current level
const int ACTION_DROP_DINAMITE        = 11; //  dynamite release
const int ACTION_EXPLODE              = 12; //  detonate dropped dynamite (in sequential order, one at a time)
const int ACTION_REMOVE_SHIELD        = 13; //  make Hero to the Ghostly state
const int ACTION_HEAL_SHIELD          = 14; //  Heals 10 Shield units
const int ACTION_TERMINATE_GAME       = 15; //  Instantly terminate game

void initInput();

void readKeyboardInput();

void readInput();

int inputGetLastAction();


#if 0
bool isMovingInDirectionOfObject(int inputAction, int objectType)
{
    bool is_moving_in_direction_of_object = false;

    int objectX, objectY;
    bool objectExists = false;

    switch(objectType)
    {
    case(OBJECT_KEY):
        objectExists = worldGetKeyLocation(objectX, objectY);
        break;
    case(OBJECT_FINISH):
        objectExists = worldGetFinishLocation(objectX, objectY);
        break;
    default:
        printf("Error! I dont know how to move to that object!\n");
    }

    if (objectExists == false)
    {
        is_moving_in_direction_of_object = false;
    }
    else
    {
        int copyEx = Ex;
        int copyEy = Ey;

        switch(inputAction)
        {
        case(ACTION_LEFT):
            copyEx = copyEx - 1;
            break;

        case(ACTION_RIGHT):
            copyEx = copyEx + 1;
            break;

        case(ACTION_UP):
            copyEy = copyEy - 1;
            break;

        case(ACTION_DOWN):
            copyEy = copyEy + 1;
            break;
        }

        float len_to_object_before_move = worldGetDistance(objectX, objectY, Ex, Ey);
        float len_to_object_after_move = worldGetDistance(objectX, objectY, copyEx, copyEy);

        bool is_moving_away = false;
        if (len_to_object_after_move > len_to_object_before_move)
        {
            is_moving_away = true;
        }

        if (is_moving_away)
        {
            is_moving_in_direction_of_object = false;
        }
        else
        {
            is_moving_in_direction_of_object = true;
        }
    }

    return is_moving_in_direction_of_object;
}

void readAI_input()
{
    Sleep(250);

    int random_direction = rand() % 4; // 0 .. 3
    int action_random = ACTION_UP + random_direction;


    int toWhichObjectWeShallMove = OBJECT_KEY;
    if (E_has_key)
    {
        toWhichObjectWeShallMove = OBJECT_FINISH;
    }

    if (isMovingInDirectionOfObject(action_random, toWhichObjectWeShallMove))
    {
        // confirm move
    }
    else
    {
        // discard move
        // 33% chance to move in wrong direction
        int random_number = rand() % 3; // 0, 1, 2

        bool canMoveInWrongDirection = false;
        if (random_number == 0) // 25% chance
        {
            canMoveInWrongDirection = true;
        }

        if (canMoveInWrongDirection)
        {
            // confirm move
        }
        else
        {
            action_random = ACTION_DO_NOTHING;
        }
    }

    lastAction = action_random;
}
#endif


#endif // INPUT_H
