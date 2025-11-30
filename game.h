#pragma once
#include <iostream>
#include <vector>

#include "player.h"
#include "ItemManager.h"
#include "enemyManager.h"
#include "room.h"


using namespace std;

class Game {
private:
    Player hero;
    ItemManager itemMgr;
    EnemyManager mobMgr;
    MapManager mapMgr;

    Room* currentRoom;
public:
    Game() {
        cout << "Starting the game..." << endl;
        mapMgr.loadMap("Rooms.txt");

        currentRoom = mapMgr.getRoom(1);
        
        if (currentRoom == nullptr) {
            cout << "HATA: Baslangic odasi bulunamadi!" << endl;
        }
    }

    void processLoot(Monster* mob) {
        
        if (mob->getGold() > 0) {
            hero.goldChange(mob->getGold());
            cout << "Ganimet: " << mob->getGold() << " altin kazandin!" << endl;
        }

        const vector<int>& drops = mob->getLootList();

        for (int itemID : drops) {
            Item* newItem = itemMgr.getItem(itemID);
            
            if (newItem == nullptr) continue;

            cout << ">> Dusen Esya: " << newItem->getName() << endl;

            if (hero.addItem(newItem)) {
                continue; 
            }

            char choice;
            cout << "Cantan dolu! " << newItem->getName() << " icin yer acmak ister misin? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                bool itemDeleted = false;
                
                while (!itemDeleted) {
                    hero.printInventory();
                    
                    int slotToDelete;
                    cout << "Atilacak esyanin numarasi (Vazgecmek icin -1): ";
                    cin >> slotToDelete;

                    if (slotToDelete == -1) {
                        cout << newItem->getName() << " yerde birakildi." << std::endl;
                        delete newItem; 
                        break; 
                    }

                    if (hero.removeItem(slotToDelete)) {
                        itemDeleted = true; 
                        hero.addItem(newItem); 
                        cout << "Takas basarili!" << endl;
                    }
                    else {
                        cout << "Baska bir esya secin." << endl;
                    }
                }
            } 
            else {
                cout << newItem->getName() << " yerde birakildi." << endl;
                delete newItem;
            }
        }
    }


void run() {}
};