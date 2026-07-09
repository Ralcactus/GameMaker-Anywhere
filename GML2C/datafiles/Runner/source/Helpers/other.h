#include <stdio.h>
#include <string.h>
#include "../variable_handler.h"
#include <variant>
#include <vector>

void ExitApplication();

#ifdef __3DS__
    #include <opusfile.h>
    #include <3ds.h>
    extern LightEvent s_event;

    void audioInit();
    bool fillBuffer(OggOpusFile *opusFile_, ndspWaveBuf *waveBuf_);
    void audioCallback(void *const nul_);
    void audioThread(void *const opusFile_);
#endif