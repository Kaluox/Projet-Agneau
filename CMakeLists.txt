cmake_minimum_required(VERSION 3.13)
project(ProjetAgneau)

##### PARAMETERS #####

set(CMAKE_CXX_STANDARD 14)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")


##### ENVIRONMENT VARIABLES #####

#This sets the environment variable to locate Qt components
set(CMAKE_PREFIX_PATH "C:/Qt/5.12.6/msvc2017_64" ${CMAKE_PREFIX_PATH})
#This sets the environment variable to locate OpenCV components
set(CMAKE_PREFIX_PATH "C:/OpenCV2/opencv/build" ${CMAKE_PREFIX_PATH})
#This adds to the variable CMAKE_PREFIX_PATH the folder containing cmake files to find depencies
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_PREFIX_PATH})

set(CMAKE_MODULE_PATH C:/ffmpeg-4.2.1-win64-dev/include ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# PKG-CONFIG is necessary for the packages
set(PKG_CONFIG_EXECUTABLE "C:/pkg-config_0.26-1_win32/bin")

# Setting OpenCV path, if this doesn't work, link your own path. The version used is OpenCV 4.2
set(OPENCV_DIR "C:/OpenCV/build/include")
# Setting FFMPEG path, link you own path if this doesn't work. The version used is FFMpeg 4.2.1 dev.
set(FFMPEG_DIR "C:/ffmpeg-4.2.1-win64-dev/include")
# Setting Aquila path, link your own path if this doesn't work, the path is where the installation was.
set(AQUILA_ROOT "C:/aquila")
set(AQUILA_DIR "C:/aquila/aquila")


message("blep")


##### Finding Packages #####

# Find the QtWidgets packages
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt5 COMPONENTS multimediawidgets REQUIRED)
find_package(Qt5 COMPONENTS gui REQUIRED)
find_package(Qt5 COMPONENTS printsupport REQUIRED)
# Find the OpenCV packages
INCLUDE_DIRECTORIES( ${OPENCV_DIR} )
find_package(OpenCV)
# Finding FFMPEG Packages
INCLUDE_DIRECTORIES(${FFMPEG_DIR})
find_package(FFMPEG)
# Finding Aquila Packages
INCLUDE_DIRECTORIES(${AQUILA_DIR})
find_package(AQUILA)

#find_package(FFmpeg COMPONENTS AVCODEC AVFORMAT AVUTIL AVFILTER SWSCALE SWRESAMPLE REQUIRED)

#include_directories(${FFMPEG_INCLUDE_DIRS})



##### Source files #####

set(SOURCES
        main.cpp
        model/soundwave.cpp
        view/view.cpp
        model/model.cpp
        view/mainwindow.cpp
        model/video.cpp
        model/node.cpp
        model/trajectory.cpp
        model/objectinformation.cpp
        view/shiftmaxwindow.cpp
        view/objecthistogramwindow.cpp
        model/algocolors.cpp
        model/algosubstraction.cpp
        view/thresholdvideowindow.cpp
        view/extractvideowindow.cpp
        model/sound.cpp
        controller/controller.cpp
        model/gestionvideo.cpp
        view/qcustomplot.cpp
        view/cascadestylesheetloader.cpp)

# Adding the headers in the read files
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/model ${PROJECT_SOURCE_DIR}/view ${PROJECT_SOURCE_DIR}/controller)


##### EXECUTING PROJECT #####

add_executable(ProjetAgneau ${SOURCES})


### Linking Libraries ###

target_link_libraries(ProjetAgneau Qt5::Widgets)
target_link_libraries(ProjetAgneau Qt5::Multimedia)
target_link_libraries(ProjetAgneau Qt5::Core)
target_link_libraries(ProjetAgneau Qt5::MultimediaWidgets)
target_link_libraries(ProjetAgneau Qt5::Gui)
target_link_libraries(ProjetAgneau Qt5::PrintSupport)
target_link_libraries(ProjetAgneau OpenCV)
target_link_libraries(ProjetAgneau FFMPEG)
target_link_libraries(ProjetAgneau AQUILA)