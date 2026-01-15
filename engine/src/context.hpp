// context.hpp

#pragma once

#include <unordered_map>
#include "object.hpp"
#include "types.hpp"

namespace realware
{
	template <typename T>
	class cMemoryPool;

	class cContext
	{
		REALWARE_CLASS(cContext)

	public:
		explicit cContext() = default;
		~cContext() = default;

		template <typename T>
		T* Create();

		template <typename T>
		void Destroy(T* object);

		void CreateMemoryPool();

		template <typename T>
		void RegisterFactory();

		void RegisterSubsystem(iObject* object);

		template <typename T>
		inline cMemoryPool<T>* GetMemoryPool() const;

		template <typename T>
		inline T* GetFactory() const;

		template <typename T>
		inline T* GetSubsystem() const;

	private:
		std::unordered_map<ClassType, std::shared_ptr<iObject>> _memoryPools;
		std::unordered_map<ClassType, std::shared_ptr<iFactory>> _factories;
		std::unordered_map<ClassType, std::shared_ptr<iObject>> _subsystems;
	};

	template <typename T>
	T* cContext::Create()
	{
		const ClassType type = T::GetType();
		const auto it = _factories.find(type);
		if (it != _factories.end())
			return (T*)_factories[type]->Create();
		else
			return nullptr;
	}

	template <typename T>
	void cContext::Destroy(T* object)
	{
		const ClassType type = T::GetType();
		const auto it = _factories.find(type);
		if (it != _factories.end())
			_factories[type]->Destroy(object);
	}

	template <typename T>
	void cContext::RegisterFactory()
	{
		const ClassType type = T::GetType();
		const auto it = _factories.find(type);
		if (it == _factories.end())
			_factories.insert({type, std::make_shared<cFactory<T>>(this)});
	}

	template <typename T>
	inline cMemoryPool<T>* cContext::GetMemoryPool() const
	{
		const ClassType type = T::GetType();
		const auto it = _memoryPools.find(type);
		if (it == _memoryPools.end())
			return _memoryPools[type];
		else
			return nullptr;
	}

	template <typename T>
	T* cContext::GetFactory() const
	{
		const ClassType type = T::GetType();
		const auto it = _factories.find(type);
		if (it != _factories.end())
			return _factories[type];
		else
			return nullptr;
	}

	template <typename T>
	T* cContext::GetSubsystem() const
	{
		const ClassType type = T::GetType();
		const auto it = _subsystems.find(type);
		if (it != _subsystems.end())
			return _subsystems[type];
		else
			return nullptr;
	}
}