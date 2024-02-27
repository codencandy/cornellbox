#include <AppKit/AppKit.h>

#include "CNC_Window.mm"
#include "CNC_Renderer.mm"

int main()
{
    NSApplication* app = [NSApplication sharedApplication];
    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
    [app setPresentationOptions: NSApplicationPresentationDefault];
    [app finishLaunching];

    bool running = true;

    MainWindow* window = CreateMainWindow( &running );
    
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
        }
    }

    return 0;
}
