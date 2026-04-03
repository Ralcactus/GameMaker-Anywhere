#include <stdio.h>
#include <string.h>

//3DS
#ifdef __3DS__
    #include <3ds.h>

    //funcs
    void ExitApplication();
#endif

//WIIU
#ifdef __WIIU__
    //funcs
    void ExitApplication();
#endif
