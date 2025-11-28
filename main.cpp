#include <iostream>
#include "player.h"
#include "room.h"

int main(){

    Player hero;
    MapManager mapT;
    Room* current_room;

    mapT.loadMap("Rooms.txt");
    current_room = mapT.getRoom(4);

    std::cout << current_room->info;

}