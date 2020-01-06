cmake_minimum_required(VERSION 3.13)
project(Projet_Agneau)

set(CMAKE_CXX_STANDARD 14)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Multimedia REQUIRED)
find_library(OpenCV REQUIRED)
#message("bop")

include_directories("D:/ProjetAgneau/install")

include_directories(controller)
include_directories(model)
include_directories(view)

add_executable(Projet_Agneau
        controller/controller.cpp
        controller/controller.h
        model/algocolors.cpp
        model/algocolors.h
        model/algosubstraction.cpp
        model/algosubstraction.h
        model/gestionvideo.cpp
        model/gestionvideo.h
        model/model.cpp
        model/model.h
        model/node.cpp
        model/node.h
        model/objectinformation.cpp
        model/objectinformation.h
        model/sound.cpp
        model/sound.h
        model/soundwave.cpp
        model/soundwave.h
        model/trajectory.cpp
        model/trajectory.h
        model/video.cpp
        model/video.h
        view/cascadestylesheetloader.cpp
        view/cascadestylesheetloader.h
        view/extractvideowindow.cpp
        view/extractvideowindow.h
        view/mainwindow.cpp
        view/mainwindow.h
        view/qcustomplot.cpp
        view/qcustomplot.h
        view/qdetectfilter.cpp
        view/qdetectfilter.h
        view/shiftmaxwindow.cpp
        view/shiftmaxwindow.h
        view/thresholdvideowindow.cpp
        view/thresholdvideowindow.h
        view/view.cpp
        view/view.h
        main.cpp
        moc_controller.cpp
        moc_mainwindow.cpp
        moc_qcustomplot.cpp
        moc_video.cpp
        qrc_resources.cpp)

target_link_libraries(Projet_Agneau Qt5::Widgets)
target_link_libraries(Projet_Agneau Qt5::Multimedia)
target_link_libraries(Projet_Agneau ${OpenCV_LIBS})