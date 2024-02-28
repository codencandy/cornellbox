#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include "CNC_Types.h"

void checkError( NSError* error )
{
    if( error != NULL )
    {
        NSLog( @"%@", [error localizedDescription] );
    }
}

@interface MetalRenderer : NSObject< MTKViewDelegate >
{
    @public
        MTKView*                   m_view;
        id<MTLDevice>              m_device;
        id<MTLCommandQueue>        m_queue;
        id<MTLRenderPipelineState> m_pipelineState;
}

- (void)setup;

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

- (void)setup
{
    @autoreleasepool
    {
        NSError* error = NULL;
        NSString* shaderSource = [NSString stringWithContentsOfFile: @"CNC_Shader.metal" encoding: NSUTF8StringEncoding error: &error];
        checkError( error );

        MTLCompileOptions* options = [MTLCompileOptions new];
        id<MTLLibrary> library = [m_device newLibraryWithSource: shaderSource options: options error: &error];
        checkError( error );

        id<MTLFunction> vertexShader   = [library newFunctionWithName:@"VertexShader"];
        id<MTLFunction> fragmentShader = [library newFunctionWithName:@"FragmentShader"];

        if( vertexShader   == NULL ) NSLog( @"could not load vertex shader" );
        if( fragmentShader == NULL ) NSLog( @"could not load fragment shader " );

        MTLRenderPipelineDescriptor* renderDesc = [MTLRenderPipelineDescriptor new];
        renderDesc.vertexFunction   = vertexShader;
        renderDesc.fragmentFunction = fragmentShader;

        m_pipelineState = [m_device newRenderPipelineStateWithDescriptor: renderDesc error: &error];
        checkError( error );
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

    [renderer setup];

    return renderer;
}

void Render( MetalRenderer* renderer )
{
    [renderer->m_view draw];
}
