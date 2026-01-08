#pragma once
#include <iostream>
#include <vector>
#include "player.h"
#include "monster.h"

using namespace std;

class CombatManager {
public:
    bool startBattle(Player* hero, std::vector<Monster*>& enemies) {
        
        for (auto n : enemies) cout << n->getName() << " blocks your path!";    
        cout << "\n";
        
        bool isBattleOver = false;
        bool playerWon = false;

        while (!isBattleOver) {     

            // --- (Player Turn) ---
            bool playerTurn = false;

            while (!playerTurn)
            {
                int choice;
                cout << "Your turn! 1: Attack | 2: Item \n";
                cin >> choice;

                switch (choice)
                {
                case 1:
                { 
                    int target;
                    int i = 1;
                    cout << "Select target! ";
                    for (auto n : enemies) cout << i++ << ": " << n->getName() << " | ";  
                    cout << "9: Back \n";
                    cout << "Choose target: ";
                    cin >> target;

                    while(target != 9 && (target < 1 || target > enemies.size())){
                        cout << "Invalid choice! Select again: ";
                        cin >> target;
                    } 

                    if (target != 9)
                    {
                        playerTurn = true;
                        int damage = hero->getAtk() - enemies[target-1]->getDef();
                        if (damage < 0) damage = 0;

                        enemies[target-1]->takeDamage(damage);
                        
                        cout << "Hit! Damage: " << damage << endl;
                        
                        if (enemies[target-1]->isDead()) {
                            cout << enemies[target-1]->getName() << " defeated!" << endl;

                            delete enemies[target-1]; 
                            enemies.erase(enemies.begin() + (target - 1));

                            if (enemies.empty()) {
                                isBattleOver = true;
                                playerWon = true;
                                break; 
                            }
                        }
                    }
                    break;
                }
                
                case 2:
        {
            int itemIndex = 0;
            bool validItemSelected = false;

            while (!validItemSelected) {

                // Listing the inventory
                const auto& inv = hero->getInventory();
                if (hero->getInventory().empty()) {
                    cout << "Inventory is empty!\n";
                    break; 
                }
                else { 
                    for (int k = 0; k < inv.size(); k++) {
                        cout << k + 1 << ": " << inv[k]->getName() << "\n";
                    }
                }
                
                cout << "\n--- Select an item to use (-1 to Back) ---\n";      
                cout << "Choose item: ";
                cin >> itemIndex;

                if (itemIndex == -1) {
                    cout << "Returning...\n";
                    break; 
                }

                if (itemIndex < 1 || itemIndex > inv.size()) {
                    cout << "Invalid choice! Try again.\n";
                    continue;
                }

                Item* selectedItem = inv[itemIndex - 1];

                Consumable* consumableItem = dynamic_cast<Consumable*>(selectedItem);

                if (consumableItem != nullptr) {
                    consumableItem->use(hero); 
                    
                    cout << selectedItem->getName() << " used.\n";
                    hero->removeItem(itemIndex - 1);

                    validItemSelected = true;
                    playerTurn = true;
                } 
                else {
                    cout << "You cannot use that item in combat!\n";
                }
            }
            break;
            }
                    
                default:
                    cout << "Invalid command.\n";
                    break;
                }
            }
            // --- (Enemy Turn) ---
            
            if (!isBattleOver) {
                    
                cout << "\n--- Enemy Turn ---\n";
                    
                for (auto enemy : enemies) {
                        
                    enemy->makeMove(hero);

                    if (hero->getHp() <= 0) {
                        cout << "\n>>> You died... " << enemy->getName() << " has slain you." << endl;
                        
                        isBattleOver = true;
                        playerWon = false;
                        break; 
                    }
                }
                cout << "------------------\n";
            }
            // --- Status Effects ---
            if (!isBattleOver) {
                hero->updateStatus();
                
                // Eger zehirden/kanamadan olursek:
                if (hero->getHp() <= 0) {
                     cout << "\n>>> Your wounds kill you..." << endl;
                     isBattleOver = true;
                     playerWon = false;
                }
            }
        }
        return playerWon;
    }
};