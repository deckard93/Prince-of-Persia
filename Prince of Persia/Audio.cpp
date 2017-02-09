#include "Audio.h"
#include <cassert>


const string Audio::ugh = "ugh.wav";
const string Audio::step = "step.wav";
const string Audio::drop = "drop.wav";
const string Audio::ledge = "ledge.wav";
const string Audio::potion = "potion.wav";
const string Audio::spikes = "spikes.wav";
const string Audio::gate_up = "gate_up.wav";
const string Audio::trigger = "trigger.wav";
const string Audio::gate_down = "gate_down.wav";
const string Audio::get_sword = "get_sword.wav";
const string Audio::sword_hit_prince = "sword_hit_prince.wav";
const string Audio::sword_hit_enemy = "sword_hit_enemy.wav";
const string Audio::sword_miss = "sword_miss.wav";
const string Audio::sword_parry = "sword_parry.wav";
const string Audio::draw_sword = "draw_sword.wav";
const string Audio::gate_close = "gate_close.wav";
const string Audio::spike_kill = "spike_kill.wav";
const string Audio::title = "title.wav";
const string Audio::platform_break = "platform_break.wav";
const string Audio::intro = "intro.wav";
const string Audio::outro = "outro.wav";
const string Audio::door_open = "door_open.wav";

hash_map<const string, Wave*>* Audio::assets = new hash_map<const string, Wave*>();


Wave* Audio::GetSound(const string& file_name)
{
    assert(assets->find(file_name) != assets->end());
    return (*assets)[file_name];
}

Audio::Audio()
{
    // Must initialize COM for XAudit2
    // Note: probably not the best practice to have this here
    // TODO
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // Create the engine
    if (FAILED(XAudio2Create(&g_engine))) {
        //Throw
    }

    // Create the mastering voice
    if (FAILED(g_engine->CreateMasteringVoice(&g_master))) {
        g_engine->Release();
        //Throw
    }
}

void Audio::RegisterSound(const string& file_name) 
{    
    Wave* buffer = new Wave();

    // Load a wave file
    if (!buffer->load(file_name.c_str())) {
        g_engine->Release();
        throw std::runtime_error(string("Error reading audio file"));
    }
    assets->insert(make_pair(file_name, buffer));
}

IXAudio2SourceVoice* Audio::PlaySound(const string& file_name)
{
    Wave* buffer = GetSound(file_name);
    IXAudio2SourceVoice* g_source;
    if (FAILED(g_engine->CreateSourceVoice(&g_source, buffer->wf()))) {
        throw("Error creating source voice");
    }
    
    g_source->Start();
    g_source->SubmitSourceBuffer(buffer->xaBuffer());
    return g_source;
}

Audio::~Audio() 
{
    // Release the engine, NOT the voices!
    g_engine->Release();
    // COM clean up
    CoUninitialize();
}