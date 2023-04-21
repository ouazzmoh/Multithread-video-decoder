#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.h"
#include <stdbool.h>
/* Les extern des variables pour la synchro ici */
extern pthread_mutex_t mtxTaille , mtxFenetre, mtxTexture;
extern pthread_cond_t condTaille, condFenetre, condProd, condCons;
extern bool fini;



/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
