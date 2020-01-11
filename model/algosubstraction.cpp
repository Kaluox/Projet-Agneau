#include "algosubstraction.h"
#include <QImage>
#include <QDebug>
#include <time.h>
#include <conio.h>



/// Constructor of algosubstraction
AlgoSubstraction::AlgoSubstraction(int shiftMax, cv::Mat& startFrame, cv::Mat* choosenObject, int nbFrame)
{
    biggestSurface=0;
    biggestSurfaceId=0;
    count_refond=0;
    this->shiftMax = shiftMax;
    this->startFrame = startFrame;
    this->choosenObject = choosenObject;
    //Initialiser la limite inférieure et la limite supérieure
    thresholdRedMin = 0;
    thresholdRedMax = 255;
    thresholdBlueMin = 0;
    thresholdBlueMax = 255;
    thresholdGreenMin = 0;
    thresholdGreenMax = 255;

    //Calculer la limite inférieure et la limite supérieure
    computeHistogram(*choosenObject);
    //Générer l'image binaire de la prémière image de la vidéo
    binaryBackground = generateBinaryImage(startFrame);

    firstFrame = (nbFrame == 0 ) ? true : false ;
}



void AlgoSubstraction::run(cv::Mat & currentFrame, int nbFrame, int& x, int& y)
{
    clock_t t_start,t_end, t_start1, t_end1;
    cv::Mat binary_frame;   //the current frame on binary
    cv::Mat img_act;        //the binary image which contains only the moving object
    cv::Mat clean_act;      //img_act do erosion = clean_act
    cv::Point2f center;     //the center of the circle
    float radius;       //the radius of the circle
    cv::Mat obj_courant;    //current object
    cv::Mat drawing = cv::Mat::zeros( currentFrame.size(), CV_8UC3 );
    t_start = clock();
    // Générer l'image binaire du frame courant
    binary_frame = generateBinaryImage(currentFrame);
    t_end = clock();
    t_start1 = clock();

    /******foreground OU background – background =
     * the binary image which contains only the moving object*************/
    absdiff(binary_frame+binaryBackground,binaryBackground,img_act);

    // Erode the frame
    morphologyEx(img_act,clean_act,cv::MORPH_OPEN,element);

    // Find the contours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours(clean_act, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE );
    myObject.setContours(contours);
    t_end1 = clock();


    // Determine whether or not the object moves out of range of video
    if(contours.size() == 0){
        if(!myTrajectory.getCenterList().empty())
            myTrajectory.setLastCenter(myTrajectory.getCenterList().back());
        myTrajectory.getCenterList().clear();
    }else if(contours.size() >= 1){
        // Find the biggest Connected domain
        for( unsigned int i = 0; i < contours.size(); i++ )
            if(contourArea(contours[i])>biggestSurface){
                biggestSurface = contourArea(contours[i]);
                biggestSurfaceId = i;
            }
        //define a color
        cv::Scalar color( rand()&255, rand()&255, rand()&255 );
        //computes the minimal enclosing circle for the biggest Connected domain
        minEnclosingCircle( (cv::Mat)contours[biggestSurfaceId], center, radius);
        biggestSurface = 0;
        biggestSurfaceId = 0;
        if(!myTrajectory.getCenterList().empty()){
             double x = myTrajectory.getCenterList().back().getCenter().x;
             double y = myTrajectory.getCenterList().back().getCenter().y;
            if(sqrt((x-center.x)*(x-center.x) + (y-center.y)*(y-center.y))<shiftMax){
                Node nodecenter(center, QDateTime::currentDateTime(), nbFrame);
                myTrajectory.addPoint(nodecenter);
                double obj_x = center.x - radius*2/3;
                double obj_y = center.y - radius*2/3;
                if(obj_x < 0 )obj_x = 0;
                if(obj_y < 0) obj_y = 0;
                obj_courant = currentFrame(cv::Rect((int)(obj_x),(int)(obj_y),
                                             (int)radius*4/3,(int)radius*4/3));
                //this->calculeHistogram(obj_courant);
                *choosenObject = obj_courant;
            //Clear trajectoire
            }else
                myTrajectory.getCenterList().clear();
        }else{
            //If it is the first image
            if(firstFrame){
                firstFrame = false;
                Node nodecenter(center, QDateTime::currentDateTime(), nbFrame);
                myTrajectory.addPoint(nodecenter);
                //circle( currentFrame, center, (int)radius, color, 2, 8, 0 );//draw circle
                //circle( currentFrame, center, 2, color, -1, 8, 0 );//draw the center of circle
                x = center.x ;
                y = center.y ;
            //If it moves too fast in the last image
            }else if(sqrt((myTrajectory.getLastCenter().getCenter().x-center.x)*(myTrajectory.getLastCenter().getCenter().x-center.x)
                     + (myTrajectory.getLastCenter().getCenter().y-center.y)*(myTrajectory.getLastCenter().getCenter().y-center.y))<shiftMax*3/2)
            {
                Node nodecenter(center, QDateTime::currentDateTime(), nbFrame);
                myTrajectory.addPoint(nodecenter);
                //circle( currentFrame, center, (int)radius, color, 2, 8, 0 );
                //circle( currentFrame, center, 2, color, -1, 8, 0 );
                x = center.x ;
                y = center.y ;
            //If it display again in the screen
            }else{
                if(count_refond<=2){
                    binaryBackground = generateBinaryImage(currentFrame);
                    count_refond++;
                }else{
                    count_refond=0;
                    if(center.x > 0 && center.y > 0){
                        Node nodecenter(center, QDateTime::currentDateTime(), nbFrame);
                        myTrajectory.addPoint(nodecenter);
                        //circle( currentFrame, center, (int)radius, color, 2, 8, 0 );
                        //circle( currentFrame, center, 2, color, -1, 8, 0 );
                        x = center.x ;
                        y = center.y ;
                    }
                }
            }

        }
    }
}



