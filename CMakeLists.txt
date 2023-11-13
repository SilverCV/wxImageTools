
cmake_minimum_required(VERSION 3.20)
project(wxImageTool)

set(CMAKE_CXX_STANDARD 17)

# find_package(CUDA REQUIRED)
find_package(wxWidgets COMPONENTS core gl base net REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(${wxWidgets_INCLUDE_DIRS})

# include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(wxImageTool
        GLHelper.cpp
        GLHelper.h
        ImageApp.cpp
        ImageApp.h
        ImageFrame.cpp
        ImageFrame.h
        typedefs.h
        ImageHelper.cpp
        ImageHelper.h
        ImageCanvas.cpp
        ImageCanvas.h
)
message("${CUDA_INCLUDE_DIRS}")
target_link_libraries(wxImageTool
        ${wxWidgets_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${OPENGL_LIBRARY}
        ${GLEW_LIBRARIES}

        # ${CUDA_LIBRARIES}
)
target_link_options(wxImageTool PRIVATE "/subsystem:windows")
