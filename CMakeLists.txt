cmake_minimum_required(VERSION 3.5)
project(ImageDedup)

set(CMAKE_CXX_STANDARD 11)

# Boost
find_package(Boost COMPONENTS filesystem timer program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES} -lboost_filesystem -lboost_system)

# OpenCV
find_package( OpenCV REQUIRED )
if (OpenCV_FOUND)
    message("-- Found OpenCV")
    message("-- Linking with OpenCV libraries: ${OpenCV_LIBS}")
    link_libraries(${OpenCV_LIBS})
else (OpenCV_FOUND)
    message("-- OpenCV not found.")
endif()

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
link_libraries(${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

# Eigen
find_package(Eigen3)
include_directories(${EIGEN_INCLUDE_DIR})

# Qt5
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Test)

add_library(imagededup-ioutils ioutils.cpp)
target_link_libraries(imagededup-ioutils
        Qt5::Core
        Qt5::Widgets
        Qt5::OpenGL
        Qt5::Test)

add_library(imagededuper imagededuper.cpp)
target_link_libraries(imagededuper
        imagededup-ioutils
        pHash
        Qt5::Core
        Qt5::Widgets
        Qt5::OpenGL
        Qt5::Test)

set(SOURCE_FILES main.cpp)
add_executable(ImageDedup ${SOURCE_FILES})
target_link_libraries(ImageDedup
        imagededup-ioutils
        imagededuper)
