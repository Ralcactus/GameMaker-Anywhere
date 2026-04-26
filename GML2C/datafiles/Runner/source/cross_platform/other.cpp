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

//gamecube and wii
#if defined(__gamecube__) || defined(__wii__)
    void ExitApplication()
    {
        //so empty...
    }
#endif
