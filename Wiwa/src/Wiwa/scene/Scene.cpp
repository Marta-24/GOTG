#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/utilities/render/LightManager.h>

#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/audio/Audio.h>

namespace Wiwa
{
	Scene::Scene() : m_InstanceRenderer(40500)
	{
		mMaxTimeEntering = 0;
		mMaxTimeLeaving = 0;

		// Initialize instance renderer with shader
		m_InstanceRenderer.Init("resources/shaders/instanced_tex_color");

		m_GuiManager = new GuiManager();
		m_GuiManager->Init(this);

		m_EntityManager.SetScene(this);
		m_CameraManager = new CameraManager();
		m_LightManager = new LightManager();
		m_PhysicsManager = new PhysicsManager();

		m_PhysicsManager->InitWorld();
	}

	Scene::~Scene()
	{
		delete m_CameraManager;
		delete m_LightManager;
		delete m_GuiManager;

		// Clear entity manager
		m_EntityManager.Clear();

		// Clear physics world
		m_PhysicsManager->CleanWorld();
		delete m_PhysicsManager;
		m_PhysicsManager = nullptr;
	}

	void Scene::Start()
	{
	}

	void Scene::Awake()
	{
		m_EntityManager.SystemsAwake();
	}

	void Scene::Init()
	{
		m_EntityManager.SystemsInit();
	}

	void Scene::Update()
	{
		switch (m_CurrentState)
		{
		case Scene::SCENE_ENTERING:
			m_EntityManager.UpdateWhitelist();

			UpdateEnter();
			RenderEnter();

			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			
			if (m_TransitionTimer >= mMaxTimeEntering) {
				m_TransitionTimer = 0;

				m_CurrentState = SCENE_LOOP;
			}
			break;
		case Scene::SCENE_LOOP:
			m_EntityManager.SystemsUpdate();
			m_GuiManager->Update();
			ProcessInput();
			UpdateLoop();
			RenderLoop();
			break;
		case Scene::SCENE_LEAVING:
			m_EntityManager.UpdateWhitelist();

			UpdateLeave();
			RenderLeave();

			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			
			if (m_TransitionTimer >= mMaxTimeLeaving) {
				m_TransitionTimer = 0;

				SceneManager::LoadSceneByIndex(m_SceneToChange, m_SceneChangeFlags);
			}
			break;
		default:
			break;
		}
	}

	void Scene::ModuleUpdate()
	{
		m_CameraManager->Update();

		Wiwa::Renderer2D &r2d = Wiwa::Application::Get().GetRenderer2D();
		r2d.UpdateInstanced(this);

		m_GuiManager->Draw();

		m_EntityManager.Update();

		m_PhysicsManager->UpdateEngineToPhysics();

		if (SceneManager::IsPlaying())
		{
			m_PhysicsManager->StepSimulation();
			m_PhysicsManager->UpdatePhysicsToEngine();
			// m_PhysicsManager->LogBodies();
		}
		// m_PhysicsManager->LogBodies();

		if (!SceneManager::IsPlaying())
		{
			m_EntityManager.UpdateWhitelist();
		}
	}

	void Scene::Load()
	{
		m_TransitionTimer = 0;

		m_CurrentState = SCENE_ENTERING;
	}

	void Scene::Unload(bool unload_resources)
	{
		Audio::StopAllEvents();
		
		// Sleep to wait till Audio thread stops all events
		Sleep(10);

		if (unload_resources)
		{
			// TODO: Fix unloading for the editor
			return;
			Wiwa::Resources::UnloadAllResources();
		}
	}

	void Scene::ChangeScene(size_t scene, int flags)
	{
		m_SceneToChange = scene;
		m_SceneChangeFlags = flags;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;
	}
}