// gameobject_manager.cpp

#include "application.hpp"
#include "gameobject_manager.hpp"
#include "render_manager.hpp"
#include "physics_manager.hpp"
#include "memory_pool.hpp"

using namespace types;

namespace realware
{
    cGameObject::cGameObject(const std::string& id, cApplication* app, cMemoryPool* memoryPool) : cIdVecObject(id, app)
    {
        sTransform* pTransform = (sTransform*)(memoryPool->Allocate(sizeof(sTransform)));
        _transform = new (pTransform) sTransform();
    }

    void cGameObject::SetPhysicsActor(eCategory staticOrDynamic, eCategory shapeType, cPhysicsSimulationScene* scene, cPhysicsSubstance* substance, f32 mass)
    {
        mPhysics* physics = _app->GetPhysicsManager();
        _actor = physics->CreateActor(
            GetID(),
            staticOrDynamic,
            shapeType,
            scene,
            substance,
            mass,
            _transform,
            this
        );
    }

    void cGameObject::SetPhysicsController(f32 eyeHeight, f32 height, f32 radius, const glm::vec3& up, cPhysicsSimulationScene* scene, cPhysicsSubstance* substance)
    {
        mPhysics* physics = _app->GetPhysicsManager();
        _controller = physics->CreateController(
            GetID(),
            eyeHeight,
            height,
            radius,
            _transform,
            up,
            scene,
            substance
        );
    }

    mGameObject::mGameObject(cApplication* app) : _app(app), _maxGameObjectCount(app->GetDesc()->_maxGameObjectCount), _gameObjects(_app, _maxGameObjectCount)
    {
    }

    cGameObject* mGameObject::CreateGameObject(const std::string& id)
    {
        return _gameObjects.Add(id, _app->GetMemoryPool());
    }

    cGameObject* mGameObject::FindGameObject(const std::string& id)
    {
        return _gameObjects.Find(id);
    }

    void mGameObject::DestroyGameObject(const std::string& id)
    {
        _gameObjects.Delete(id);
    }
}