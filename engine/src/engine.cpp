// engine.cpp

#include "engine.hpp"
#include "context.hpp"
#include "gameobject_manager.hpp"

namespace realware
{
	cEngine::cEngine(cContext* context) : iObject(context) {}

	void cEngine::Initialize()
	{
		_context->CreateMemoryPool();

		_context->RegisterFactory<cGameObject>();
	}
}