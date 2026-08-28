#include <SDL3/SDL_image.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <sys/stat.h>
#include "renderer.hpp"
#include "meta.hpp"
using namespace std;

SDL_Window *window;
SDL_Renderer* renderer;

void DrawImage(float DrawX, float DrawY, const char* ImgPath){
    SDL_FRect pos = { DrawX, DrawY, 0, 0 };

    //Make the real path
    char fullPath[512];
    SDL_snprintf(fullPath, sizeof(fullPath), "%sResources/Images/%s", SDL_GetBasePath(), ImgPath);


    // load logos from file
    SDL_Surface *pizzy = IMG_Load(fullPath);
    SDL_Texture *texture = NULL;
    if (pizzy){
        pos.w = pizzy->w;
        pos.h = pizzy->h;
        texture = SDL_CreateTextureFromSurface(renderer, pizzy);
        SDL_DestroySurface(pizzy);
    }
    else{
        printf("ERROR COULDN'T FIND the image!!\n");
    }    

    SDL_RenderClear(renderer);

    if (texture)
        SDL_RenderTexture(renderer, texture, NULL, &pos);

    SDL_RenderPresent(renderer);
}

void Renderer_Init(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Hello Mario", 1366, 768, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, NULL);

    //Window couldn't be created
    if (window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    }

    char RuntimeFolder[512];
    SDL_snprintf(RuntimeFolder, sizeof(RuntimeFolder), "%sRuntime", SDL_GetBasePath());

    struct stat sb;
    if (stat(RuntimeFolder, &sb) != 0){
        char ERROR_MES[512];
        SDL_snprintf(ERROR_MES, sizeof(RuntimeFolder), "%s\nWAS NOT FOUND! PLEASE RE-DOWNLOAD THE APP!", RuntimeFolder);
        ShowError(ERROR_MES);
        ExitRenderer();
        exit(1);
    }
    else
        printf("Runtime folder found!\n");
}

void ExitRenderer(){
    SDL_DestroyWindow(window);
}