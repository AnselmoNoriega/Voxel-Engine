#include <Forge.h>
#include <Forge/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Forge
{
    class NotEditor : public Application
    {
    public:
        NotEditor(const ApplicationSpecification& spec)
            : Application(spec)
        {
            PushLayer(new EditorLayer());
        }
    };

    Application* CreateApplication(AppCommandLineArgs args)
    {
        ApplicationSpecification spec;
        spec.Name = "Engine";
        spec.CommandLineArgs = args;

        return new NotEditor(spec);
    }
}