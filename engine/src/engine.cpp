// engine.cpp

#include "engine.hpp"
#include "context.hpp"
#include "graphics.hpp"
#include "camera_manager.hpp"
#include "texture_manager.hpp"
#include "filesystem_manager.hpp"
#include "font_manager.hpp"
#include "physics_manager.hpp"
#include "time.hpp"
#include "event_manager.hpp"
#include "gameobject_manager.hpp"
#include "thread_manager.hpp"
#include "render_context.hpp"
#include "audio.hpp"

namespace realware
{
	cEngine::cEngine(cContext* context) : iObject(context) {}

	void cEngine::Initialize()
	{
		// Create memory pool
		_context->CreateMemoryPool();

		// Register factories
		_context->RegisterFactory<cWindow>();
		_context->RegisterFactory<cBuffer>();
		_context->RegisterFactory<cShader>();
		_context->RegisterFactory<cTexture>();
		_context->RegisterFactory<cRenderTarget>();
		_context->RegisterFactory<cRenderPass>();

		// Register subsystems
		_context->RegisterSubsystem(new cGraphics(_context));
		_context->RegisterSubsystem(new cCamera(_context));
		_context->RegisterSubsystem(new cTextureAtlas(_context));
		_context->RegisterSubsystem(new cFileSystem(_context));
		_context->RegisterSubsystem(new cFont(_context));
		_context->RegisterSubsystem(new cPhysics(_context));
		_context->RegisterSubsystem(new cGameObject(_context));
		_context->RegisterSubsystem(new cThread(_context));
		_context->RegisterSubsystem(new cTime(_context));
		_context->RegisterSubsystem(new cEventDispatcher(_context));
		_context->RegisterSubsystem(new cAudio(_context));

		// Create application window
		cGraphics* graphics = _context->GetSubsystem<cGraphics>();
		graphics->SetAPI(cGraphics::API::OPENGL);
		graphics->SetWindow(640, 480);

		// Create texture manager
		cTextureAtlas* texture = _context->GetSubsystem<cTextureAtlas>();
		texture->SetAtlas(glm::vec3(2048, 2048, 16));

		// Create sound context
		cAudio* audio = _context->GetSubsystem<cAudio>();
		audio->SetAPI(cAudio::API::OAL);
	}
}