#include "wipch.h"

#include "MeshViewPanel.h"
#include "../../ShadingView.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/Application.h>

#include <Wiwa/Renderer2D.h>
#include <Wiwa/Renderer3D.h>

#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/Model.h>

#include <Wiwa/Input.h>
#include <Wiwa/KeyCodes.h>

MeshViewPanel::MeshViewPanel()
    : Panel("Mesh view")
{
    Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
    float ar = res.w / (float)res.h;

    m_FrameBuffer.Init(res.w, res.h);

    m_Camera.SetPerspective(45.0f, ar);
    m_Camera.setPosition({ 0.0f, 1.0f, 5.0f });

    m_ActiveMesh = new Wiwa::Model("resources/meshes/BakerHouse.fbx");
    m_ActiveMaterial = new Wiwa::Material("assets/textures/test.wimaterial");

    m_MeshPosition = { 0.0f, 0.0f, 0.0f };
    m_MeshRotation = { 0.0f, 0.0f, 0.0f };
    m_MeshScale = { 1.0f, 1.0f, 1.0f };
    
    m_MeshColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_Camera.lookat(m_MeshPosition);

    // Camera control
    camSpeed = 0.005f;
    sensitivity = 0.5f;

    yaw = -90.0f;
    pitch = 0.0f;
}

MeshViewPanel::~MeshViewPanel()
{
}

void MeshViewPanel::Update()
{
}

void MeshViewPanel::Draw()
{
    ImGui::Begin(name, &active, ImGuiWindowFlags_MenuBar);
    
    // Calculate viewport aspect ratio
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
    Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

    float scale = scales.x < scales.y ? scales.x : scales.y;

    ImVec2 isize = { resolution.w * scale, resolution.h * scale };

    ImTextureID tex = (ImTextureID)(intptr_t)m_FrameBuffer.getColorBufferTexture();

    // Horizontal-align viewport
    ImVec2 cpos = ImGui::GetCursorPos();
    cpos.x = (viewportPanelSize.x - isize.x) / 2;
    ImGui::SetCursorPos(cpos);

    if (ImGui::IsWindowHovered())
    {
        // Calculate mouse position in viewport (0 to 1)
        ImVec2 mpos = ImGui::GetMousePos();
        ImVec2 cspos = ImGui::GetCursorScreenPos();

        ImVec2 rpos = { mpos.x - cspos.x, mpos.y - cspos.y };
        CLAMP(rpos.x, 0.0f, isize.x);
        CLAMP(rpos.y, 0.0f, isize.y);
        
        Wiwa::Vector2f v2f = { rpos.x / (float)isize.x, rpos.y / (float)isize.y };
        Wiwa::Vector2f rel2f = lastPos - v2f;
        rel2f.x /= rel2f.x == 0.0f ? 1.0f : abs(rel2f.x);
        rel2f.y /= rel2f.y == 0.0f ? 1.0f : abs(rel2f.y);

        lastPos = v2f;

        // Check if right click was pressed
        if (Wiwa::Input::IsMouseButtonPressed(1)) {
            // Check if relative motion is not 0
            if (rel2f != Wiwa::Vector2f::Zero()) {
                float xoffset = -rel2f.x * sensitivity;
                float yoffset = rel2f.y * sensitivity;

                yaw += xoffset;
                pitch += yoffset;

                if (pitch > 89.0f) pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;

                glm::vec3 direction;
                direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                direction.y = sin(glm::radians(pitch));
                direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

                glm::vec3 front = glm::normalize(direction);
                m_Camera.setFront({ front.x, front.y, front.z });
            }
        }

        // Camera movement
        glm::vec3 campos = m_Camera.getPosition();

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::W)) {
            campos += m_Camera.getFront() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::S)) {
            campos -= m_Camera.getFront() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::A)) {
            campos -= glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::D)) {
            campos += glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::LeftShift)) {
            campos -= m_Camera.getUp() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::Q)) {
            campos += m_Camera.getUp() * camSpeed;
        }

        if (Wiwa::Input::IsKeyPressed(Wiwa::Key::E)) {
            campos -= m_Camera.getUp() * camSpeed;
        }

        m_Camera.setPosition({ campos.x, campos.y, campos.z });
    }

    // Render to frame buffer and imgui viewport
    Wiwa::Application::Get().GetRenderer3D().RenderMeshMaterial(m_ActiveMesh, m_MeshPosition, m_MeshRotation, m_MeshScale, m_ActiveMaterial, &m_FrameBuffer, &m_Camera);
    ImGui::Image(tex, isize, ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const wchar_t* path = (const wchar_t*)payload->Data;
            std::wstring ws(path);
            std::string pathS(ws.begin(), ws.end());
            std::filesystem::path p = pathS.c_str();
            if (p.extension() == ".fbx" || p.extension() == ".FBX")
            {
                WI_INFO("Trying to load payload at path {0}", pathS.c_str());
                //TODO: Load the model
                ResourceId id = Wiwa::Resources::Load<Wiwa::Model>(pathS.c_str());
                m_ActiveMesh = Wiwa::Resources::GetResourceById<Wiwa::Model>(id);
                m_Camera.setPosition({ 0.0f, 1.0f, 5.0f });
                m_Camera.lookat({ 0.0f, 0.0f, 0.0f });
            }
            if (p.extension() == ".wimaterial")
            {
                WI_INFO("Trying to load payload at path {0}", pathS.c_str());
                ResourceId id = Wiwa::Resources::Load<Wiwa::Material>(pathS.c_str());
                m_ActiveMaterial = Wiwa::Resources::GetResourceById<Wiwa::Material>(id);
            }
        }

        ImGui::EndDragDropTarget();
    }
   /* ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    glm::mat4 cameraView = Wiwa::Application::Get().GetRenderer3D().GetView();
    const glm::mat4& cameraProjection = Wiwa::Application::Get().GetRenderer3D().GetPersProjection();
    glm::mat4 transform(1.0f);
    glm::vec3 pos = { m_MeshPosition.x, m_MeshPosition.y, m_MeshPosition.z };
    transform = glm::translate(transform, pos);


    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
        ImGuizmo::OPERATION::SCALE, ImGuizmo::LOCAL, glm::value_ptr(transform));
    */
    ImGui::End();
}
