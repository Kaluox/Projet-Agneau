QT       += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

TARGET = Decter


TEMPLATE = app

SOURCES += main.cpp \
    soundwave.cpp \
    view.cpp \
    model.cpp \
    mainwindow.cpp \
    video.cpp \
    node.cpp \
    trajectory.cpp \
    objectinformation.cpp \
    shiftmaxwindow.cpp \
    objecthisrogramwindow.cpp \
    algocolors.cpp \
    algosubstraction.cpp \
    thresholdvideowindow.cpp \
    extractvideowindow.cpp \
    sound.cpp \
    controller.cpp \
    gestionvideo.cpp \
    QCustomPlot/qcustomplot.cpp \
    cascadestylesheetloader.cpp

HEADERS += \
    soundwave.h \
    view.h \
    model.h \
    mainwindow.h \
    video.h \
    node.h \
    trajectory.h \
    objectinformation.h \
    shiftmaxwindow.h \
    objecthisrogramwindow.h \
    algocolors.h \
    algosubstraction.h \
    thresholdvideowindow.h \
    extractvideowindow.h \
    sound.h \
    controller.h \
    gestionvideo.h \
    QCustomPlot/qcustomplot.h \
    cascadestylesheetloader.h

#si opencv non trouvé après installation exécuter la commande suivante en remplaçant le chemin par le votre : setx -m OPENCV_DIR C:\opencv\build\x86\vc12
INCLUDEPATH += "C:\opencv\build\include" \
INCLUDEPATH += "C:\ffmpeg-4.1.3-win64-dev\include" \
INCLUDEPATH += "C:\Program Files (x86)\Aquila\include\aquila"
CONFIG(debug, debug|release){
LIBS +=-LC:\\opencv-build\\install\\x86\\mingw\\lib -lopencv_calib3d410 -lopencv_contrib410 -lopencv_features2d410 -lopencv_flann410 -lopencv_highgui410 -lopencv_imgproc410 -lopencv_objdetect410 -lopencv_video410 -lopencv_core410 \
        -LC:\\opencv-build\\install\\x86\\mingw\\bin -lopencv_calib3d410 -lopencv_contrib410 -lopencv_features2d410 -lopencv_flann410 -lopencv_highgui410 -lopencv_imgproc410 -lopencv_objdetect410 -lopencv_video410 -lopencv_core410 \
        -LC:\\ffmpeg\\lib -lavformat -lavcodec -lavutil -lswscale -lswresample -lavfilter \
        -LC:\\ffmpeg\\bin -lavformat -lavcodec -lavutil -lswscale -lswresample -lavfilter
LIBS += "F:/informatique3/projetSILibrary/Libraries\\aquila\\lib\\debug\\aquila.lib"
LIBS += "F:/informatique3/projetSILibrary/Libraries\\aquila\\lib\\debug\\Ooura_fft.lib"
}
CONFIG(release, debug|release){
#LIBS += "C:\Program Files (x86)\Aquila\lib\libAquila.a"
#LIBS +=  "C:\Program Files (x86)\Aquila\lib\libOoura_fft.a"
LIBS += "C:\Program Files (x86)\Aquila\lib\aquila.lib"
LIBS +=  "C:\Program Files (x86)\Aquila\lib\Ooura_fft.lib"
LIBS += -LC:\opencv\build\lib\Release \
        -lopencv_calib3d410 \
        -lopencv_features2d410 \
        -lopencv_flann410 \
        -lopencv_highgui410 \
        -lopencv_imgproc410 \
        -lopencv_objdetect410 \
        -lopencv_video410 \
        -lopencv_videoio410 \
        -lopencv_core410

LIBS += -LC:\opencv\build\bin\Release \
        -lopencv_calib3d410 \
        -lopencv_features2d410 \
        -lopencv_flann410 \
        -lopencv_highgui410 \
        -lopencv_imgproc410 \
        -lopencv_objdetect410 \
        -lopencv_video410 \
        -lopencv_videoio410 \
        -lopencv_core410

LIBS += -LC:\ffmpeg-4.1.3-win64-dev\lib
        -lavformat
        -lavcodec
        -lavutil
        -lpostproc
        -lswscale
        -lswresample
        -lavfilter


}

RESOURCES += \
    resources.qrc

DISTFILES +=


