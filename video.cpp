#include "video.h"
#include <QObject>
#include <conio.h>
#include <time.h>
#include <QDebug>
#include <global.h>
#include <source/WaveFile.h>
#include <QPainter>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include "cascadestylesheetloader.h"

Video::Video() : QThread() {
    algoColors = new AlgoColors() ;
    algoIDtoRun = 0 ;
    //minHue = 36 ; maxHue = 88 ;
    minHue = 100; maxHue = 15;
    //minSaturation = 20 ; maxSaturation = 255 ;
    minSaturation = 150; maxSaturation = 255;
    //minValue = 83 ; maxValue = 255 ;
    minValue = 83 ; maxValue = 255;
    CascadeStyleSheetLoader& loader = CascadeStyleSheetLoader::getInstance();

    QColor themeColor(loader.getProperty("#accentColor"));

    red = themeColor.red();
    green = themeColor.green();
    blue = themeColor.blue();
}

bool Video::loadVideo(std::string filename){
    fullpath = filename;

    //qDebug() << filename.c_str();

    videoCapture.open(filename) ;
    if(videoCapture.isOpened()){
        //videoCapture.set(720,CV_CAP_PROP_FRAME_HEIGHT);
        //std::cout << "PINGAS" << std::endl;
        //videoCapture.set(1280,CV_CAP_PROP_FRAME_WIDTH);
        //qDebug() << videoCapture.get(CV_CAP_PROP_FPS);
        //videoCapture.set(30,CV_CAP_PROP_FPS);

        //qDebug() << videoCapture.get(CV_CAP_PROP_FPS);
        framerate = videoCapture.get(CV_CAP_PROP_FPS);
        //framerate = 1000;
        videoCapture.set(CV_CAP_PROP_POS_FRAMES, 1);
        if(videoCapture.read(frame))
        {
            firstFrame =  frame ;
            //startFrame = mat2QImage(frame);
            //endFrame = getLastFrame();
        }
        else
            return false ;
        //nbFrames = videoCapture.get(CV_CAP_PROP_FRAME_COUNT);
        //framerate = 6;
        stopBool = false;
        return true ;
    }
    return false ;
}
/**
void Video::play(){
    if(!isRunning()){
        if(isStopped())
            stopBool = false ;
    }
    else {

    }
}
**/
void Video::play(){
    if(!isRunning()){
        if(isStopped())
            stopBool = false ;
        start(LowPriority) ;
    }
}

void Video::stop(){stopBool = true ;}

bool Video::isStopped(){ return stopBool ;}

void Video::resume() {
    pauseAndResumeMutex.lock();
    qDebug() << "resume";
    stopBool = false;
    pauseAndResumeMutex.unlock();
    pauseCond.wakeAll();
}

void Video::pause()
{
    pauseAndResumeMutex.lock();
    qDebug() << "pause";
    stopBool = true;
    pauseAndResumeMutex.unlock();
}

