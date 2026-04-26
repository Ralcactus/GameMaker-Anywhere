#include <stdio.h>
#include <string.h>

//3ds
#ifdef __3DS__
    #include <3ds.h>

    FILE* loadDataRomfs(const char* filename){
        romfsInit();

        char path[256];
        snprintf(path, sizeof(path), "romfs:/%s", filename);
        return fopen(path, "rb");
    }

#endif

//wii u
#ifdef __WIIU__

    FILE* loadDataRomfs(const char* filename){
        char path[256];
        snprintf(path, sizeof(path), "/vol/content/%s", filename);
        return fopen(path, "rb");
    }

#endif