#pragma once

#include "wave.h"

#include <xaudio2.h>
#include <hash_map>
#include <string>

using std::hash_map;
using std::string;

class Audio 
{
private:
    IXAudio2* g_engine;
    IXAudio2MasteringVoice* g_master;
    static hash_map<const string, Wave*>* assets;

    Wave* GetSound(const string& file_name);

public:
    Audio();
    ~Audio();

    void RegisterSound(const string& file_name);
    IXAudio2SourceVoice* PlaySound(const string& file_name);

    static const string step;
    static const string drop;
    static const string ledge;
    static const string potion;
    static const string spikes;
    static const string trigger;
    static const string gate_up;
    static const string get_sword;
    static const string gate_down;
    static const string gate_close;
    static const string draw_sword;
    static const string spike_kill;
    static const string platform_break;
    static const string ugh;
    static const string sword_hit_prince;
    static const string sword_hit_enemy;
    static const string sword_miss;
    static const string sword_parry;
    static const string title;
    static const string intro;
    static const string outro;
    static const string door_open;
};
