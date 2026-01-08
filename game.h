#pragma once
#include <iostream>
#include <vector>
#include <ctime>

#include "player.h"
#include "ItemManager.h"
#include "enemyManager.h"
#include "room.h"
#include "combat.h" // 1. Combat dosyasini ekledik

using namespace std;

class Game {
private:
    Player hero;
    ItemManager itemMgr;
    EnemyManager mobMgr;
    MapManager mapMgr;
    CombatManager combatMgr; // 2. CombatManager nesnesini olusturduk

    Room* currentRoom;

public:
    Game() {
        cout << "Preparing Game Engine..." << endl;
        
        mapMgr.loadMap("Rooms.txt");

        // Baslangic odasi ID: 1
        currentRoom = mapMgr.getRoom(1);
        
        if (currentRoom == nullptr) {
            cout << "ERROR: Starting room not found! (ID: 1)" << endl;
        }
    }

    // --- MAIN GAME LOOP ---
    void run() {
        if (currentRoom == nullptr) return;

        cout << "--- GAME STARTED ---\n" << endl;
        bool isRunning = true;

        while (isRunning) {
            
            // --- OTOMATIK SAVAS KONTROLU ---
            // Eger odada bir canavar varsa, oyuncuya hareket hakki tanima.
            // Direkt savasi baslat.
            if (currentRoom->monsterID != -1) {
                Monster* enemy = mobMgr.getEnemy(currentRoom->monsterID);
                
                if (enemy != nullptr) {
                    // Dusmani bir grup vektorune koy (CombatManager vector istiyor)
                    vector<Monster*> battleGroup;
                    battleGroup.push_back(enemy);

                    // Savasi Baslat
                    // startBattle true donerse kazandik, false donerse olduk.
                    bool win = combatMgr.startBattle(&hero, battleGroup);

                    if (win) {
                        cout << "\n*** VICTORY ***" << endl;
                        
                        // Odadan canavar ID'sini sil (Artik oda temiz)
                        currentRoom->monsterID = -1;
                        
                        // NOT: CombatManager icinde 'delete enemy' yaptigimiz icin
                        // burada tekrar 'delete' yapmiyoruz veya 'processLoot' cagirmiyoruz.
                        // Loot sistemini ileride CombatManager'a tasimamis gerekebilir.
                    } 
                    else {
                        cout << "\n*** GAME OVER ***" << endl;
                        isRunning = false; // Oyunu kapat
                        continue; // Dongunun geri kalanini yapma
                    }
                }
            }

            // 1. ODA BILGISINI GOSTER
            cout << "\n--------------------------------" << endl;
            cout << "LOCATION: " << currentRoom->info << endl;
            
            // A. Yerde Esya Var mi?
            if (currentRoom->itemID != -1) {
                Item* tempItem = itemMgr.getItem(currentRoom->itemID);
                if (tempItem) {
                    cout << "GROUND: There is a [" << tempItem->getName() << "] here." << endl;
                    delete tempItem; 
                }
            }
            
            cout << "--------------------------------" << endl;

            // 2. KOMUT AL (Sadece savas yoksa buraya duser)
            cout << "What will you do? (w,a,s,d: Move | i: Inventory | e: Interact | q: Quit): ";
            char input;
            cin >> input;

            // 3. KOMUTU ISLE
            switch (input) {
                case 'w': move(currentRoom->n); break;
                case 's': move(currentRoom->s); break;
                case 'd': move(currentRoom->e); break;
                case 'a': move(currentRoom->w); break;
                
                case 'i': 
                    hero.printInventory(); 
                    break;
                
                case 'e':
                    interactWithRoom();
                    break;

                case 'q': 
                    cout << "Exiting game..." << endl;
                    isRunning = false;
                    break;

                default: cout << "Invalid command." << endl;
            }
        }
    }

    // --- HELPER FUNCTIONS ---

    // Hareket Mantigi
    void move(int roomID) {
        if (roomID == -1) {
            cout << "You can't go that way (Wall)." << endl;
        } else {
            Room* nextRoom = mapMgr.getRoom(roomID);
            if (nextRoom != nullptr) {
                currentRoom = nextRoom;
                cout << "Walking..." << endl;
            } else {
                cout << "ERROR: Room not found (ID: " << roomID << ")" << endl;
            }
        }
    }

    // Etkilesim Mantigi (Sadece ESYALAR icin kaldi)
    // Savas artik 'run' dongusunde otomatik basliyor.
    void interactWithRoom() {
        
        // Savas mantigi buradan kaldirildi cunku otomatik hale geldi.
        
        // ESYA TOPLAMA
        if (currentRoom->itemID != -1) {
            Item* item = itemMgr.getItem(currentRoom->itemID);
            if (item) {
                cout << ">> Trying to pick up " << item->getName() << "..." << endl;
                
                if (hero.addItem(item)) {
                    // Basarili alindiysa yerden sil
                    currentRoom->itemID = -1; 
                    cout << "You picked up the " << item->getName() << "." << endl;
                } else {
                    cout << "Inventory full! Could not pick up item." << endl;
                    delete item; 
                }
            }
        } else {
            cout << "Nothing to interact with here." << endl;
        }
    }

    // LOOT SISTEMI
    // (Simdilik kullanim disi kaldi cunku CombatManager dusmani siliyor.
    // Ileride loot sistemini CombatManager icine tasimamiz gerekecek.)
    void processLoot(Monster* mob) {
        if (mob->getGold() > 0) {
            hero.goldChange(mob->getGold());
            cout << "Loot: Found " << mob->getGold() << " gold!" << endl;
        }
        // ... Loot kodunun geri kalani ayni ...
    }
};