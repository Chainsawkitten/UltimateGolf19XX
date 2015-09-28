cmake_minimum_required(VERSION 2.8.12)

project(UltimateGolf19XX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests" FORCE)
add_subdirectory(externals/glfw)
include_directories(externals/glfw/include)

# GLEW
add_subdirectory(externals/glew/build/cmake)
include_directories(externals/glew/include)

# SimpleIni
include_directories(externals/simpleini)

# GLM
include_directories(externals/glm)

# STB
include_directories(externals/stb)

# OpenAL Soft
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Don't build OpenAL Soft examples" FORCE)
add_subdirectory(externals/openal-soft)
include_directories(externals/openal-soft/include)
if(WIN32)
    set(OPENAL_LIB OpenAL32)
else()
    set(OPENAL_LIB openal)
endif()

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

# Copy resources
set(RESOURCES
    assets/TropicalSunnyDay
    assets/CGTextures
    assets/Terrain
    assets/DustParticle.png
)

file(COPY ${RESOURCES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources)

# Setup embedded source
set(EMBEDDED_INPUT
    shaders/Default2D.vert
    shaders/Font.vert
    shaders/Font.frag
    shaders/SingleColor2D.frag
    shaders/Skybox.vert
    shaders/Skybox.frag
    shaders/Texture2D.frag
    shaders/Post.vert
    shaders/PostCopy.frag
    shaders/PostColor.frag
    shaders/PostFXAA.frag
    shaders/Deferred.frag
    shaders/Default3D.vert
    shaders/Default3D.geom
    shaders/Default3D.frag
    shaders/Blendmap.frag
    shaders/Particle.vert
    shaders/Particle.geom
    shaders/Particle.frag
    assets/ABeeZee.ttf
    assets/File.png
    assets/Options.png
    assets/Play.png
)

# Headerize
find_program(HEADERIZE_EXE Headerize
    DOC "Location of the Headerize executable"
)

macro(ProcessWithHeaderize headervar sourcevar inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
    add_custom_command(
        OUTPUT ${outputfile}.hpp ${outputfile}.cpp
        COMMAND ${HEADERIZE_EXE} ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${inputfile}
        COMMENT "Generating headers for ${inputfile}"
    )
    list(APPEND ${headervar} ${outputfile}.hpp)
    list(APPEND ${sourcevar} ${outputfile}.cpp)
endmacro()

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

include_directories(src)


## TARGETS ##

## Core ##
set(CORE_SRCS
        src/Core/GameWindow.cpp
        src/Core/Resources.cpp
        src/Core/RenderTarget.cpp
        src/Core/Light.cpp
        src/Core/Skybox.cpp
        src/Core/Audio/Listener.cpp
        src/Core/Audio/Sound.cpp
        src/Core/Audio/SoundBuffer.cpp
        src/Core/Audio/SoundSystem.cpp
        src/Core/Audio/WaveFile.cpp
        src/Core/Audio/VorbisFile.cpp
        src/Core/Font/Font.cpp
        src/Core/Util/FileSystem.cpp
        src/Core/Util/Input.cpp
        src/Core/Util/Settings.cpp
        src/Core/Util/GameSettings.cpp
        src/Core/Util/Log.cpp
        src/Core/Shader/Shader.cpp
        src/Core/Shader/ShaderProgram.cpp
        src/Core/Camera/Camera.cpp
        src/Core/Camera/FirstPersonCamera.cpp
        src/Core/Geometry/Geometry2D.cpp
        src/Core/Geometry/Rectangle.cpp
        src/Core/Geometry/Geometry3D.cpp
        src/Core/Geometry/Cube.cpp
        src/Core/Geometry/Square.cpp
	src/Core/Geometry/Terrain.cpp
        src/Core/Object/Object.cpp
	src/Core/Object/TerrainObject.cpp
        src/Core/Object/GeometryObject.cpp
        src/Core/Object/GolfBall.cpp
        src/Core/Player/Player.cpp
        src/Core/Player/FirstPersonPlayer.cpp
        src/Core/PostProcessing/PostProcessing.cpp
        src/Core/PostProcessing/ColorFilter.cpp
        src/Core/PostProcessing/FXAAFilter.cpp
        src/Core/Scene/Scene.cpp
        src/Core/Scene/TestScene.cpp
        src/Core/Texture/Texture.cpp
        src/Core/Texture/Texture2D.cpp
        src/Core/Texture/CubeMapTexture.cpp
        src/Core/Particles/CuboidParticleEmitter.cpp
        src/Core/Particles/ParticleEmitter.cpp
        src/Core/Particles/ParticleSystem.cpp
        src/Core/Particles/PointParticleEmitter.cpp
	src/Core/Physics/AABB.cpp
	src/Core/Physics/Frustum.cpp
	src/Core/Physics/OBB.cpp
	src/Core/Physics/Ray.cpp
	src/Core/Physics/Rectangle2D.cpp
	src/Core/Physics/Triangle.cpp
    )

set(CORE_HEADERS
        src/Core/GameWindow.hpp
        src/Core/Resources.hpp
        src/Core/RenderTarget.hpp
        src/Core/Light.hpp
        src/Core/Skybox.hpp
        src/Core/Audio/Listener.hpp
        src/Core/Audio/Sound.hpp
        src/Core/Audio/SoundBuffer.hpp
        src/Core/Audio/SoundFile.hpp
        src/Core/Audio/SoundSystem.hpp
        src/Core/Audio/WaveFile.hpp
        src/Core/Audio/VorbisFile.hpp
        src/Core/Font/Font.hpp
        src/Core/Util/FileSystem.hpp
        src/Core/Util/Input.hpp
        src/Core/Util/Settings.hpp
        src/Core/Util/GameSettings.hpp
        src/Core/Util/Log.hpp
        src/Core/Shader/Shader.hpp
        src/Core/Shader/ShaderProgram.hpp
        src/Core/Camera/Camera.hpp
        src/Core/Camera/FirstPersonCamera.hpp
        src/Core/Geometry/Geometry2D.hpp
        src/Core/Geometry/Rectangle.hpp
        src/Core/Geometry/Geometry3D.hpp
        src/Core/Geometry/Cube.hpp
        src/Core/Geometry/Square.hpp
	src/Core/Geometry/Terrain.hpp
        src/Core/Object/Object.hpp
	src/Core/Object/TerrainObject.hpp
        src/Core/Object/GeometryObject.hpp
        src/Core/Object/GolfBall.hpp
        src/Core/Player/Player.hpp
        src/Core/Player/FirstPersonPlayer.hpp
        src/Core/PostProcessing/PostProcessing.hpp
        src/Core/PostProcessing/Filter.hpp
        src/Core/PostProcessing/ColorFilter.hpp
        src/Core/PostProcessing/FXAAFilter.hpp
        src/Core/Scene/Scene.hpp
        src/Core/Scene/TestScene.hpp
        src/Core/Texture/Texture.hpp
        src/Core/Texture/Texture2D.hpp
        src/Core/Texture/CubeMapTexture.hpp
        src/Core/Particles/CuboidParticleEmitter.hpp
        src/Core/Particles/ParticleEmitter.hpp
        src/Core/Particles/ParticleSystem.hpp
        src/Core/Particles/PointParticleEmitter.hpp
        src/Core/Particles/ParticleType.hpp
	src/Core/Physics/AABB.hpp
	src/Core/Physics/Frustum.hpp
	src/Core/Physics/OBB.hpp
	src/Core/Physics/Ray.hpp
	src/Core/Physics/Rectangle2D.hpp
        src/Core/Physics/Triangle.hpp
    )

create_directory_groups(${CORE_SRCS} ${CORE_HEADERS})

#set_property(SOURCE ${CORE_SRCS} APPEND PROPERTY OBJECT_DEPENDS ${EMBEDDED_HEADER})

add_library(Core STATIC ${CORE_SRCS} ${CORE_HEADERS} ${EMBEDDED_SOURCE})
target_compile_features(Core PRIVATE cxx_nullptr cxx_generalized_initializers)



## Editor ##
set(EDITOR_SRCS
        src/Editor/editor.cpp
        src/Editor/EditorWindow.cpp
        src/Editor/GUI/Widget.cpp
        src/Editor/GUI/Container.cpp
        src/Editor/GUI/HorizontalLayout.cpp
        src/Editor/GUI/VerticalLayout.cpp
        src/Editor/GUI/Button.cpp
        src/Editor/GUI/ImageButton.cpp
        src/Editor/GUI/ImageTextButton.cpp
        src/Editor/GUI/SelectHymnWindow.cpp
        src/Editor/GUI/TextField.cpp
        src/Editor/Util/EditorSettings.cpp
    )

set(EDITOR_HEADERS
        src/Editor/editor.hpp
        src/Editor/EditorWindow.hpp
        src/Editor/GUI/Widget.hpp
        src/Editor/GUI/Container.hpp
        src/Editor/GUI/HorizontalLayout.hpp
        src/Editor/GUI/VerticalLayout.hpp
        src/Editor/GUI/Button.hpp
        src/Editor/GUI/ImageButton.hpp
        src/Editor/GUI/ImageTextButton.hpp
        src/Editor/GUI/SelectHymnWindow.hpp
        src/Editor/GUI/TextField.hpp
        src/Editor/Util/EditorSettings.hpp
    )

create_directory_groups(${EDITOR_SRCS} ${EDITOR_HEADERS})

add_executable(Editor ${EDITOR_SRCS} ${EDITOR_HEADERS})
target_link_libraries(Editor Core)
target_link_libraries(Editor glew glfw ${GLFW_LIBRARIES} ${OPENAL_LIB})
target_compile_features(Editor PRIVATE cxx_nullptr cxx_generalized_initializers)



## Game ##
set(GAME_SRCS
        src/Game/game.cpp
    )

set(GAME_HEADERS
        src/Game/game.hpp
    )

create_directory_groups(${GAME_SRCS} ${GAME_HEADERS})

add_executable(Game ${GAME_SRCS} ${GAME_HEADERS})
target_link_libraries(Game Core)
target_link_libraries(Game glew glfw ${GLFW_LIBRARIES} ${OPENAL_LIB})
target_compile_features(Game PRIVATE cxx_nullptr cxx_generalized_initializers)
