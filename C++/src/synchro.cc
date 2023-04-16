#include <mutex>
#include <condition_variable>


#include "synchro.hpp"
#include "ensitheora.hpp"

using namespace std;
/* les variables pour la synchro, ici */
mutex mtxTaille;
mutex mtxFenetre;
condition_variable condTaille;
condition_variable condFenetre;
bool changedTaille = false;

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





void debutConsommerTexture() {}

void finConsommerTexture() {}

void debutDeposerTexture() {}

void finDeposerTexture() {}

