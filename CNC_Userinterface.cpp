#include "CNC_Userinterface.h"
#include "CNC_Application.h"

void ShowCornellboxUi( bool* showCornellbox, Application* application )
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin( "Cornellbox UI", showCornellbox );

    ImGui::SeparatorText("3D viewport settings");
    ImGui::Spacing();

    static f32 nearClippingPlane = 0.1f;
    static f32 farClippingPlane  = 20.0f;
    static f32 fov               = 60.0f;
    static f32 screenWidth       = WINDOW_WIDTH;
    static f32 screenHeight      = WINDOW_HEIGHT;

    ImGui::DragFloat( "near clipping", &nearClippingPlane, 0.5f,  0.0f,   20.0f, "%.3f" );
    ImGui::DragFloat( "far clipping",  &farClippingPlane,  0.5f, 10.0f, 1000.0f, "%.3f" );
    ImGui::DragFloat( "field of view", &fov,               0.5f,  0.0f,  120.0f, "%.3f" );
    ImGui::DragFloat( "screen width",  &screenWidth,       0.5f,  0.0f, 1200.0f, "%.3f" );
    ImGui::DragFloat( "screen height", &screenHeight,      0.5f,  0.0f,  800.0f, "%.3f" );

    ImGui::End();
}
