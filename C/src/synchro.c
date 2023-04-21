#include "ensitheora.h"
#include "synchro.h"
#include "pthread.h"

/* les variables pour la synchro, ici */
pthread_mutex_t mtxTaille , mtxFenetre, mtxTexture;
pthread_cond_t condTaille, condFenetre, condCons, condProd;
bool changedTaille = false;
bool changedTexture = false;

int emptyCases = NBTEX;

/* l'implantation des fonctions de synchro ici */
//Decodeur
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&mtxTaille);
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    changedTaille = true; //Used in the loop Because no Hoare signal
    pthread_cond_signal(&condTaille);
    pthread_mutex_unlock(&mtxTaille);
}

//Afficheur
void attendreTailleFenetre() {
    pthread_mutex_lock(&mtxTaille);
    while (!changedTaille){
        pthread_cond_wait(&condTaille, &mtxTaille);
    }
    changedTaille = false;
    pthread_mutex_unlock(&mtxTaille);
}
//------------------------------
//Decodeur
void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&mtxFenetre);
    changedTexture = true;
    pthread_cond_signal(&condFenetre);
    pthread_mutex_unlock(&mtxFenetre);
}

//Afficheur
void attendreFenetreTexture() {
    pthread_mutex_lock(&mtxFenetre);
    while (!changedTexture) {
        pthread_cond_wait(&condFenetre, &mtxFenetre);
    }
    changedTexture = false;
    pthread_mutex_lock(&mtxFenetre);
}
//---------------------------------
void debutConsommerTexture() {
    pthread_mutex_lock(&mtxTexture);
    while(emptyCases == NBTEX){
        pthread_cond_wait(&condCons, &mtxTexture);
    }
    pthread_mutex_unlock(&mtxTexture);
}

void finConsommerTexture() {
    pthread_mutex_lock(&mtxTexture);
    emptyCases++;
    pthread_cond_signal(&condProd);
    pthread_mutex_unlock(&mtxTexture);
}

void debutDeposerTexture() {
    pthread_mutex_lock(&mtxTexture);
    while(emptyCases == 0){
        pthread_cond_wait(&condProd, &mtxTexture);
    }
    pthread_mutex_unlock(&mtxTexture);

}

void finDeposerTexture() {
    pthread_mutex_lock(&mtxTexture);
    emptyCases--;
    pthread_cond_signal(&condCons);
    pthread_mutex_unlock(&mtxTexture);
}
