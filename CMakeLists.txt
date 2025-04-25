cmake_minimum_required(VERSION 3.20) # Use a recent version for better preset/vcpkg support

project(AIHauntedHouse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Renderer.cpp
    src/TextureManager.cpp
    src/InputHandler.cpp
    src/Camera.cpp
    src/AudioManager.cpp
    src/Ghost.cpp
    src/Maze.cpp
    # Add other .cpp files here as you create them (e.g., PhysicsManager.cpp, AIManager.cpp)
)

# Define header files (optional, but good for IDEs)
set(HEADERS
    src/Game.h
    src/Renderer.h
    src/TextureManager.h
    src/InputHandler.h
    src/Camera.h
    src/AudioManager.h
    src/Ghost.h
    src/Maze.h
    src/Config.h
    # Add other .h files here
)

# Find packages managed by vcpkg (and standard OpenGL)
# The vcpkg toolchain file specified in CMakePresets.json helps find these.
# Explicit find_package grants access to imported targets for linking.
find_package(OpenGL REQUIRED)       # Finds opengl32.lib on Windows
find_package(GLEW REQUIRED)         # Provides GLEW::GLEW target
find_package(GLUT REQUIRED)         # Provides GLUT::GLUT target (for freeglut)
#find_package(SOIL REQUIRED)         # Provides SOIL::SOIL target (Check vcpkg for exact target name, might be unofficial::soil::soil)
# find_package(OpenAL CONFIG REQUIRED) # Provides OpenAL::OpenAL target [Uncomment when implementing OpenAL]
# find_package(unofficial-bullet3 REQUIRED) # Provides unofficial::bullet3::* targets [Uncomment when implementing Bullet]
# find_package(Torch REQUIRED)        # Provides ${TORCH_LIBRARIES} variable [Uncomment when implementing LibTorch]
# find_package(dr_libs REQUIRED)      # Provides dr_libs::dr_wav target [Uncomment when implementing dr_libs with OpenAL]

# Add executable
add_executable(AIHauntedHouse ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(AIHauntedHouse PRIVATE
    OpenGL::GL          # Link opengl32.lib
    GLEW::GLEW          # Link GLEW
    GLUT::GLUT          # Link FreeGLUT
    #SOIL::SOIL          # Link SOIL (Adjust target name if needed)
    #OpenAL::OpenAL      # Link OpenAL Soft [Uncomment when implementing OpenAL]
    # unofficial::bullet3::BulletDynamics # Link Bullet components [Uncomment when implementing Bullet]
    # unofficial::bullet3::BulletCollision
    # unofficial::bullet3::LinearMath
    # ${TORCH_LIBRARIES}  # Link LibTorch [Uncomment when implementing LibTorch]
    # dr_libs::dr_wav     # Link dr_wav [Uncomment when implementing dr_libs]
    winmm               # Link winmm.lib for PlaySound (Windows only)
   
)
target_include_directories(AIHauntedHouse PRIVATE "C:/dev/vcpkg/installed/x64-windows/include")
target_link_directories(AIHauntedHouse PRIVATE "C:/dev/vcpkg/installed/x64-windows/lib")
target_link_libraries(AIHauntedHouse PRIVATE SOIL)


# Include directories (vcpkg toolchain usually handles this)
# target_include_directories(AIHauntedHouse PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include) # Example if needed

# Optional: Copy necessary runtime DLLs (e.g., from SOIL, Torch, OpenAL, Bullet) to output directory
# This is often needed for dynamic builds, especially on Windows.
# Example for SOIL.dll (adjust path and library name as needed):
# find_library(SOIL_DLL NAMES SOIL.dll HINTS ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/bin)
# if(SOIL_DLL)
#     add_custom_command(TARGET AIHauntedHouse POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy ${SOIL_DLL} $<TARGET_FILE_DIR:AIHauntedHouse>
#         COMMENT "Copying SOIL DLL to output directory")
# endif()

# Add similar custom commands for other required DLLs (freeglut.dll, OpenAL32.dll, Bullet DLLs, LibTorch DLLs)

# Specify include directories if headers are in a separate 'include' folder
target_include_directories(AIHauntedHouse PRIVATE src) # Assuming headers are in 'src' alongside .cpp

