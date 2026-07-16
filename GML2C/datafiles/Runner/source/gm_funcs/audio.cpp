#include "audio.h"
#include "../helpers/other.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <misc.h>

#ifdef __3DS__
    #include <opusfile.h>
    #include <3ds.h>

    OggOpusFile *opusFile = NULL;
    Thread threadId = NULL;
#endif

#if defined(__gamecube__) || defined(__wii__)
    #include <gccore.h>
    #include <asndlib.h>

    #include "../oggplayer.h"
    #include "../include_audio.h"
#endif

void audio_listener_position(float x, float y, float z){
    //so empty...
}

void audio_listener_velocity(float x, float y, float z){
    //so empty...
}

void audio_listener_orientation(float lookat_x, float lookat_y, float lookat_z, float up_x, float up_y, float up_z){
    //so empty...
}

void audio_emitter_create(){
    //so empty...
}

void audio_emitter_free(int emitter_id){
    //so empty...
}

bool audio_emitter_exists(int emitter_id){
    return false;
}

float audio_emitter_get_x(int emitter_id){
    return 0;
}

float audio_emitter_get_y(int emitter_id){
    return 0;
}

float audio_emitter_get_z(int emitter_id){
    return 0;
}

void audio_emitter_position(int emitter_id, float x, float y, float z){
    //so empty...
}

float audio_emitter_get_vx(int emitter_id){
    return 0;
}

float audio_emitter_get_vy(int emitter_id){
    return 0;
}

float audio_emitter_get_vz(int emitter_id){
    return 0;
}

void audio_emitter_velocity(int emitter_id, float vx, float vy, float vz){
    //so empty...
}

void audio_emitter_falloff(int emitter_id, float falloff_ref, float falloff_max, float falloff_factor){
    //so empty...
}

float audio_emitter_get_gain(int emitter_id){
    return 0;
}

void audio_emitter_gain(int emitter_id, float gain){
    //so empty...
}

float audio_emitter_get_pitch(int emitter_id){
    return 0;
}

float audio_emitter_get_listener_mask(int emitter_id){
    return 0;
}

void audio_emitter_set_listener_mask(int emitter_id, int mask){
    //so empty...
}

void audio_emitter_get_bus(int emitter_id, int bus_id){
    //so empty...
}

void audio_emitter_bus(int emitter_id, int bus_id){
    //so empty...
}

int audio_play_sound_on(int emitter_id, int soundid, bool loop, float priority, float gain, float offset, float pitch, float listener_mask){
    return 0;
}

int audio_play_sound_at(int soundid, float x, float y, float z, float falloff_ref, float falloff_max, float falloff_factor, bool loop, int priority, float gain, float offset, float pitch, float listener_mask){
    return 0;
}

int audio_play_sound_ext(float WHATHOW_I_DONT_KNOW_WHERE_HUHH){
    return 0;
}



#ifdef __3DS__
    int audio_play_sound(int soundid, int priority, bool loop){
        int error = 0;
        opusFile = op_open_file(("romfs:/audio/" + std::to_string(soundid) + ".opus").c_str(), &error);
        
        if (error != 0 || opusFile == NULL) {
            printf("Failed to open opus file: %d\n", error);
            return -1;
        }

        ndspSetCallback(audioCallback, NULL);
        int32_t priority_BLEH = 0x30;
        svcGetThreadPriority(&priority_BLEH, CUR_THREAD_HANDLE);
        priority_BLEH -= 1;
        priority_BLEH = priority_BLEH < 0x18 ? 0x18 : priority_BLEH;
        priority_BLEH = priority_BLEH > 0x3F ? 0x3F : priority_BLEH;
        threadId = threadCreate(audioThread, opusFile, 32 * 1024, priority_BLEH, -1, false);

        return soundid;
    }
#endif

#if defined(__gamecube__) || defined(__wii__)
    int audio_play_sound(int soundid, int priority, bool loop){

        if(ogg_refs[soundid] == NULL || ogg_refs_size[soundid] == NULL) {
            printf("Sound ID %d not found or size is NULL\n", soundid);
            return -1;
        }

        if(StatusOgg() == OGG_STATUS_RUNNING) {
            StopOgg();
        }
        
        if(StatusOgg() != OGG_STATUS_RUNNING){
            PlayOgg(ogg_refs[soundid], ogg_refs_size[soundid], 0, loop ? OGG_INFINITE_TIME : OGG_ONE_TIME);
        }

        return soundid;
    }
#endif
