// context.cpp

#include "context.hpp"
#include "memory_pool.hpp"
#include "render_context.hpp"

namespace realware
{
	void cContext::CreateMemoryPool()
	{
		_memoryPools.insert({ cWindow::GetType(), std::make_shared<cMemoryPool<cWindow>>(this) });
		_memoryPools.insert({ cBuffer::GetType(), std::make_shared<cMemoryPool<sBuffer>>(this) });
		_memoryPools.insert({ cShader::GetType(), std::make_shared<cMemoryPool<sShader>>(this) });
		_memoryPools.insert({ cTexture::GetType(), std::make_shared<cMemoryPool<sTexture>>(this) });
		_memoryPools.insert({ cRenderTarget::GetType(), std::make_shared<cMemoryPool<sRenderTarget>>(this) });
		_memoryPools.insert({ cRenderPass::GetType(), std::make_shared<cMemoryPool<sRenderPass>>(this) });
	}

	void cContext::RegisterSubsystem(iObject* object)
	{
		ClassType type = object->GetType();
		const auto it = _subsystems.find(type);
		if (it == _subsystems.end())
			_subsystems.insert({ type, std::make_shared<iObject>(object) });
	}
}