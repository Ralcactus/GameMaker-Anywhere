#include <stdbool.h>
#include "shortcut_functions.h"

bool is_running3DS()
{
    #ifdef __3DS__
        return true;
    #else
        return false;
    #endif
}

bool is_runningRAY()
{
    #ifdef __RAYLIB__
        return true;
    #else
        return false;
    #endif
}

bool is_runningWiiU()
{
    #ifdef __WIIU__
        return true;
    #else
        return false;
    #endif
}
