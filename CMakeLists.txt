# set the project name
project(Projet-Agneau)


#Used to add external librairies necessary to the project
FIND_PACKAGE(Qt4 REQUIRED)

#Version of CMake
cmake_minimum_required(VERSION 3.13.4)

#find_package(Qt4 COMPONENTS Widgets REQUIRED)
foreach(plugin ${Qt5Network_PLUGINS})
    get_target_property(_loc ${plugin} LOCATION)
    message("Plugin ${plugin} is at location ${_loc}")
endforeach()


set(CMAKE_CXX_STANDARD 14)

# add the executable
#add_executable(LambTracker main.cpp)

#Create variables with files to compile
set(SRCS main.cpp
        model/soundwave.cpp
        view/view.cpp
        model/model.cpp
        view/mainwindow.cpp
        model/video.cpp
        model/node.cpp
        model/trajectory.cpp
        model/objectinformation.cpp
        view/shiftmaxwindow.cpp
        model/objecthistogramwindow.cpp
        model/algocolors.cpp
        model/algosubstraction.cpp
        view/thresholdvideowindow.cpp
        view/extractvideowindow.cpp
        model/sound.cpp
        controller/controller.cpp
        model/gestionvideo.cpp
        view/qcustomplot.cpp
        view/cascadestylesheetloader.cpp)

set(HEADERS model/soundwave.h
            view/view.h
            model/model.h
            view/mainwindow.h
            model/video.h
            model/node.h
            model/trajectory.h
            model/objectinformation.h
            view/shiftmaxwindow.h
            model/objecthistogramwindow.h
            model/algocolors.h
            model/algosubstraction.h
            view/thresholdvideowindow.h
            view/extractvideowindow.h
            model/sound.h
            controller/controller.h
            model/gestionvideo.h
            view/qcustomplot.h
            view/cascadestylesheetloader.h)

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})

#Include repertories of Qt
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable(LambTracker ${HEADERS_MOC} ${SRCS})
TARGET_LINK_LIBRARIES(Projet-Agneau ${QT_LIBRARIES})