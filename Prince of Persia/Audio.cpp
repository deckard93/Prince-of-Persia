#include "Audio.h"
#include <cassert>

const string Audio::step = "step.wav";
const string Audio::drop = "drop.wav";

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

void Audio::PlaySound(const string& file_name)
{
	Wave* buffer = GetSound(file_name);
	IXAudio2SourceVoice* g_source;
	if (FAILED(g_engine->CreateSourceVoice(&g_source, buffer->wf()))) {
		throw("Error creating source voice");
	}
	g_source->Start();
	g_source->SubmitSourceBuffer(buffer->xaBuffer());
}

Audio::~Audio() 
{
	// Release the engine, NOT the voices!
	g_engine->Release();
	// COM clean up for XAudio2
	CoUninitialize();
}