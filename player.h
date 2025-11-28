#pragma once
#include <string>
#include <iostream>

class Item;
class Weapon;
class Armor;

class Player {
private:
    int max_hp;
    int hp;
    int atk;
    int exp;
    int lvl;
    int gold;
    int current_room;
    Weapon* equippedWeapon;
    Armor* equippedArmor;

    const int MAX_LVL = 5;

    const int lvl_need[5] = {20, 45, 75, 120, 180};
    const int hp_table[5] = {40, 65, 100, 150, 230};
    const int base_atk[5] = {5, 9, 15, 22, 30};

    void lvlUp() {
        if (lvl < MAX_LVL) {
            lvl++;
            max_hp = hp_table[lvl - 1];
            atk = base_atk[lvl - 1];
            std::cout << ">>> TEBRIKLER! SEVIYE " << lvl << " OLDUNUZ! <<<" << std::endl;
        }
    }

public:
    Player() {
        lvl = 1;
        exp = 0;
        gold = 17;
        max_hp = hp_table[0];
        hp = max_hp;
        atk = base_atk[0];
        current_room = 0;
        equippedWeapon = nullptr;
        equippedArmor = nullptr;
    }

    int getHp() { return hp; }
    int getMaxHp() { return max_hp; }
    int getAtk() { return atk; }
    int getLvl() { return lvl; }
    int getExp() { return exp; }
    int getGold() { return gold; }

    void addExp(int amount) {
        if (lvl >= MAX_LVL) return;

        exp += amount;

        while (lvl < MAX_LVL && exp >= lvl_need[lvl - 1]) {
            exp -= lvl_need[lvl - 1];
            lvlUp();
        }
    }

    void roomChange(int room){
        current_room = room;
    }
   
    bool isAlive() const {
        return hp <= 0;
    }

    void goldChange(int amount){
        gold = gold + amount;
        if (gold < 0)
        {
            gold = 0;
        }
    }

    void heal(int amount){
        hp = hp + amount;
        if (hp > max_hp)
        {
            hp = max_hp;
        }
    }

    void hurt(int amount){
        hp = hp - amount;
        if (hp < 0)
        {
            hp = 0;
        }
    }

    void buff(int amount){
        atk = atk + amount;
    }

    void printStats() {
        std::cout << "--- Status ---" << std::endl;
        std::cout << "Hp: " << hp << "/" << max_hp << std::endl;
        std::cout << "Strength: " << atk << std::endl;
        std::cout << "Level: " << lvl << (lvl == MAX_LVL ? " (MAX)" : "") << std::endl;
        std::cout << "Gold: " << gold << std::endl;

        if (lvl < MAX_LVL)
            std::cout << "XP: " << exp << " / " << lvl_need[lvl - 1] << std::endl;
        else
            std::cout << "XP: -" << std::endl;

        std::cout << "-------------" << std::endl;
    }
};