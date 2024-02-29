#include <AppKit/AppKit.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_metal.h>
#include <imgui_impl_osx.h>

#include "CNC_Types.h"
#include "CNC_Window.mm"
#include "CNC_Renderer.mm"

#include "CNC_Application.h"
#include "CNC_Application.cpp"

int main()
{
    NSApplication* app = [NSApplication sharedApplication];
    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
    [app setPresentationOptions: NSApplicationPresentationDefault];
    [app finishLaunching];

    bool running = true;

    MainWindow*    window   = CreateMainWindow( &running );
    MetalRenderer* renderer = CreateMetalRenderer();

    [window setContentView: renderer->m_view];

    // IMGUI SETUP
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.Fonts->AddFontFromFileTTF( "res/comfortaa.ttf", 14 );

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();
    ImGuiStyle& myStyle = ImGui::GetStyle();
    myStyle.FrameRounding  = 3.0f;
    myStyle.WindowRounding = 6.0f;
    myStyle.WindowPadding  = ImVec2( 8.0f, 8.0f );
    myStyle.FramePadding   = ImVec2( 8.0f, 8.0f );

    // Setup Renderer backend
    ImGui_ImplMetal_Init(renderer->m_device);
    ImGui_ImplOSX_Init(renderer->m_view);

    io.DisplaySize.x = renderer->m_view.bounds.size.width;
    io.DisplaySize.y = renderer->m_view.bounds.size.height;

    CGFloat framebufferScale = renderer->m_view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
    io.DisplayFramebufferScale = ImVec2(framebufferScale, framebufferScale);

    struct Application cornellApp = {0};

    Load( &cornellApp );

    while( running )
    {
        @autoreleasepool
        {
            NSEvent* event = NULL;
            do
            {
                event = [app nextEventMatchingMask:NSEventMaskAny 
                                         untilDate: NULL 
                                            inMode: NSDefaultRunLoopMode
                                           dequeue:true];

                [app sendEvent: event];
                [app updateWindows];
            }
            while( event != NULL );

            [window->m_displayLinkSignal wait];

            Update( &cornellApp );
            Render( &cornellApp );

            // Start the Dear ImGui frame
            ImGui_ImplMetal_NewFrame( [renderer->m_view currentRenderPassDescriptor] );
            ImGui_ImplOSX_NewFrame( renderer->m_view );
            ImGui::NewFrame();            

            static bool show_demo_window = true;
            if( show_demo_window )
            {
                ImGui::ShowDemoWindow( &show_demo_window );
            }
            else
            {
                if( io.KeysDown[ImGuiKey_F1] )
                {
                    show_demo_window = true;
                }
            }

            // render the next frame
            Render( renderer );
        }
    }

    Exit( &cornellApp );

    return 0;
}
