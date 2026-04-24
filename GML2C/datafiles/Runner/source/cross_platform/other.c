#include <stdio.h>
#include <string.h>

//3ds
#ifdef __3DS__
    #include <3ds.h>

    void ExitApplication()
    {
        romfsExit();
        gfxExit();
    }

#endif

//wii u
#ifdef __gamecube__

    void ExitApplication()
    {
        //so empty...
    }

#endif