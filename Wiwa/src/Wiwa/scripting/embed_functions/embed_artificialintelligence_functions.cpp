#include "wipch.h"
#include "embed_artificialintelligence_functions.h"
#include "Wiwa/ecs/systems/AgentAISystem.h"


void SendAIToPosition(size_t entity_id,glm::vec3 target_pos)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(entity_id);

	if (agentPtr)
	{
		agentPtr->GoToPosition(target_pos);
	}
}

void SendAIToPositionById(size_t entity_id, size_t target_id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(entity_id);

	//WI_INFO(" Position of the player: X {}",em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition.x);
	//WI_INFO(" Position of the player: Y {}", em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition.y);
	//WI_INFO(" Position of the player: Z {}", em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition.z);
	if (agentPtr)
	{
		agentPtr->GoToPosition(em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition);		
	}
}

float DistanceAgentTargetById(size_t entity_id, size_t target_id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	return glm::distance(em.GetComponent<Wiwa::Transform3D>(entity_id)->localPosition, em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition);
}
