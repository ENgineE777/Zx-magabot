
#pragma once

#include "SceneEntities/2D/ScriptEntity2D.h"
#include "SceneEntities/2D/AnimGraph2D.h"
#include "SceneEntities/2D/Node2D.h"
#include "SceneEntities/2D/Camera2D.h"
#include "SceneEntities/2D/KinematicCapsule2D.h"

namespace Oak
{
	class Character : public ScriptEntity2D
	{
		AnimGraph2D* animRef;
		KinematicCapsule2D* controllerRef;
		float gravity = 0.0f;

		int curScreen = -1;
		float screenOffset = 0.0f;

		AssetTextureRef projectile;
		AssetTextureRef enemy;
		AssetAnimGraph2DRef explostion;
		SceneEntityRef<Camera2D> camera;
		SceneEntityRef<Node2D> screensRoot;

		bool flipped = false;
		bool projectileFired = false;
		float projectileDir = 1.0f;
		Math::Vector3 projectilePos;
		float timeToSpawnBot = 1.0f;
		Math::Vector3 botPos;
		float botPosY = 0.0f;
		float botWave = 0.0f;

		float time2ShowExp = -1.0f;
		Math::Vector3 expPos;

	public:
		META_DATA_DECL_BASE(SimpleScript)

		void Init() override;

		void SetActiveScreen(int index);

		void Play() override;
		void Update(float dt) override;
		void Draw(float dt);
	};
}