void Video::run(){
    //qDebug()<< "video : " << QThread::currentThreadId();
    double startTime = videoCapture.get(CV_CAP_PROP_POS_MSEC);
    QTime timer;
    timer.start();
    while(!stopBool){
        //qDebug() << "video : " << videoCapture.get(CV_CAP_PROP_POS_FRAMES);
        if(videoCapture.get(CV_CAP_PROP_POS_FRAMES) == videoCapture.get(CV_CAP_PROP_FRAME_COUNT)-1)
        {
            stopBool = true ;
            emit processedImage(getFirstFrame(), 1, QString::fromStdString(info.str()));
            qDebug() << "That's all folks !";
        }
        else{
            info.str("") ;

            videoCapture.read(frame);
            //resize(frame, frame, cv::Size(1280, 720), 0, 0, cv::INTER_CUBIC);
            //videoCapture.grab();
            //videoCapture.retrieve(frame);
            if(trajectoryChecked){
                switch(algoIDtoRun){
                case 0:
                    if(!keepRunning){
                        algoSoustraction = new AlgoSubstraction(shiftMax, firstFrame, &choosenObject, keepRunning) ;
                        keepRunning = true ;
                    }
                    algoSoustraction->run(frame, getCurrentFrameID(), x, y);
                    drawObject(x,y, frame);
                    //qDateTime = QDateTime::currentDateTime() ; modification de la date de la trajectoire prise sur l'heure du PC
                    qDateTime.setMSecsSinceEpoch(videoCapture.get(CV_CAP_PROP_POS_MSEC));
                    info << "(" << x << "," << y << ") : " << qDateTime.toString("mm:ss").toStdString();
                    break;
                case 1:
                    cvtColor(frame, HSV, cv::COLOR_BGR2HSV);

                    // Si le rang de couleur est normal l'algo classique est utilisÃ©
                    if (minHue <= maxHue)
                    {
                        inRange(HSV, cv::Scalar(minHue, minSaturation, minValue), cv::Scalar(maxHue, maxSaturation, maxValue), threshold);
                    }
                    // Si le minimum est plus grand que le maximum, le rang est coupÃ© en deux, et les rÃ©sultats sont fusionnÃ©s
                    else
                    {
                        cv::Mat mat1, mat2;
                        inRange(HSV, cv::Scalar(minHue, minSaturation, minValue), cv::Scalar(179, maxSaturation, maxValue), mat1);
                        inRange(HSV, cv::Scalar(0, minSaturation, minValue), cv::Scalar(maxHue, maxSaturation, maxValue), mat2);
                        threshold = mat1 | mat2;
                    }

                    algoColors->morphObs(threshold);
                    emit processedThresholdImage(mat2QImage(threshold));
                    if(algoColors->trackFilteredObject(x, y, threshold, frame)){
                        drawObject(x,y, frame);
                        qDateTime = QDateTime::currentDateTime() ; // modification de la date de la trajectoire prise sur l'heure du PC
                        qDateTime.setMSecsSinceEpoch(videoCapture.get(CV_CAP_PROP_POS_MSEC));
                        info << x << "," << y << "," << qDateTime.toString("mm:ss").toStdString();
                    }
                    break;
                }
            }
            double localTime = videoCapture.get(CV_CAP_PROP_POS_MSEC);
            qDebug() << timer.elapsed() << startTime << localTime;
            if(timer.elapsed() + startTime < localTime)
            {
                QThread::msleep(localTime - timer.elapsed() - startTime) ;
            }
            //QThread::msleep(framerate/1000);
            emit processedImage(mat2QImage(frame), getCurrentFrameID(), QString::fromStdString(info.str()));
        }
    }
    //qDebug() << "The cake is a lie !";
}

double Video::getSecFromNbFrame(double nbFrames){return nbFrames/(double)framerate ;}
int Video::getCurrentFrameID(){ return videoCapture.get(CV_CAP_PROP_POS_FRAMES) ;}
void Video::setCurrentFrame(int frameNumber){videoCapture.set(CV_CAP_PROP_POS_FRAMES, frameNumber) ;}
int Video::getNumberOfFrames(){ return videoCapture.get(CV_CAP_PROP_FRAME_COUNT) ;}
void Video::setTrajectoryChecked(bool checked){ trajectoryChecked = checked ;}
bool Video::getTrajectoryChecked(){return trajectoryChecked ;}
void Video::setShiftMax(int shiftMax){this->shiftMax = shiftMax ; }
void Video::setVisualize(bool visualize){this->visualize = visualize ;}
void Video::setTreatment(bool treatment){this->treatment = treatment ;}
void Video::isFirstFrame(bool boolFirstFrame){this->boolFirstFrame = boolFirstFrame ;}
void Video::toKeepRunning(bool keepRunning){this->keepRunning = keepRunning ; }
void Video::setAlgoIDtoRun(int ID){ algoIDtoRun = ID ;}

int Video::getMsecPos() {
    return videoCapture.get(CV_CAP_PROP_POS_MSEC);
}

