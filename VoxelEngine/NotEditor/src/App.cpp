#include <NotVoxel.h>
#include <NotVoxel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace VoxelForge
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
        spec.Name = "NotRed";
        spec.CommandLineArgs = args;

        return new NotEditor(spec);
    }
}