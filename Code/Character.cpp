
#include "Character.h"
#include "Root/Root.h"

namespace Oak
{
	ENTITYREG(SceneEntity, Character, "Sample", "Character")

	META_DATA_DESC(Character)
		BASE_SCENE_ENTITY_PROP(Character)
		ASSET_TEXTURE_PROP(Character, projectile, "Visual", "Projectile")
		ASSET_TEXTURE_PROP(Character, enemy, "Visual", "Enemy")
		ASSET_ANIM_GRAPH_2D_PROP(Character, explostion, "Visual", "Explostion")
		SCENEOBJECT_PROP(Character, camera, "Properties", "camera")
		SCENEOBJECT_PROP(Character, screensRoot, "Properties", "screens root")
	META_DATA_DESC_END()

	void Character::Init()
	{
		ScriptEntity2D::Init();

		Tasks(true)->AddTask(0, this, (Object::Delegate)&Character::Draw);
	}

	void Character::Play()
	{
		ScriptEntity2D::Play();

		animRef = FindChild<AnimGraph2D>();
		controllerRef = FindChild<KinematicCapsule2D>();

		auto& screens = screensRoot->GetChilds();

		for (auto* screen : screens)
		{
			screen->SetVisiblity(false);
		}

		SetActiveScreen(0);
	}

	void Character::SetActiveScreen(int index)
	{
		auto& screens = screensRoot->GetChilds();

		if (curScreen != -1)
		{
			screens[curScreen]->SetVisiblity(false);
		}

		curScreen = index;

		screens[curScreen]->SetVisiblity(true);

		auto pos = screens[curScreen]->GetTransform().position;

		screenOffset = pos.x;

		camera->GetTransform().position = pos + Math::Vector3(128.0f, 96.0f, 0.0f);

		pos = transform.position;
		pos.x = screenOffset + 17.0f;

		controllerRef->SetPosition(pos);

		timeToSpawnBot = 2.0f + Math::Rand() * 2.0f;
	}

	void Character::Update(float dt)
	{
		if (animRef == nullptr || controllerRef == nullptr)
		{
			return;
		}

		Math::Vector2 moveDir = 0.0f;
		float runSpeed = 120.0f;
		const char* anim = "idle";

		if (GetRoot()->GetControls()->DebugKeyPressed("KEY_A", AliasAction::Pressed))
		{
			moveDir.x = -1.0f;
			flipped = true;
		}

		if (GetRoot()->GetControls()->DebugKeyPressed("KEY_D", AliasAction::Pressed))
		{
			moveDir.x = 1.0f;
			flipped = false;
		}

		if (!projectileFired && GetRoot()->GetControls()->DebugKeyPressed("KEY_F", AliasAction::JustPressed))
		{
			projectileFired = true;
			projectilePos = transform.position;
			projectilePos.y += 14.0f;

			projectileDir = flipped ? -1.0f : 1.0f;

			projectilePos.x += 16.0f * projectileDir;
		}

		if (fabs(moveDir.x) > 0.1f)
		{
			animRef->GetTransform().scale = Math::Vector3(moveDir.x, 1.0f, 1.0f);
		}

		moveDir.x *= runSpeed;

		gravity -= dt * 400.0f;

		if (gravity < -150.0f)
		{
			gravity = -150.0f;
		}

		moveDir.y = gravity;

		if (gravity > 0.0f)
		{
			anim = "jump";
		}
		else
		{
			anim = "fall";
		}

		if (gravity < 0.0f && controllerRef->controller->IsColliding(PhysController::CollideDown))
		{
			gravity = 0.0f;

			if (fabs(moveDir.x) > 0.1f)
			{
				anim = "run";
			}
			else
			{
				anim = "idle";
			}

			if (GetRoot()->GetControls()->DebugKeyPressed("KEY_W", AliasAction::JustPressed))
			{
				gravity = 200.0f;
			}
		}

		controllerRef->Move(moveDir);

		if (transform.position.x > screenOffset + 256.0f - 10.0f)
		{
			SetActiveScreen(curScreen + 1);
		}

		animRef->anim.GotoNode(anim, false);

		if (projectileFired)
		{
			projectilePos.x += projectileDir * 200.0f * dt;
			projectilePos.z = -0.001f;

			if (projectilePos.x < screenOffset || projectilePos.x > screenOffset + 256.0f)
			{
				projectileFired = false;
			}
		}

		if (timeToSpawnBot >= 0.0f)
		{
			timeToSpawnBot -= dt;

			if (timeToSpawnBot < 0.0f)
			{
				timeToSpawnBot = -1.0f;

				botPosY = 20.0f + Math::Rand() * 30.0f;

				botPos = transform.position;
				botPos.x = 255.0f + screenOffset;
				botPos.y = botPosY;
				botWave = 0.0f;

				GetRoot()->Log("SpawnBot", "Enter");
				
			}
		}
		else
		{
			botPos.x -= dt * 100.0f;
			botWave += dt * 4.0f;
			botPos.y = botPosY + cosf(botWave) * 20.0f;

			if (projectileFired &&
				(projectilePos.x > botPos.x - 10.0f) && (botPos.x + 10.0f > projectilePos.x) &&
				(projectilePos.y > botPos.y - 10.0f) && (botPos.y + 10.0f > projectilePos.y))
			{
				timeToSpawnBot = 2.0f + Math::Rand() * 2.0f;
				projectileFired = false;
				time2ShowExp = 0.2f;
				explostion.GotoNode("Exp", true);
				expPos = botPos;
			}
			else
			if (botPos.x < screenOffset)
			{
				timeToSpawnBot = 2.0f + Math::Rand() * 2.0f;
			}
		}
	}

	void Character::Draw(float dt)
	{
		if (projectileFired)
		{
			Transform trasn = transform;
			trasn.position = projectilePos;
			trasn.size = projectile.GetSize();
			projectile.Draw(&trasn, COLOR_WHITE, dt);
		}
		
		if (time2ShowExp >= 0.0f)
		{
			time2ShowExp -= dt;

			if (time2ShowExp < 0.0f)
			{
				time2ShowExp = -1.0f;
			}
			else
			{
				Transform trasn = transform;
				trasn.position = expPos;
				trasn.size = explostion.GetSize();
				explostion.Draw(&trasn, COLOR_WHITE, dt);
			}
		}

		if (timeToSpawnBot < 0.0f)
		{
			Transform trasn = transform;
			trasn.position = botPos;
			trasn.size = enemy.GetSize();
			enemy.Draw(&trasn, COLOR_WHITE, dt);
		}
	}
}