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
    MapManager mapT;

public:
    Game() {
        cout << "Starting the game..." << endl;
    }

    void processLoot(Monster* mob) {
        
        if (mob->getGold() > 0) {
            hero.goldChange(mob->getGold());
            cout << "Ganimet: " << mob->getGold() << " altin kazandin!" << endl;
        }

        const vector<int>& drops = mob->getLootList();

        if (drops.empty()) return;
        
        for (int itemID : drops) {
            Item* newItem = itemMgr.getItem(itemID);
            
            if (newItem == nullptr) continue;

            cout << ">> Dusen Esya: " << newItem->getName() << endl;

            // Ekleme basariliysa (Canta bos veya yer var) siradaki esyaya gec
            if (hero.addItem(newItem)) {
                continue; 
            }

            // --- CANTA DOLU KRİZ YÖNETİMİ ---
            char choice;
            cout << "Cantan dolu! " << newItem->getName() << " icin yer acmak ister misin? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                bool itemDeleted = false; // Basarili silme bayragi
                
                while (!itemDeleted) {
                    hero.printInventory();
                    
                    int slotToDelete;
                    cout << "Atilacak esyanin numarasi (Vazgecmek icin -1): ";
                    cin >> slotToDelete;

                    // Vazgecme
                    if (slotToDelete == -1) {
                        cout << newItem->getName() << " yerde birakildi." << std::endl;
                        delete newItem; // Yerdekini sil (RAM temizligi)
                        break; 
                    }

                    // KRITIK NOKTA: Player::removeItem artik BOOL donuyor!
                    // Eger "KeyItem" secersek false donecek ve silemeyecek.
                    if (hero.removeItem(slotToDelete)) {
                        // Silme basarili, yer acildi
                        itemDeleted = true; 
                        hero.addItem(newItem); // Simdi ekle
                        cout << "Takas basarili!" << endl;
                    }
                    else {
                        // removeItem false donerse (KeyItem ise)
                        // Dongu basa sarar ve tekrar sorar.
                        cout << "Baska bir esya secin." << endl;
                    }
                }
            } 
            else {
                // Oyuncu istemedi
                cout << newItem->getName() << " yerde birakildi." << endl;
                delete newItem;
            }
        }
    }


    void run() {
        cout << "--- TESTING ---" << endl;

        Monster* testMob = mobMgr.getEnemy(5);
        testMob->addLoot(101); 

        cout << "Karsina " << testMob->getName() << " cikti ve onu yendin!" << endl;
        
        processLoot(testMob);
        delete testMob;
        
        cout << "\nSon Durum:" << endl;
        hero.printInventory();
    }
};