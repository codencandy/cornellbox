#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

@interface MetalRenderer : NSObject< MTKViewDelegate >
{
    @public
        MTKView*            m_view;
        id<MTLDevice>       m_device;
        id<MTLCommandQueue> m_queue;
}

@end

@implementation MetalRenderer 

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{

}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    @autoreleasepool
    {
        id<MTLCommandBuffer> commandBuffer = [m_queue commandBuffer];
        id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor: [m_view currentRenderPassDescriptor]];

        [commandEncoder endEncoding];
        [commandBuffer presentDrawable: [m_view currentDrawable]];
        [commandBuffer commit];
    }
}

@end

MetalRenderer* CreateMetalRenderer()
{
    MetalRenderer* renderer = [MetalRenderer new];

    CGRect contentRect = CGRectMake( 0, 0, 800, 500 );
    renderer->m_device = MTLCreateSystemDefaultDevice();
    renderer->m_view   = [[MTKView alloc] initWithFrame: contentRect device: renderer->m_device];
    
    [renderer->m_view setDelegate: renderer];
    [renderer->m_view setPaused: true];
    [renderer->m_view setNeedsDisplay: false];
    [renderer->m_view setClearColor: MTLClearColorMake( 1.0, 0.0, 0.0, 1.0) ];

    renderer->m_queue = [renderer->m_device newCommandQueue];

    return renderer;
}

void Render( MetalRenderer* renderer )
{
    [renderer->m_view draw];
}