QImage Video::getFirstFrame(){
    //qDebug()<< "first frame : " << QThread::currentThreadId();
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, 0) ;
    emit processedImage(mat2QImage(frame), getCurrentFrameID(), "");
    //qDebug() << "hahaha";
    if(videoCapture.read(frame))
        return mat2QImage(frame) ;
    return (QImage)NULL;
}

QImage Video::getCurrentFrame(){
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, videoCapture.get(CV_CAP_PROP_POS_FRAMES)-1) ;
    if(videoCapture.read((frame)))
        return mat2QImage(frame) ;
    return (QImage)NULL;
}

QImage Video::getPreviousFrame(){
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, videoCapture.get(CV_CAP_PROP_POS_FRAMES) - 2) ;
    if(videoCapture.read((frame)))
        return mat2QImage(frame) ;
    return (QImage)NULL ;
}

QImage Video::getPreviousFrameExtended(int extension){
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, videoCapture.get(CV_CAP_PROP_POS_FRAMES) - extension) ;
    if(videoCapture.read((frame)))
        return mat2QImage(frame) ;
    return (QImage)NULL ;
}

QImage Video::getNextFrame(){
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, videoCapture.get(CV_CAP_PROP_POS_FRAMES)) ;
    if(videoCapture.read((frame)))
        return mat2QImage(frame) ;
    return (QImage)NULL ;
}

QImage Video::getNextFrameExtended(int extension){
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, videoCapture.get(CV_CAP_PROP_POS_FRAMES) + extension) ;
    if(videoCapture.read((frame)))
        return mat2QImage(frame) ;
    return (QImage)NULL ;
}

QImage Video::getLastFrame(){
    //qDebug() << "last frame : " << QThread::currentThreadId();
    videoCapture.set(CV_CAP_PROP_POS_FRAMES, videoCapture.get(CV_CAP_PROP_FRAME_COUNT)-2);
    emit processedImage(mat2QImage(frame), getCurrentFrameID(), "");
    if(videoCapture.read((frame)))
        return mat2QImage(frame) ;
    return (QImage)NULL ;
}

QImage Video::mat2QImage(const cv::Mat &src){
    cv::Mat temp ;
    if(src.channels() == 3){
        cvtColor(src, temp, cv::COLOR_BGR2RGB) ;
        QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        dest.bits() ;
        return dest ;
    }
    if(src.channels() == 1){
        QVector<QRgb>  sColorTable;
        for ( int i = 0; i < 256; ++i )
            sColorTable.push_back( qRgb( i, i, i ) );
        QImage dest( src.data, src.cols, src.rows, src.step, QImage::Format_Indexed8 );
        dest.setColorTable( sColorTable );
        return dest ;
    }

    return (QImage)NULL ;
}

QImage Video::getImageMousePressed(int x, int y, int qVideoLabelHeight, int qVideoLabelWidth){
    frame.copyTo(imageOriginal);
    previousPoint = cv::Point(x*(videoCapture.get(CV_CAP_PROP_FRAME_WIDTH)/qVideoLabelWidth), y*(videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT)/qVideoLabelHeight)) ;
    std::stringstream s ;
    s << "(" << x << "," << y << ")" ;
    putText(imageOriginal,s.str().c_str(),previousPoint, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255,0,0,0),2,8);
    circle(imageOriginal, previousPoint,2,cv::Scalar(255,0,0,0), cv::FILLED, cv::LINE_AA,0) ;
    return mat2QImage(imageOriginal) ;
}

