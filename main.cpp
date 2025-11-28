#include <iostream>
#include "player.h"
#include "room.h"
#include "item.h"
#include "ItemManager.h"

int main(){

    Player hero;
    MapManager mapT;
    ItemManager itemT;
    Room* current_room;

    Item* wooden_sword = itemT.getItem(101);
    Item* potion = itemT.getItem(1);

    if (wooden_sword != nullptr) {
        hero.addItem(wooden_sword);
    }
    if (potion != nullptr) {
        hero.addItem(potion);
    }

    mapT.loadMap("Rooms.txt");
    current_room = mapT.getRoom(4);

    hero.printInventory();

    std::cout << current_room->info;
}