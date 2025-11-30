#include <iostream>
#include "player.h"
#include "room.h"
#include "item.h"
#include "ItemManager.h"
#include "condition.h"
#include "game.h"

int main(){

    Player hero;
    MapManager mapT;
    ItemManager itemT;
    Room* current_room;
    Game rpg;

    rpg.run();

}