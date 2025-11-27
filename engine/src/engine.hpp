// engine.hpp

#pragma once

#include <unordered_map>
#include "object.hpp"
#include "types.hpp"

namespace realware
{
	class cEngine : public iObject
	{
		REALWARE_CLASS(cEngine)

	public:
		explicit cEngine(cContext* context);
		virtual ~cEngine() = default;

		void Initialize();
	};
}