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
    Box*    box    = &application->m_cornellBox;

    f32 roll     = toDegrees( camera->m_roll );
    f32 pitch    = toDegrees( camera->m_pitch );
    f32 yaw      = toDegrees( camera->m_yaw );
    f32 posZ     = camera->m_position[2];

    ImGui::DragFloat( "field of view", &camera->m_fov,  0.5f,  0.0f,  120.0f, "%.3f" );
    ImGui::DragFloat( "roll",          &roll,           0.5f,-90.0f,   90.0f, "%.3f" );
    ImGui::DragFloat( "pitch",         &pitch,          0.5f,-90.0f,   90.0f, "%.3f" );
    ImGui::DragFloat( "yaw",           &yaw,            0.5f,-90.0f,   90.0f, "%.3f" );
    ImGui::DragFloat( "box width",     &box->m_width,   0.5f,  1.0f,   20.0f, "%.3f" );
    ImGui::DragFloat( "box height",    &box->m_height,  0.5f,  1.0f,   20.0f, "%.3f" );
    ImGui::DragFloat( "box length",    &box->m_length,  0.5f,  1.0f,   20.0f, "%.3f" );

    camera->m_roll  = toRadians( roll );
    camera->m_pitch = toRadians( pitch );
    camera->m_yaw   = toRadians( yaw );
    camera->m_position[2] = posZ;

    if( ImGui::Button( "reload" ) )
    {
        Platform* platform = application->m_platform;
        platform->reloadShaders( application->m_renderer );
    }

    ImGui::End();
}
