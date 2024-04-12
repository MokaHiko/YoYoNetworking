#pragma once

#include <Core/Layer.h>

namespace yoyo
{
    class RendererLayer;
    class Application;
    class RenderPacket;
}

class SandboxLayer : public yoyo::Layer
{
public:
    LayerType(SandboxLayer)

    SandboxLayer(yoyo::Application* app);
    virtual ~SandboxLayer();

    virtual void OnAttach() override;
    virtual void OnDetatch() override;

    virtual void OnEnable() override;
    virtual void OnDisable() override;

    virtual void OnUpdate(float dt) override;
protected:
    void SendRenderPacket(yoyo::RenderPacket* rp);
private:
    yoyo::RendererLayer* m_renderer_layer;
    yoyo::Application* m_app;
};
