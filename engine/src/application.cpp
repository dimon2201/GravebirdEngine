// application.cpp

#include <iostream>
#include "application.hpp"
#include "engine.hpp"
#include "graphics.hpp"
#include "context.hpp"
#include "camera_manager.hpp"
#include "render_manager.hpp"
#include "render_context.hpp"
#include "sound_context.hpp"
#include "font_manager.hpp"
#include "sound_manager.hpp"
#include "filesystem_manager.hpp"
#include "physics_manager.hpp"
#include "gameobject_manager.hpp"
#include "texture_manager.hpp"
#include "memory_pool.hpp"
#include "event_manager.hpp"
#include "thread_manager.hpp"
#include "input.hpp"
#include "time.hpp"
#include "log.hpp"

using namespace types;

namespace realware
{
    iApplication::iApplication(cContext* context, const sEngineCapabilities* capabilities) : iObject(context)
    {
        _engine = _context->Create<cEngine>(_context, capabilities, this);
        _window = _context->Create<cWindow>(_context, capabilities->windowTitle, capabilities->windowWidth, capabilities->windowHeight);
    }

    iApplication::~iApplication()
    {
        _context->Destroy<cWindow>(_window);
        _context->Destroy<cEngine>(_engine);
    }
}