///
/// This function takes an image and returns its binary image by calculating its binaryRedMax, binaryGreenMax and binaryBlueMax values.
///
cv::Mat AlgoSubstraction::generateBinaryImage(cv::Mat& tmp)
{
    cv::Mat binary;
    cv::Mat mv_fond[3];
    split(tmp, mv_fond);

    cv::Mat binaryRedMin;
    threshold(mv_fond[2], binaryRedMin, thresholdRedMin, 255, cv::THRESH_BINARY_INV);
    cv::Mat binaryRedMax;
    threshold(mv_fond[2], binaryRedMax, thresholdRedMax, 255, cv::THRESH_BINARY);
    cv::Mat binaryRed = binaryRedMin + binaryRedMax;

    cv::Mat binaryGreenMin;
    threshold(mv_fond[1], binaryGreenMin, thresholdGreenMin, 255, cv::THRESH_BINARY_INV);
    cv::Mat binaryGreenMax;
    threshold(mv_fond[1], binaryGreenMax, thresholdGreenMax, 255, cv::THRESH_BINARY);
    cv::Mat binaryGreen = binaryGreenMin + binaryGreenMax;

    cv::Mat binaryBlueMin;
    threshold(mv_fond[0], binaryBlueMin, thresholdBlueMin, 255, cv::THRESH_BINARY_INV);
    cv::Mat binaryBlueMax;
    threshold(mv_fond[0], binaryBlueMax, thresholdBlueMax, 255, cv::THRESH_BINARY);
    cv::Mat binaryBlue = binaryBlueMin + binaryBlueMax;

    threshold(binaryBlue+binaryGreen+binaryRed, binary, 254, 255, cv::THRESH_BINARY_INV);
    return binary;
}

