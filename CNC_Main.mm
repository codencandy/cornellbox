#include <AppKit/AppKit.h>

#include "CNC_Types.h"
#include "CNC_Window.mm"
#include "CNC_Renderer.mm"
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

            // render the next frame
            Render( renderer );
        }
    }

    return 0;
}
