#include <SDL2/SDL.h>
#include <cassert>
#include <string>
#include <thread>

#include "ensivideo.hpp"

#include "oggstream.hpp"
#include "stream_common.hpp"

#include "ensitheora.hpp"

using namespace std;

unique_ptr<thread> theorathread;
unique_ptr<thread> vorbisthread;
unique_ptr<thread> displaythread;

int main(int argc, char *argv[]) {
  int res;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);//0 on success
  atexit(SDL_Quit);
  assert(res == 0);

  // Your code HERE
  // start the two stream readers (theoraStreamReader and vorbisStreamReader)
  // each in a thread
  theorathread = make_unique<thread>(theoraStreamReader, string(argv[1]));
  vorbisthread = make_unique<thread>(vorbisStreamReader, string(argv[1]));


  // wait for vorbis thread
  vorbisthread->join();
  // 1 seconde of sound in advance, thus wait 1 seconde
  // before leaving
  this_thread::sleep_for(1s);

  // Wait for theora and theora2sdl threads
  //TODO: Possible problem with this, wait for termination after ??
  theorathread.release();
  displaythread.release();//Imported from stream_common

  /* liberer des choses ? */
  //No need for the threads because we use unique pointers


  exit(EXIT_SUCCESS);
}
