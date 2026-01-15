// event_manager.cpp

#pragma once

#include "application.hpp"
#include "gameobject_manager.hpp"
#include "event_manager.hpp"
#include "buffer.hpp"

using namespace types;

namespace realware
{
    cEventHandler::cEventHandler(eEventType type, cGameObject* receiver, EventFunction&& function) : _receiver(receiver), _type(type), _function(std::make_shared<EventFunction>(std::move(function))) {}

    void cEventHandler::Invoke(cDataBuffer* data)
    {
        _function->operator()(data);
    };

    cEventDispatcher::cEventDispatcher(cContext* context) : iObject(context) {}

    void cEventDispatcher::Unsubscribe(eEventType type, cGameObject* receiver)
    {
        if (_listeners.find(type) == _listeners.end())
            return;

        auto& events = _listeners[type];
        for (usize i = 0; i < events->GetElementCount(); i++)
        {
            const cEventHandler* listenerEvent = &events->GetElements()[i];
            const cGameObject* listenerReceiver = listenerEvent->GetReceiver();
            if (listenerReceiver == receiver)
            {
                events->Delete(listenerReceiver->GetIdentifier()->GetID());
                return;
            }
        }
    }

    void cEventDispatcher::Send(eEventType type)
    {
        cDataBuffer data(_context);

        Send(type, &data);
    }

    void cEventDispatcher::Send(eEventType type, cDataBuffer* data)
    {
        if (_listeners.find(type) == _listeners.end())
            return;

        auto& events = _listeners[type];
        for (usize i = 0; i < events->GetElementCount(); i++)
            events->GetElements()[i].Invoke(data);
    }
}