#pragma once

#include "Forge/Core/Window.h"
#include "Forge/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Forge
{
    class WinWindow : public Window
    {
	public:
		WinWindow(const WindowProps& props);
		~WinWindow() override;

		void Update() override;

		unsigned int GetWidth() const override { return mData.Width; }
		unsigned int GetHeight() const override { return mData.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { mData.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return mWindow; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* mWindow;
		Scope<GraphicsContext> mContext;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData mData;
    };
}