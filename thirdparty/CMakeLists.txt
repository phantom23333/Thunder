cmake_minimum_required (VERSION 3.8)

project (CMakeTest)

set(THIRDPARTY_LIBRARIES)

# glfw
if (NOT TARGET GLFW)
	add_subdirectory (glfw-3.3.2)
	set_property(TARGET glfw PROPERTY FOLDER "thirdparty")
endif()

# glad
if (NOT TARGET glad)
	add_subdirectory (glad)
	set_property(TARGET glad PROPERTY FOLDER "thirdparty")
endif()

# glm
if (NOT TARGET glm)
	add_subdirectory (glm)
	set_property(TARGET glm PROPERTY FOLDER "thirdparty")
endif()	

# stb
if (NOT TARGET stb_image AND NOT TARGET stb_truetype)
	add_subdirectory (stb_image)
	add_subdirectory (stb_truetype)
	
	set_property(TARGET stb_image PROPERTY FOLDER "thirdparty")
	set_property(TARGET stb_truetype PROPERTY FOLDER "thirdparty")
endif()


# glui
if (NOT TARGET glui)
	add_subdirectory (glui)
	set_property(TARGET glui PROPERTY FOLDER "thirdparty")
endif()

# imgui
if (NOT TARGET imgui)
	add_subdirectory (imgui-docking)
	set_property(TARGET imgui PROPERTY FOLDER "thirdparty")
endif()

# profilerLib
if (NOT TARGET profilerLib)
	add_subdirectory (profilerLib)
	set_property(TARGET profilerLib PROPERTY FOLDER "thirdparty")
endif()

#safeSave
if (NOT TARGET safeSave)
	add_subdirectory (safeSave)
	set_property(TARGET safeSave PROPERTY FOLDER "thirdparty")
endif()

# raudio
if (NOT TARGET raudio)
	add_subdirectory (raudio)
	set_property(TARGET raudio PROPERTY FOLDER "thirdparty")
endif()

if (NOT TARGET gl2d)
	add_subdirectory (gl2d)
	set_property(TARGET gl2d PROPERTY FOLDER "thirdparty")
endif()