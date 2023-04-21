#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "oggstream.h"
#include "synchro.h"
#include "stream_common.h"

pthread_t theorathread, vorbisthread;

int main(int argc, char *argv[]) {
  int res;
  pthread_mutex_init(&mapMtx, NULL);
  pthread_mutex_init(&mtxTaille, NULL);
  pthread_mutex_init(&mtxFenetre, NULL);
  pthread_mutex_init(&mtxTexture, NULL);
  pthread_cond_init(&condTaille, NULL);
  pthread_cond_init(&condFenetre, NULL);
  pthread_cond_init(&condProd, NULL);
  pthread_cond_init(&condCons, NULL);

  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);//Returns 0 on success
  atexit(SDL_Quit);
  assert(res == 0);

  // Your code HERE
  // start the two stream readers (theoraStreamReader and vorbisStreamReader)
  pthread_create(&theorathread, NULL, theoraStreamReader, (void *)argv[1]);
//  pthread_join(theorathread, NULL);
  pthread_create(&vorbisthread, NULL, vorbisStreamReader, (void *)argv[1]);
  // each in a thread

  // wait for vorbis thread
  pthread_join(vorbisthread, NULL);

  // 1 seconde of sound in advance, thus wait 1 seconde
  // before leaving
  sleep(1);

  // Wait for theora and theora2sdl threads
  pthread_cancel(theorathread);
  pthread_cancel(theora2sdlthread);

  // TODO
  /* liberer des choses ? */
  pthread_mutex_destroy(&mapMtx);
  pthread_mutex_destroy(&mtxTaille);
  pthread_mutex_destroy(&mtxFenetre);
  pthread_mutex_destroy(&mtxTexture);
  pthread_cond_destroy(&condTaille);
  pthread_cond_destroy(&condFenetre);
  pthread_cond_destroy(&condProd);
  pthread_cond_destroy(&condCons);

  exit(EXIT_SUCCESS);
}
