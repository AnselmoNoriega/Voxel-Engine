#pragma once

#include "Forge/Core/Layer.h"

namespace Forge
{
    class  ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void Attach() override;
        void Detach() override;
        void OnEvent(Event& myEvent) override;

        void Begin();
        void End();

        void SetEventsActive(bool block) { mEventsBlocked = block; }

        void SetThemeColors();

        uint32_t GetActiveWidgetID() const;

    private:
        bool mEventsBlocked = false;
        float mTime = 0.0f;
    };
}

