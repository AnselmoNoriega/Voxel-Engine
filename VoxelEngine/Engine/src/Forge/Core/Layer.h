#pragma once

#include "Forge/Core/Core.h"
#include "Forge/Events/Event.h"

namespace Forge
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void Attach() {}
        virtual void Detach() {}
        virtual void Update(float deltaTime) {}
        virtual void ImGuiRender() {}
        virtual void OnEvent(Event& myEvent) {}

        inline const std::string& GetName() const { return mDebugName; }

    protected:
        std::string mDebugName;
    };
}

