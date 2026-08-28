#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "compiler/compiler_main.hpp"
#include "helpers/renderer.hpp"
#include <SDL3/SDL_messagebox.h>

bool running = true;

int main(int argc, char* argv[]){
    //Init the renderer
    Renderer_Init();

    //Check the keystate
    const bool *keystate = SDL_GetKeyboardState(NULL);
    bool EscapePress = false;

    //Silly :3 (up here instead of in the loop because it lowkey tries to load every frame gulp!)
    DrawImage(50, 50, "pizzy.png");

    printf("Press space to compile!\n");
    while (running){
        SDL_Event event;

        //Close window
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT){
                running = false;
            }
        }

        //Actually run the compiler
        if (keystate[SDL_SCANCODE_SPACE] && !EscapePress){
            RunCompiler();
        }
        EscapePress = keystate[SDL_SCANCODE_SPACE];
    }



    //Goodbye friend!!
    ExitRenderer();
    SDL_Quit();
    return 0;
}