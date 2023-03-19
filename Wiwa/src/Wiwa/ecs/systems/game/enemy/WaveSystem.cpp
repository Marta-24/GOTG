#include <wipch.h>
#include "WaveSystem.h"
#include "../../PhysicsSystem.h"

#include <random>
namespace Wiwa
{
	/*struct Enemy
	{
		int enemyType;
		bool hasFinished;
	};*/

	WaveSystem::WaveSystem()
	{
		m_Wave = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		init = false;
	}

	WaveSystem::~WaveSystem()
	{
	}

	void WaveSystem::OnAwake()
	{
		
	}

	void WaveSystem::OnInit()
	{
		m_Wave = GetComponentIterator<Wave>();
		Wave* wave = GetComponentByIterator<Wave>(m_Wave);
		m_EnemiesCmp.reserve(wave->maxEnemies);
		m_EnemiesId.reserve(wave->maxEnemies);
		wave->currentEnemiesAlive = wave->maxEnemies;
		for (int i = 0; i < wave->maxEnemies; i++)
			SpawnEnemy(i);

		init = true;
	}

	void WaveSystem::OnUpdate()
	{
		if (!init)
		{
			OnInit();
			init = true;
		}

		Wave* wave = GetComponentByIterator<Wave>(m_Wave);

		// Wave has finished
		if (wave->currentEnemiesAlive <= 0)
		{
			wave->hasFinished = true;
		}
		
		QueryEnemies(wave);
	}

	void WaveSystem::OnDestroy()
	{
		m_EnemiesCmp.clear();
		m_EnemiesId.clear();
	}

	void WaveSystem::QueryEnemies(Wave* wave)
	{
		for (int i = 0; i < m_EnemiesCmp.size(); i++)
		{
			Enemy* enemy = GetComponentByIterator<Enemy>(m_EnemiesCmp[i]);
			if (enemy->hasFinished)
			{
				DestroyEnemy(i);
				i--;
				wave->currentEnemiesAlive--;
			}
		}
	}

	void WaveSystem::SpawnEnemy(int index)
	{
		// Create an empty entity
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		Wave* wave = GetComponentByIterator<Wave>(m_Wave);
		std::string enemyName = entityManager.GetEntityName(m_EntityId);
		enemyName += "_enemy_" + std::to_string(index);
		EntityId newEnemyId = entityManager.CreateEntity(enemyName.c_str(), m_EntityId);

		Transform3D enemyTransform = *entityManager.GetComponent<Transform3D>(m_EntityId);
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> dis(-5, 5);
		enemyTransform.localPosition.x += dis(gen);
		enemyTransform.localPosition.z += dis(gen);
		entityManager.AddComponent<Transform3D>(newEnemyId, enemyTransform);

		// Create collision body
		ColliderCapsule capsule;
		capsule.height = 0.7f;
		capsule.radius = 0.7f;
		entityManager.AddComponent<ColliderCapsule>(newEnemyId, capsule);

		CollisionBody initCollBody;
		initCollBody.positionOffset.x = 0;
		initCollBody.positionOffset.y = 0;
		initCollBody.positionOffset.z = 0;
		initCollBody.scalingOffset.x = 1;
		initCollBody.scalingOffset.y = 1;
		initCollBody.scalingOffset.z = 1;
		initCollBody.isTrigger = true;
		initCollBody.isStatic = false;
		initCollBody.doContinuousCollision = false;
		initCollBody.selfTag = 4;
		initCollBody.filterBits = 1 << 0;
		CollisionBody* collBodyPtr = entityManager.AddComponent<CollisionBody>(newEnemyId,initCollBody);
		collBodyPtr->positionOffset.x = 0;
		collBodyPtr->positionOffset.y = 0;
		collBodyPtr->positionOffset.z = 0;
		collBodyPtr->scalingOffset.x = 1;
		collBodyPtr->scalingOffset.y = 1;
		collBodyPtr->scalingOffset.z = 1;
		collBodyPtr->isTrigger = true;
		collBodyPtr->isStatic = false;
		collBodyPtr->doContinuousCollision = false;
		collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		collBodyPtr->filterBits = 1 << 0;

		Character stats;
		stats.healthPoints = 60;
		stats.damage = 0;
		stats.range = 0;
		stats.rof = 0;
		stats.shieldRegeneration = 0;
		stats.speed = 0;
		entityManager.AddComponent<Character>(newEnemyId, stats);

		// Create a enemy component and enemy system
		Enemy enemy;
		enemy.enemyType = 0;
		enemy.hasFinished = false;
		entityManager.AddComponent<Enemy>(newEnemyId, enemy);
		SystemHash sysHash = FNV1A_HASH("EnemySystem");
	
		// Save the enemy component in the list
		m_EnemiesCmp.emplace_back(GetComponentIterator<Enemy>(newEnemyId));
		m_EnemiesId.emplace_back(newEnemyId);

		entityManager.ApplySystem(newEnemyId, sysHash);
		entityManager.ApplySystem<Wiwa::PhysicsSystem>(newEnemyId);

	}

	void WaveSystem::DestroyEnemy(int index)
	{
		EntityId id = m_EnemiesId[index];
		m_EnemiesCmp.erase(m_EnemiesCmp.begin() + index);
		m_EnemiesId.erase(m_EnemiesId.begin() + index);
		// Delete the enemy entity entirely
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		entityManager.DestroyEntity(id);
	}
}