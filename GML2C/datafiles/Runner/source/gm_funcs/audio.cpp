#include "audio.h"

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

int audio_play_sound(int soundid, int priority, bool loop){
    return 0;
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

