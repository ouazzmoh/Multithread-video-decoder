#include <mutex>
#include <condition_variable>


#include "synchro.hpp"
#include "ensitheora.hpp"

using namespace std;
/* les variables pour la synchro, ici */
//First 4 functions
mutex mtxTaille;
mutex mtxFenetre;
condition_variable condTaille;
condition_variable condFenetre;
bool changedTaille = false;
//Last 4 functions
mutex mtxTexture;
condition_variable condCons; condition_variable condProd;

int idk = 1; //TODO: What number should be ? Most likely depends of tex_iaff and/or tex_iwri

/* l'implantation des fonctions de synchro ici */
//Decodeur
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    unique_lock<mutex> lockTaille(mtxTaille); // Block of code should be in mutual exclusion
    //Edits
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    //
    changedTaille = true; //Used in the loop in attendreTailleFenetre
    lockTaille.unlock();
    condTaille.notify_one();
}

//Afficheur
void attendreTailleFenetre() {
    unique_lock<mutex> lockTaille(mtxTaille);
    while (!changedTaille){
        condTaille.wait(lockTaille);
    }
    changedTaille = false;
    lockTaille.unlock();
}

//Decodeur
void signalerFenetreEtTexturePrete() {
    condFenetre.notify_one();
}


//Afficheur
void attendreFenetreTexture() {
    unique_lock<mutex> lockFenetre(mtxFenetre);
    condFenetre.wait(lockFenetre);
    lockFenetre.unlock();
}





void debutConsommerTexture() {
    unique_lock<mutex> lockTexture(mtxTexture);
    while(idk  == 0){
        condCons.wait(lockTexture);
    }
    lockTexture.unlock();
}

void finConsommerTexture() {
    condProd.notify_one();
}



void debutDeposerTexture() {
    unique_lock<mutex> lockTexture(mtxTexture);
    while(idk  == NBTEX){
        condProd.wait(lockTexture);
    }
    lockTexture.unlock();
}

void finDeposerTexture() {
    condCons.notify_one();
}

