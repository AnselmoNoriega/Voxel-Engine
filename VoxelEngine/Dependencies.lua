
-- Not ENGINE Dependencies

IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/Engine/vendor/ImGui"
IncludeDir["GLFW"] = "%{wks.location}/Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/vendor/Glad/include"
IncludeDir["Glm"] = "%{wks.location}/Engine/vendor/glm"
IncludeDir["Stb"] = "%{wks.location}/Engine/vendor/stb_image"

Library = {}
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"