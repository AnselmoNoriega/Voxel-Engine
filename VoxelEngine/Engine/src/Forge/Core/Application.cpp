#include "pch.h"
#include "Application.h"

#include "Forge/Events/Event.h"
#include "Forge/Events/ApplicationEvent.h"
#include "Forge/Utils/PlatformUtils.h"

#include "Input/Input.h"

#include "Forge/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <filesystem>

namespace Forge
{
#define BIND_EVENT_FN(x) [this](auto&&... args)-> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

    Application* Application::sInstance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : mSpecification(specification)
    {
        PROFILE_FUNCTION();

        CORE_ASSERT(!sInstance, "There can only be one application");
        sInstance = this;

        if (!mSpecification.WorkingDirectory.empty())
        {
            std::filesystem::current_path(mSpecification.WorkingDirectory);
        }

        mWindow = Window::Create(WindowProps(mSpecification.Name));
        mWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        mImGuiLayer = new ImGuiLayer();
        PushOverlay(mImGuiLayer);
    }

    void Application::OnEvent(Event& e)
    {
        PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

        for (auto layer = mLayerStack.rbegin(); layer != mLayerStack.rend(); ++layer)
        {
            if (e.Handled)
            {
                break;
            }
            (*layer)->OnEvent(e);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        PROFILE_FUNCTION();

        mLayerStack.PushLayer(layer);
        layer->Attach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        PROFILE_FUNCTION();

        mLayerStack.PushOverlay(overlay);
        overlay->Attach();
    }

    void Application::Run()
    {
        PROFILE_FUNCTION();

        while (mRunning)
        {
            PROFILE_SCOPE("Run loop");

            float time = Time::GetTime();
            mTimeStep.UpdateTimeFrame(time);

            if (!mMinimized)
            {
                PROFILE_SCOPE("LayerStack Update");

                for (Layer* layer : mLayerStack)
                {
                    layer->Update(mTimeStep);
                }
            }

            mImGuiLayer->Begin();
            {
                PROFILE_SCOPE("ImGUIStack Update");

                for (Layer* layer : mLayerStack)
                {
                    layer->ImGuiRender();
                }
            }
            mImGuiLayer->End();

            mWindow->Update();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        mRunning = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            mMinimized = true;
            return false;
        }

        mMinimized = false;
        Renderer::ResizeWindow(e.GetWidth(), e.GetHeight());

        return false;
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }
}

