#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include "CNC_Types.h"
#include "imgui.h"

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
        id<MTLBuffer>              m_uniformBuffer;
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

        [commandEncoder setRenderPipelineState: m_pipelineState];

        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData( ImGui::GetDrawData(), commandBuffer, commandEncoder);

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

        MTLVertexDescriptor* vertexDesc = [MTLVertexDescriptor new];
        vertexDesc.attributes[0].bufferIndex = 0;
        vertexDesc.attributes[0].format      = MTLVertexFormatFloat3;
        vertexDesc.attributes[0].offset      = offsetof( struct VertexInput, m_position );
        
        vertexDesc.attributes[1].bufferIndex = 0;
        vertexDesc.attributes[1].format      = MTLVertexFormatFloat2;
        vertexDesc.attributes[1].offset      = offsetof( struct VertexInput, m_uv );

        vertexDesc.layouts[0].stride         = sizeof( struct VertexInput );
        vertexDesc.layouts[0].stepFunction   = MTLVertexStepFunctionPerVertex;

        MTLRenderPipelineDescriptor* renderDesc = [MTLRenderPipelineDescriptor new];
        renderDesc.vertexFunction                  = vertexShader;
        renderDesc.fragmentFunction                = fragmentShader;
        renderDesc.vertexDescriptor                = vertexDesc;
        renderDesc.colorAttachments[0].pixelFormat = [m_view colorPixelFormat];

        m_pipelineState = [m_device newRenderPipelineStateWithDescriptor: renderDesc error: &error];
        checkError( error );

        m_uniformBuffer = [m_device newBufferWithLength: sizeof( struct UniformData )options: MTLResourceCPUCacheModeDefaultCache];
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
