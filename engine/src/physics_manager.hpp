// physics_manager.hpp

#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include "../../thirdparty/glm/glm/glm.hpp"
#include "category.hpp"
#include "id_vec.hpp"
#include "log.hpp"

namespace physx
{
    class PxActor;
    class PxRigidDynamic;
    class PxShape;
    class PxMaterial;
    class PxJoint;
    class PxScene;
    class PxFoundation;
    class PxPhysics;
}

namespace realware
{
    class cApplication;
    class cGameObject;
    struct sVertexBufferGeometry;
    struct sTransform;

    class cPhysicsAllocator : public physx::PxAllocatorCallback
    {
        virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override final
        {
            return malloc(size);
        }

        virtual void deallocate(void* ptr) override final
        {
            free(ptr);
        }
    };

    class cPhysicsError : public physx::PxErrorCallback
    {
        virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override final
        {
            Print(message);
        }
    };

    class cPhysicsCPUDispatcher : public physx::PxCpuDispatcher
    {
        virtual void submitTask(physx::PxBaseTask& task) override final
        {
            task.run();
            task.release();
        }

        virtual uint32_t getWorkerCount() const override final
        {
            return 0;
        }
    };

    class cPhysicsSimulationEvent : public physx::PxSimulationEventCallback
    {
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override final {}
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override final {}
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override final {}
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override final {}
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override final {}
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override final {}
    };

    class cPhysicsSimulationScene : public cIdVecObject
    {
    public:
        explicit cPhysicsSimulationScene(const std::string& id, const cApplication* const app, const physx::PxScene* const scene, const physx::PxControllerManager* const controllerManager) : cIdVecObject(id, app), _scene((physx::PxScene*)scene), _controllerManager((physx::PxControllerManager*)controllerManager) {}
        ~cPhysicsSimulationScene() = default;

        inline physx::PxScene* GetScene() const { return _scene; }
        inline physx::PxControllerManager* GetControllerManager() const { return _controllerManager; }

    private:
        physx::PxScene* _scene = nullptr;
        physx::PxControllerManager* _controllerManager = nullptr;
    };

    class cPhysicsSubstance : public cIdVecObject
    {
    public:
        explicit cPhysicsSubstance(const std::string& id, const cApplication* const app, const physx::PxMaterial* const substance) : cIdVecObject(id, app), _substance((physx::PxMaterial*)substance) {}
        ~cPhysicsSubstance() = default;

        inline physx::PxMaterial* GetSubstance() const { return _substance; }

    private:
        physx::PxMaterial* _substance = nullptr;
    };

    class cPhysicsController : public cIdVecObject
    {
    public:
        explicit cPhysicsController(const std::string& id, const cApplication* const app, const physx::PxController* const controller, const types::f32 eyeHeight) : cIdVecObject(id, app), _controller((physx::PxController*)controller), _eyeHeight(eyeHeight) {}
        ~cPhysicsController() = default;

        inline physx::PxController* GetController() const { return _controller; }
        inline types::f32 GetEyeHeight() const { return _eyeHeight; }

    private:
        physx::PxController* _controller = nullptr;
        types::f32 _eyeHeight = 0.0f;
    };

    class cPhysicsActor : public cIdVecObject
    {
    public:
        explicit cPhysicsActor(const std::string& id, const cApplication* const app, const cGameObject* const gameObject, const physx::PxActor* const actor, const eCategory& actorType) : cIdVecObject(id, app), _gameObject((cGameObject*)gameObject), _actor((physx::PxActor*)actor), _type(actorType) {}
        ~cPhysicsActor() = default;

        inline cGameObject* GetGameObject() const { return _gameObject; }
        inline physx::PxActor* GetActor() const { return _actor; }
        inline eCategory GetType() const { return _type; }

    private:
        cGameObject* _gameObject = nullptr;
        physx::PxActor* _actor = nullptr;
        eCategory _type = eCategory::PHYSICS_ACTOR_DYNAMIC;
    };

    class mPhysics
    {
    public:
        explicit mPhysics(const cApplication* const app);
        ~mPhysics();

        cPhysicsSimulationScene* CreateScene(const std::string& id, const glm::vec3& gravity = glm::vec3(0.0f, -9.81f, 0.0f));
        cPhysicsSubstance* CreateSubstance(const std::string& id, const glm::vec3& params = glm::vec3(0.5f, 0.5f, 0.6f));
        cPhysicsActor* CreateActor(const std::string& id, const eCategory& staticOrDynamic, const eCategory& shapeType, const cPhysicsSimulationScene* const scene, const cPhysicsSubstance* const substance, const types::f32 mass, const sTransform* const transform, const cGameObject* const gameObject);
        cPhysicsController* CreateController(const std::string& id, const types::f32 eyeHeight, const types::f32 height, const types::f32 radius, const sTransform* const transform, const glm::vec3& up, const cPhysicsSimulationScene* const scene, const cPhysicsSubstance* const substance);
        cPhysicsSimulationScene* FindScene(const std::string&);
        cPhysicsSubstance* FindSubstance(const std::string&);
        cPhysicsActor* FindActor(const std::string&);
        cPhysicsController* FindController(const std::string&);
        void DestroyScene(const std::string& id);
        void DestroySubstance(const std::string& id);
        void DestroyActor(const std::string& id);
        void DestroyController(const std::string& id);

        void MoveController(const cPhysicsController* const controller, const glm::vec3& position, const types::f32 minStep = 0.001f);
        glm::vec3 GetControllerPosition(const cPhysicsController* const controller);

        void Simulate();

    private:
        cApplication* _app = nullptr;
        cPhysicsAllocator* _allocator = nullptr;
        cPhysicsError* _error = nullptr;
        cPhysicsCPUDispatcher* _cpuDispatcher = nullptr;
        cPhysicsSimulationEvent* _simulationEvent = nullptr;
        physx::PxFoundation* _foundation = nullptr;
        physx::PxPhysics* _physics = nullptr;
        std::mutex _mutex;
        cIdVec<cPhysicsSimulationScene> _scenes;
        cIdVec<cPhysicsSubstance> _substances;
        cIdVec<cPhysicsActor> _actors;
        cIdVec<cPhysicsController> _controllers;
    };
}