#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

// Oyuncunun secebilecegi cevap secenegi
struct DialogueOption {
    string text;     // Ekranda gorunecek yazi: "Nasılsın?"
    int nextNodeID;  // Bu secilirse hangi ID'li cevaba gidilecek? (-1 ise konusma biter)

    DialogueOption(string t, int next) : text(t), nextNodeID(next) {}
};

// NPC'nin tek bir konusma balonu
struct DialogueNode {
    int id;
    string npcText; // NPC'nin soyledigi: "Merhaba yabanci."
    vector<DialogueOption> options; // Oyuncunun secebilecegi cevaplar listesi

    DialogueNode(int _id = 0, string _text = "") : id(_id), npcText(_text) {}
};

class NPC {
private:
    int id;             // NPC'nin numarasi (50, 51 vb.)
    string name;        // NPC'nin Adi (Yasli Bilge)
    bool metBefore;     // Daha once karsilastik mi? (Evet/Hayir)
    
    // Anahtar (Key): Node ID -> Deger (Value): DialogueNode
    map<int, DialogueNode> dialogueTree; 

public:
    // --- (Constructor) ---
    NPC(int _id, string _name) : id(_id), name(_name) {
        metBefore = false;
    }

    // --- GETTER / SETTER ---
    string getName() const { return name; }
    int getID() const { return id; }
    bool hasMet() const { return metBefore; }
    void setMet(bool status) { metBefore = status; }


    // --- DIYALOG YONETIM FONKSIYONLARI ---

    // 1. NPC'ye yeni bir cumle ogretir (Yonetici bunu kullanacak)
    void addDialogueNode(int nodeID, string text) {
        dialogueTree[nodeID] = DialogueNode(nodeID, text);
    }

    // 2. Var olan bir cumleye cevap secenegi ekler
    void addOption(int nodeID, string optionText, int nextNodeID) {
        // Once bu ID'de bir konusma var mi bakalim?
        if (dialogueTree.find(nodeID) != dialogueTree.end()) {
            dialogueTree[nodeID].options.push_back(DialogueOption(optionText, nextNodeID));
        } else {
            // Hata ayiklama icin (Eger ID yanlis girildiyse)
            cout << "HATA: NPC " << name << " icin Node ID " << nodeID << " bulunamadi!" << endl;
        }
    }

    // 3. Oyun sirasinda istenilen cumleyi getirir
    // Game.h icinde: npc->getDialogue(0) dedigimizde ilk konusmayi dondurur.
    DialogueNode* getDialogue(int nodeID) {
        if (dialogueTree.find(nodeID) != dialogueTree.end()) {
            return &dialogueTree[nodeID];
        }
        return nullptr; // Eger boyle bir konusma yoksa bos doner
    }
};