QImage Video::getImageMousePressedAndMoving(int x, int y, int qVideoLabelHeight, int qVideoLabelWidth){
    imageOriginal.copyTo(imageTmp);
    if(previousPoint.x != -1 && previousPoint.y != -1)
    {
        currentPoint = cv::Point(x*(videoCapture.get(CV_CAP_PROP_FRAME_WIDTH)/qVideoLabelWidth), y*(videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT)/qVideoLabelHeight)) ;

        std::stringstream s ;
        s << "(" << x << "," << y << ")" ;

        putText(imageTmp,s.str().c_str(),currentPoint, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255,0,0,0),2,8);
        rectangle(imageTmp, previousPoint, currentPoint, cv::Scalar(0,255,0,0),2,8,0);
        return mat2QImage(imageTmp) ;
    }
    return (QImage)NULL;
}

QImage Video::getImageMouseReleased(int x, int y, int qVideoLabelHeight, int qVideoLabelWidth){
    imageTmp.copyTo(imageOriginal);
    //currentPoint = Point(x*(videoCapture.get(CV_CAP_PROP_FRAME_WIDTH)/qVideoLabelWidth), y*(videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT)/qVideoLabelHeight)) ;
    if(previousPoint.x != -1 && previousPoint.y != -1)
    {
        std::stringstream s ;
        s << "(" << x << "," << y << ")" ;

        //putText(imageOriginal,s.str().c_str(),currentPoint, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255,0,0,0),2,8);
        //circle(imageOriginal, previousPoint,2,Scalar(255,0,0,0),CV_FILLED, CV_AA,0) ;

        imageOriginal.copyTo(imageTmp);
        cvtColor(frame(cv::Rect(min(currentPoint.x, previousPoint.x), min(currentPoint.y, previousPoint.y), abs(previousPoint.x - currentPoint.x) , abs(previousPoint.y - currentPoint.y))), choosenObject, cv::COLOR_BGR2RGB) ;
        boolFirstFrame = false ;
        previousPoint = cv::Point(-1, -1);
        return mat2QImage(imageOriginal) ;
    }
    return (QImage)NULL;
}

int Video::getRedMin(){return redMin ;}
int Video::getRedMax(){return redMax ;}
int Video::getGreenMin(){return greenMin ;}
int Video::getGreenMax(){return greenMax ;}
int Video::getBlueMin(){return blueMin ;}
int Video::getBlueMax(){return blueMax ;}

QImage Video::getHistRed(){return qImageHistRed ;}
QImage Video::getHistGreen(){return qImageHistGreen ;}
QImage Video::getHistBlue(){return qImageHistBlue ;}

void Video::setMinHue(int minHue){this->minHue = minHue ;}
void Video::setMinSaturation(int minSaturation){this->minSaturation = minSaturation ;}
void Video::setMinValue(int minValue){this->minValue = minValue ;}

void Video::setMaxHue(int maxHue){this->maxHue = maxHue ;}
void Video::setMaxSaturation(int maxSaturation){this->maxSaturation = maxSaturation ;}
void Video::setMaxValue(int maxValue){this->maxValue = maxValue ;}

void Video::setFrameStartExtraction(int start){frameStartExtraction = start ;}
void Video::setFrameEndExtraction(int end){frameEndExtraction = end ;}