/// Computes the histogram
void AlgoSubstraction::computeHistogram(cv::Mat & src){
    // Separate the image in 3 places ( B, G and R )
    std::vector<cv::Mat> bgr_planes;
    double maxRedVal = 0, maxGreenVal = 0, maxBlueVal = 0;
    double redVal = 0, greenVal = 0, blueVal = 0;
    split( src, bgr_planes );

    // Establish the number of bins
    int histSize = 256;

    // Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;

    // Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Calcule max and min value
    // obtain the maximum (peak) value for each channel
    for (int j = 0; j < 256; j++){
        if(b_hist.at<float>(j) > maxBlueVal ){
            maxBlueVal = b_hist.at<float>(j);
            blueVal = j;
        }
        if(g_hist.at<float>(j) > maxGreenVal ){
            maxGreenVal = g_hist.at<float>(j);
            greenVal = j;
        }
        if(r_hist.at<float>(j) > maxRedVal ){
          maxRedVal = r_hist.at<float>(j);
          redVal = j;
        }
    }
      // obtain the minimum value on the abscissa
      while(thresholdRedMin < 256 && r_hist.at<float>(thresholdRedMin) <= 5)
          thresholdRedMin++;

      while(thresholdGreenMin < 256 && g_hist.at<float>(thresholdGreenMin) <= 5)
          thresholdGreenMin++;

      while(thresholdBlueMin < 256 && b_hist.at<float>(thresholdBlueMin) <= 5)
          thresholdBlueMin++;


      // obtain the maximum value on the abscissa
      while(thresholdRedMax >= 0 && r_hist.at<float>(thresholdRedMax) <= 5)
          thresholdRedMax--;

      while(thresholdGreenMax >= 0 && g_hist.at<float>(thresholdGreenMax) <= 5)
          thresholdGreenMax--;

      while(thresholdBlueMax >= 0 && b_hist.at<float>(thresholdBlueMax) <= 5)
          thresholdBlueMax--;


      // calcule six values
      thresholdRedMin=(int)(thresholdRedMin-15);
      thresholdRedMax=(int)(thresholdRedMax+15);
      if((thresholdRedMax-redVal)>3*(redVal-thresholdRedMin)) thresholdRedMax=redVal+3*(redVal-thresholdRedMin);
      if((redVal-thresholdRedMin)>3*(thresholdRedMax-redVal)) thresholdRedMin=redVal-3*(thresholdRedMax-redVal);
      if(thresholdRedMin<=0)thresholdRedMin=1;
      if(thresholdRedMax>=256)thresholdRedMax=255;

      thresholdGreenMin=(int)(thresholdGreenMin-15);
      thresholdGreenMax=(int)(thresholdGreenMax+15);
      if((thresholdGreenMax-greenVal)>3*(greenVal-thresholdGreenMin)) thresholdGreenMax=greenVal+3*(greenVal-thresholdGreenMin);
      if((greenVal-thresholdGreenMin)>3*(thresholdGreenMax-greenVal)) thresholdGreenMin=greenVal-3*(thresholdGreenMax-greenVal);
      if(thresholdGreenMin<=0)thresholdGreenMin=1;
      if(thresholdGreenMax>=256)thresholdGreenMax=255;

      thresholdBlueMin=(int)(thresholdBlueMin-15);
      thresholdBlueMax=(int)(thresholdBlueMax+15);
      if((thresholdBlueMax-blueVal)>3*(blueVal-thresholdBlueMin)) thresholdBlueMax=blueVal+3*(blueVal-thresholdBlueMin);
      if((blueVal-thresholdBlueMin)>3*(thresholdBlueMax-blueVal)) thresholdBlueMin=blueVal-3*(thresholdBlueMax-blueVal);
      if(thresholdBlueMin<=0)thresholdBlueMin=1;
      if(thresholdBlueMax>=256)thresholdBlueMax=255;
}


/// Getter of trajectory
Trajectory AlgoSubstraction::getTrajectory(){return myTrajectory ; }


