// audio.cpp

#include "audio.hpp"
#include "sound_context.hpp"

namespace realware
{
	cAudio::cAudio(cContext* context) : iObject(context) {}

	cAudio::~cAudio()
	{
		if (api == API::NONE)
		{
			Print("Error: sound API not selected!");

			return;
		}
		else if (api == API::OPENAL)
		{
			if (_api)
				delete _api;
		}
	}

	void cAudio::SetAPI(API api)
	{
		if (api == API::NONE)
		{
			Print("Error: sound API not selected!");

			return;
		}
		else if (api == API::OAL)
		{
			_api = new cOpenALSoundAPI(_context);
		}
	}
}