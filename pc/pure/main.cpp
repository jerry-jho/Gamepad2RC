#include <iostream>
#include <Windows.h>
#include <SDL2/SDL.h>
#undef main

int main(int argc, char *argv[]) {
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
    std::cerr << "SDL_INIT_JOYSTICK is fail" <<std::endl;
    return -1;
  }
  int count = SDL_NumJoysticks();
  if (count != 1) {
    std::cerr << "No of Multiple Joystick Found" <<std::endl;
  }
  auto g_pJoy = SDL_JoystickOpen(0);
  if (g_pJoy != NULL) {
    SDL_JoystickEventState(SDL_ENABLE);
  } else {
    std::cerr << "SDL_JoystickOpen is fail" <<std::endl;
  }
  std::cout << "# INFO: "<<SDL_JoystickName(g_pJoy) <<std::endl;


  SDL_Event test_event;
  for(;;) {
    //std::cout << "# INFO: wait event" <<std::endl;
    while (SDL_WaitEvent(&test_event)) {
        switch (test_event.type) {
            case SDL_JOYAXISMOTION:
                printf("We got a SDL_JOYAXISMOTION event.\n");
                break;
            default:
                printf("Unhandled Event!\n");
                break;           
        }
    }
    SDL_Delay(10);
  }
  std::cout << "# INFO: quit" <<std::endl;
  return 0;
}