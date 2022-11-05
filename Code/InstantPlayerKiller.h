
#pragma once

#include "SceneEntities/2D/PhysTriger2D.h"

namespace Oak
{
	class InstantPlayerKiller : public PhysTriger2D
	{
	public:

		virtual ~InstantPlayerKiller() = default;

		META_DATA_DECL(InstantPlayerKiller)

		void Init() override;
		void OnContactStart(int index, SceneEntity* entity, int contactIndex) override;
	};
}
