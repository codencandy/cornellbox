#include "CNC_Userinterface.h"
#include "CNC_Application.h"
#include "CNC_Types.h"

void ShowCornellboxUi( bool* showCornellbox, Application* application )
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin( "Cornellbox UI", showCornellbox );

    ImGui::SeparatorText("3D viewport settings");
    ImGui::Spacing();

    Camera* camera = &application->m_camera;

    f32 rotation = toDegrees( camera->m_leftRightRotation );

    ImGui::DragFloat( "near clipping", &camera->m_near, 0.5f,  0.0f,   20.0f, "%.3f" );
    ImGui::DragFloat( "far clipping",  &camera->m_far,  0.5f, 10.0f, 1000.0f, "%.3f" );
    ImGui::DragFloat( "field of view", &camera->m_fov,  0.5f,  0.0f,  120.0f, "%.3f" );
    ImGui::DragFloat( "left/right",    &rotation,       0.5f,-90.0f,   90.0f, "%.3f" );

    camera->m_leftRightRotation = toRadians( rotation );

    if( ImGui::Button( "reload" ) )
    {
        Platform* platform = application->m_platform;
        platform->reloadShaders( application->m_renderer );
    }

    ImGui::End();
}
