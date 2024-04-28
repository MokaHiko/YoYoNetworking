#pragma once

#include <Core/Layer.h>
#include <Core/Memory.h>

namespace yoyo
{
    class RendererLayer;
    class Application;
    class RenderPacket;
}

class Scene;
class ClientLayer : public yoyo::Layer
{
public:
    LayerType(SandboxLayer)

    ClientLayer(yoyo::Application* app);
    virtual ~ClientLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetatch() override;

    virtual void OnEnable() override;
    virtual void OnDisable() override;

    virtual void OnUpdate(float dt) override;
protected:
    void SendRenderPacket(yoyo::RenderPacket* rp);
private:
    yoyo::RenderPacket* m_render_packet;
    yoyo::RendererLayer* m_renderer_layer;
    yoyo::Application* m_app;

    Ref<Scene> m_root_scene;
};
