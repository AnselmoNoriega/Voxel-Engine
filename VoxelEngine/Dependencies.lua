
-- Not Voxel Dependencies

IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/NotVoxel/vendor/ImGui"
IncludeDir["GLFW"] = "%{wks.location}/NotVoxel/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/NotVoxel/vendor/Glad/include"
IncludeDir["Glm"] = "%{wks.location}/NotVoxel/vendor/glm"
IncludeDir["Stb"] = "%{wks.location}/NotVoxel/vendor/stb_image"

Library = {}
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"