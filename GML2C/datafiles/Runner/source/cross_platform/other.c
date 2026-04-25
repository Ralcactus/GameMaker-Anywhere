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

//gamecube
#ifdef __gamecube__

    void ExitApplication()
    {
        //so empty...
    }

#endif

//wii
#ifdef __wii__

    void ExitApplication()
    {
        //so empty...
    }

#endif