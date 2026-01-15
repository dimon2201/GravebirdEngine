// application.cpp

#include <iostream>
#include <GLFW/glfw3.h>
#include "application.hpp"
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
#include "time.hpp"
#include "log.hpp"

using namespace types;

namespace realware
{
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        cApplication* app = (cApplication*)glfwGetWindowUserPointer(window);
        key &= app->K_KEY_BUFFER_MASK;

        if (action == GLFW_PRESS)
            app->SetKey(key, K_TRUE);
        else if (action == GLFW_RELEASE)
            app->SetKey(key, K_FALSE);
    }

    void WindowFocusCallback(GLFWwindow* window, int focused)
    {
        cApplication* app = (cApplication*)glfwGetWindowUserPointer(window);

        if (focused)
        {
            if (app->GetWindowFocus() == K_FALSE)
                app->SetWindowFocus(K_TRUE);
        }
        else
        {
            app->SetWindowFocus(K_FALSE);
        }
    }

    void WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        cApplication* app = (cApplication*)glfwGetWindowUserPointer(window);

        app->_desc._windowDesc._width = width;
        app->_desc._windowDesc._height = height;
            
        app->_render->ResizeWindow(glm::vec2(width, height));
    }

    void CursorCallback(GLFWwindow* window, double xpos, double ypos)
    {
        cApplication* app = (cApplication*)glfwGetWindowUserPointer(window);

        app->SetCursorPosition(glm::vec2(xpos, ypos));
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        cApplication* app = (cApplication*)glfwGetWindowUserPointer(window);

        if (action == GLFW_RELEASE)
            app->SetMouseKey(button, 0);
        else if (action == GLFW_PRESS)
            app->SetMouseKey(button, 1);
    }

    iApplication::iApplication(cContext* context) : iObject(context)
    {
        _engine = std::make_shared<cEngine>(_context);
    }

    iApplication::~iApplication()
    {
    }

    void iApplication::Run()
    {
        _engine->Initialize();

        Setup();

        auto gfx = _context->GetSubsystem<cGraphics>();
        auto camera = _context->GetSubsystem<cCamera>();
        auto time = _context->GetSubsystem<cTime>();
        auto physics = _context->GetSubsystem<cPhysics>();

        time->BeginFrame();

        while (GetRunState() == K_FALSE)
        {
            time->Update();
            physics->Simulate();
            camera->Update();
            gfx->CompositeFinal();
            gfx->SwapBuffers();

            glfwPollEvents();
        }

        time->EndFrame();

        Stop();
    }
}