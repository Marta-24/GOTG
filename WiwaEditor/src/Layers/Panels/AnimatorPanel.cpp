#include "AnimatorPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Mesh.h>
//#include <Wiwa/utilities/render/Animation.h>


AnimatorPanel::AnimatorPanel(EditorLayer* instance)
	: Panel("Animator", ICON_FK_MALE, instance)
{
}

AnimatorPanel::~AnimatorPanel()
{
}

void AnimatorPanel::Draw()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);

	static float w = 200.0f;
	static float h = 300.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::BeginChild("child1", ImVec2(w, h), true);

	if (m_EntitySet && (m_CurrentID >= 0))
	{
		const char* entName = em.GetEntityName(m_CurrentID);
		std::string edit = entName;

		ImGui::Text("Name: %s", (char*)edit.c_str());
		ImGui::SameLine();
		ImGui::Text("(%i)", m_CurrentID);

		Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(m_CurrentID);

		ImGui::NewLine();

		if (model->GetAnimations()[0] != NULL)
		{
			std::vector<Wiwa::Animation*> aux = model->GetAnimations();
			if (aux[0] != nullptr)
			{
				for (int i = 0; i < aux.size(); i++)
				{
					
					if (ImGui::Button(aux[i]->name.c_str()))
					{
						animButtons.push_back(buttonInfo(aux[i]->name.c_str()));
					}
				
				}
			}
		}
	}
	else
	{
		ImGui::Text("Select an entity!");
	}

	

	
	ImGui::EndChild();
	ImGui::SameLine();
	
	if (ImGui::InvisibleButton("splitter", ImVec2(8.0f, h))) 
	{
		w += ImGui::GetIO().MouseDelta.x;
	}
	ImGui::SameLine();
	ImGui::BeginChild("child2", ImVec2(0, h), true);
	for (int i = 0; i < animButtons.size(); i++)
	{
		//ImGui::Button(animButtons[i].name.c_str());
		ImGui::BeginChild(animButtons[i].name.c_str(), ImVec2(100, 100), true);
		ImGui::EndChild();
	}
	ImGui::EndChild();
	
	ImGui::PopStyleVar();
	ImGui::End();
}

	
void AnimatorPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>({ &AnimatorPanel::OnEntityChangeEvent, this });
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>({ &AnimatorPanel::OnSceneChangeEvent, this });
}

bool AnimatorPanel::OnEntityChangeEvent(EntityChangeEvent& e)
{
	m_CurrentID = e.GetResourceId();
	m_EntitySet = true;
	return false;
}

bool AnimatorPanel::OnSceneChangeEvent(Wiwa::SceneChangeEvent& e)
{
	m_CurrentID = -1;
	m_EntitySet = false;
	return false;
}