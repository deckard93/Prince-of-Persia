#pragma once

#include "wave.h"

#include <xaudio2.h>
#include <hash_map>
#include <string>

using std::hash_map;
using std::string;

class Audio {
private:
	IXAudio2* g_engine;
	IXAudio2MasteringVoice* g_master;
	static hash_map<const string, Wave*>* assets;

	Wave* GetSound(const string& file_name);

public:
	Audio();
	~Audio();

	void RegisterSound(const string& file_name);
	void PlaySound(const string& file_name);

	static const string step;
	static const string drop;
};
