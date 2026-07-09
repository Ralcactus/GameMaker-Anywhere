#include <stdio.h>
#include <string.h>
#include "../variable_handler.h"
#include <variant>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../gm_funcs/audio.h"

//3ds
#ifdef __3DS__
    #include <opusfile.h>
    #include <3ds.h>

    #define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
    LightEvent s_event;
    int16_t *s_audioBuffer = NULL;
    ndspWaveBuf s_waveBufs[3];



    void audioInit(){
        // Setup NDSP
        ndspChnReset(0);
        ndspSetOutputMode(NDSP_OUTPUT_STEREO);
        ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);
        ndspChnSetRate(0, 48000);
        ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);

        // Allocate audio buffer
        const size_t bufferSize = 48000 * 120 / 1000 * 2 * sizeof(int16_t) * ARRAY_SIZE(s_waveBufs);
        s_audioBuffer = (int16_t *)linearAlloc(bufferSize);
        // Setup waveBufs for NDSP
        memset(&s_waveBufs, 0, sizeof(s_waveBufs));
        int16_t *buffer = s_audioBuffer;

        for(size_t i = 0; i < ARRAY_SIZE(s_waveBufs); ++i) {
            s_waveBufs[i].data_vaddr = buffer;
            s_waveBufs[i].status     = NDSP_WBUF_DONE;
            buffer += 48000 * 120 / 1000 * 2 * sizeof(int16_t) / sizeof(buffer[0]);
        }
    }

    bool fillBuffer(OggOpusFile *opusFile_, ndspWaveBuf *waveBuf_) {
        // Decode samples until our waveBuf is full
        int totalSamples = 0;

        //Load the audio file
        while(totalSamples < 48000 * 120 / 1000){
            int16_t *buffer = waveBuf_->data_pcm16 + (totalSamples * 2);
            const size_t bufferSize = (48000 * 120 / 1000 - totalSamples) * 2;

            // Decode bufferSize samples from opusFile_ into buffer,
            // storing the number of samples that were decoded (or error)
            const int samples = op_read_stereo(opusFile_, buffer, bufferSize);
            if(samples <= 0) {
                break;
            }
            
            totalSamples += samples;
        }

        // If no samples were read in the last decode cycle, we're done
        if(totalSamples == 0) {
            printf("Audio finished playing\n");
            return false;
        }

        // Pass samples to NDSP
        waveBuf_->nsamples = totalSamples;
        ndspChnWaveBufAdd(0, waveBuf_);
        DSP_FlushDataCache(waveBuf_->data_pcm16,
            totalSamples * 2 * sizeof(int16_t));


        return true;
    }

    void audioThread(void *const opusFile_) {
        OggOpusFile *const opusFile = (OggOpusFile *)opusFile_;

        while(true) {
            for(size_t i = 0; i < ARRAY_SIZE(s_waveBufs); ++i) {
                if(s_waveBufs[i].status != NDSP_WBUF_DONE) {
                    continue;
                }
                
                if(!fillBuffer(opusFile, &s_waveBufs[i])) {   // Playback complete
                    return;
                }
            }

            // Wait for a signal that we're needed again before continuing,
            // so that we can yield to other things that want to run
            // (Note that the 3DS uses cooperative threading)
            LightEvent_Wait(&s_event);
        }
    }

    void audioCallback(void *const nul_) {
        (void)nul_;  // Unused

        if(false) { // Quit flag
            return;
        }
        
        LightEvent_Signal(&s_event);
    }


    void ExitApplication()
    {
        //Audio cleanup
        LightEvent_Signal(&s_event);
        threadJoin(threadId, UINT64_MAX);
        threadFree(threadId);
        ndspChnReset(0);
        linearFree(s_audioBuffer);
        ndspExit();
        op_free(opusFile);

        //Other cleanup
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
