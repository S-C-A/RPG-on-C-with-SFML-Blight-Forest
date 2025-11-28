#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>  // Dosya okumak icin
#include <sstream>  // String parcalamak (Parsing) icin
using namespace std;

struct Room {
    int id;                 // Odanin kimligi (Orn: 5)
    std::string info;       // Odanin hikayesi ("Karanlik bir zindandasiniz...")
    
    // Yonler: Hangi yonde hangi oda var?
    // -1 degeri "O yonde kapi yok / Duvar var" anlamina gelir.
    int n, s, e, w;         // North, South, East, West

    // --- GELECEKTE EKLENECEKLER (Simdilik yorum satirinda kalsin) ---
    // bool hasEnemy;       // Odada dusman var mi?
    // int enemyID;         // Hangi dusman? (Goblin mi Ejderha mi?)
    // int itemID;          // Yerde bir esya var mi?

    // Constructor (Kurucu) - Odayi hizlica olusturmak icin
    Room(int _id = 0, std::string _info = "", int _n = -1, int _s = -1, int _e = -1, int _w = -1) {
        id = _id;
        info = _info;
        n = _n; s = _s; e = _e; w = _w;
    }
};

class MapManager {
private:

    std::map<int, Room> rooms; 

public:

    void loadMap(string filename) {
        ifstream file(filename);
        if (!file.is_open()) cout << "File couldn't be opened.";
    
        string line;

        while(getline(file,line))
        {
            stringstream ss(line);
            string segment;

            int id, n, s, e, w;
            string info;

            if (getline(ss, segment, ',')) id = stoi(segment);
            if(getline(ss, segment, ',')) info = segment;
            if(getline(ss, segment, ',')) n = stoi(segment); 
            if(getline(ss, segment, ',')) s = stoi(segment); 
            if(getline(ss, segment, ',')) e = stoi(segment); 
            if(getline(ss, segment, ',')) w = stoi(segment); 

            Room newRoom(id, info, n, s, e, w);
            rooms[id] = newRoom;
        }
        
        file.close();
        cout << "Harita yuklendi. Toplam oda: " << rooms.size() << endl;
    }

    Room* getRoom(int id) {
        if (rooms.find(id) != rooms.end()) {
            return &rooms[id];
        }
        return nullptr;
    }
};