void Video::extractVideo(QString filename){

    QTime tStart(0,0,0) ;
    tStart = tStart.addSecs(getSecFromNbFrame(frameStartExtraction)) ;

    QTime timelaps(0,0,0) ;
    timelaps = timelaps.addSecs(getSecFromNbFrame(frameEndExtraction - frameStartExtraction)) ;

    QTime tEnd(0,0,0) ;
    tEnd = tEnd.addSecs(getSecFromNbFrame(frameEndExtraction)) ;

    /*Extract a video from 00:00:30 to 00:00:35 (+00:00:05)
        ffmpeg -ss 00:00:30 -i orginalfile -t 00:00:05 -vcodec copy -acodec copy newfile*/
    std::stringstream cmd ;
    /*cmd << "ffmpeg -ss " << tStart.toString("hh:mm:ss").toStdString()
        << " -i " << filename.toStdString() << ".mp4"
        << " -t " << timelaps.toString("hh:mm:ss").toStdString() << " -vcodec copy -acodec copy "
        << filename.toStdString().c_str() << "_" << tStart.toString("hh-mm-ss").toStdString() << "_" << tEnd.toString("hh-mm-ss").toStdString() << ".mp4";
    qDebug() << cmd.str().c_str() ;
    system(cmd.str().c_str()) ;*/

    cmd.str("") ;
    QString filenameOut =  filename+ "_" + tStart.toString("hh-mm-ss") + "_" + tEnd.toString("hh-mm-ss") + ".mp4";
    QString filenameWav = filename+ "_" + tStart.toString("hh-mm-ss") + "_" + tEnd.toString("hh-mm-ss") + ".wav";
    QString filenamePng = filename+ "_" + tStart.toString("hh-mm-ss") + "_" + tEnd.toString("hh-mm-ss") + ".png";
    GestionVideo gestionExtract(fullpath);
    gestionExtract.extractVideo(filenameOut.toStdString().c_str(), tStart.hour()*3600 + tStart.minute()*60 + tStart.second(), tEnd.hour()*3600 + tEnd.minute()*60 + tEnd.second());
    GestionVideo gestionExtractAudio(filenameOut.toStdString());
    gestionExtractAudio.extractAudio(filenameWav.toStdString());

    /*cmd << "ffmpeg -i " << filename.toStdString().c_str()  << "_" << tStart.toString("hh-mm-ss").toStdString() << "_" << tEnd.toString("hh-mm-ss").toStdString() << ".mp4"
        << " -vn -ar 44100 -ac 2 -ab 192k -f wav "
        << filename.toStdString().c_str()  << "_" << tStart.toString("hh-mm-ss").toStdString() << "_" << tEnd.toString("hh-mm-ss").toStdString() << ".wav" ;
    qDebug() << cmd.str().c_str() ;
    system(cmd.str().c_str()) ;

    cmd.str("") ;*/
    if(QFile(filenameWav).exists()){
        unsigned int width = 1280;
        unsigned int height = 120;
        Aquila::WaveFile wav(filenameWav.toStdString());
        double max = 0;
        unsigned int total = wav.getSamplesCount()/width;
        std::vector<double> result;
        double calcul = 0;
        for(unsigned int i = 0; i < wav.getSamplesCount(); i++){
            if(i%total == total - 1)
            {
                result.push_back(calcul/total);
                if(calcul/total > max)
                    max = calcul/total;
                calcul = 0;
            }
            else
            {
                calcul += qAbs(wav.sample(i));
            }
        }
        QImage image(width,height,QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPainter paint;
        paint.begin(&image);
        paint.setPen(QColor(255, 0, 0));
        for(unsigned int i = 0; i < result.size(); i++)
        {
            double value;
            if(max >=  (height/2)){
                value = result.at(i)*(height/2) / max;
            }
            else {
                value = result.at(i);
            }
            if(value > 0){
                paint.drawLine(QLineF(QPointF(i, (height/2)), QPointF(i, value + (height/2))));
                paint.drawLine(QLineF(QPointF(i, 30), QPointF(i,  (height/2) - value)));
            }

        }

        paint.end();
        image.save(filenamePng);
    }

    /*cmd << "ffmpeg -i " << filename.toStdString().c_str()  << "_" << tStart.toString("hh-mm-ss").toStdString() << "_" << tEnd.toString("hh-mm-ss").toStdString() << ".mp4"
        << " -filter_complex \"aformat=channel_layouts=mono,showwavespic=s=1280x60\" -frames:v 1 "
        << filename.toStdString().c_str()  << "_" << tStart.toString("hh-mm-ss").toStdString() << "_" << tEnd.toString("hh-mm-ss").toStdString() << ".png" ;
    qDebug() << cmd.str().c_str() ;
    system(cmd.str().c_str()) ;*/
}

void Video::computeHistogramChosenObject(){
    redMin = 0, greenMin = 0, blueMin = 0 ;
    redMax = 255, greenMax = 255, blueMax = 255 ;
    // Separate the image in 3 places ( B, G and R )
    vector<cv::Mat> bgr_planes;
    split( choosenObject, bgr_planes );

    // Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;

    // Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Calcule max and min value
    // obtain the minimum value on the abscissa
    while(redMin < 255 && r_hist.at<float>(redMin) < 5)
        redMin++;

    while(greenMin < 255 && g_hist.at<float>(greenMin) < 5)
        greenMin++;

    while(blueMin < 255 && b_hist.at<float>(blueMin) < 5)
        blueMin++;

    // obtain the maximum value on the abscissa
    while(redMax >=0 && r_hist.at<float>(redMax) < 5)
        redMax--;

    while(greenMax >=0 && g_hist.at<float>(greenMax) < 5)
        greenMax--;

    while(blueMax >= 0 && b_hist.at<float>(blueMax) < 5)
        blueMax--;


    // Draw the histograms for B, G and R
    //int hist_w = 512; int hist_h = 400;
    int hist_w = 212; int hist_h = 150;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat histImageRed( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
    cv::Mat histImageGreen( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
    cv::Mat histImageBlue( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

    // Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImageBlue.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(g_hist, g_hist, 0, histImageGreen.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(r_hist, r_hist, 0, histImageRed.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    // Draw for each channel
    for( int i = 1; i < histSize; i++ ){
        line( histImageBlue, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
              cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              cv::Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImageGreen, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
              cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              cv::Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImageRed, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
              cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              cv::Scalar( 0, 0, 255), 2, 8, 0  );
    }

    cvtColor(histImageRed,histImageRed,cv::COLOR_BGR2RGB);
    cvtColor(histImageGreen,histImageGreen,cv::COLOR_BGR2RGB);
    cvtColor(histImageBlue,histImageBlue,cv::COLOR_BGR2RGB);

    qImageHistRed = mat2QImage(histImageRed) ;
    qImageHistGreen = mat2QImage(histImageGreen) ;
    qImageHistBlue = mat2QImage(histImageBlue) ;

}

void Video::drawObject(int x, int y, cv::Mat &frame){
    int width = videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
    //putText(frame, "Tracking Object", Point(0, 50), 2, 1, (0, 255, 0), 2);
    circle(frame, cv::Point(x, y), 40, cv::Scalar(blue, green, red), 2);
    if (y - 25>0)
        line(frame, cv::Point(x, y), cv::Point(x, y - 25), cv::Scalar(blue, green, red), 2);
    else line(frame, cv::Point(x, y),cv::Point(x, 0), cv::Scalar(blue, green, red), 2);
    if (y + 25<height)
        line(frame, cv::Point(x, y), cv::Point(x, y + 25), cv::Scalar(blue, green, red), 2);
    else line(frame, cv::Point(x, y),cv::Point(x, height), cv::Scalar(blue, green, red), 2);
    if (x - 25>0)
        line(frame, cv::Point(x, y), cv::Point(x - 25, y), cv::Scalar(blue, green, red), 2);
    else line(frame, cv::Point(x, y), cv::Point(0, y), cv::Scalar(blue, green, red), 2);
    if (x + 25<width)
        line(frame, cv::Point(x, y), cv::Point(x + 25, y), cv::Scalar(blue, green, red), 2);
    else line(frame, cv::Point(x, y), cv::Point(width, y), cv::Scalar(blue, green, red), 2);
    QString text = QString::number(x) + "," + QString::number(y);
    putText(frame, text.toStdString(), cv::Point(x, y + 30), 1, 1, cv::Scalar(blue, green, red), 2);
}

double Video::getFramerate()
{
    return framerate;
}

int Video::timeVideo()
{
    return videoCapture.get(CV_CAP_PROP_POS_MSEC)/1000;
}

Video::~Video(){
    destructorMutex.lock();
    stopBool = true ;
    videoCapture.release();
    condition.wakeOne();
    destructorMutex.unlock();
    wait() ;

    delete algoColors ;
}
