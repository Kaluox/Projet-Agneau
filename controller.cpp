#include "controller.h"
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QHash>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStringList>
#include <stdlib.h>

/**
 * @brief Controller::Controller
 * @param model le modèle de l'application associé au controleur
 * @param view la vue de l'application associée au controleur
 */
Controller::Controller(Model *model, View *view){
    // Création des éléments principaux de la fenêtre et initialisation des paramètres
    this->model = model ;
    this->view = view ;

    this->hasSound = false;

    if(!QDir("Videos").exists())
        QDir().mkdir("Videos") ;
    currentPath = QDir::currentPath() ;
    std::cout << "current " << currentPath.toStdString() << std::endl;
    QDir::setCurrent(QDir::currentPath() + "/Videos") ;

    qWidgetMainWindow = view->getMainWindow() ;
    qWidgetShiftMaxWindow = view->getShiftMaxWindow() ;
    qWidgetObjectHistogramWindow = view->getObjectHistogramWindow() ;
    qWidgetVideoThresholdWindow = view->getThresholdVideoWindow() ;
    qWidgetColorSelection = view->getQColorDialog() ;

    view->getQColorDialog()->setWindowTitle("Sélection de la couleur de l`objet à tracker");

    qWidgetExtractVideoWindow = view->getExtractVideoWindow() ;


    // Association des slots à appeler lorsque les actions sont cliquées
    ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setMouseTracking(true);
    // QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionShiftMax(), SIGNAL(triggered()), this, SLOT(displayShiftMaxDialog()) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionOpenVideo(), SIGNAL(triggered()), this, SLOT(openVideoFile()) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionObjectHistogram(), SIGNAL(triggered(bool)), this, SLOT(displayObjectHistogram())) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmColors(), SIGNAL(triggered(bool)), this, SLOT(algorithmColorsChecked(bool)) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmSubstraction(), SIGNAL(triggered(bool)), this, SLOT(algorithmSubstractionChecked(bool)) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionOpenThresholdVideo(), SIGNAL(triggered(bool)), this, SLOT(displayVideoThreshold()) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionOpenColorSelection(), SIGNAL(triggered(bool)), this, SLOT(displayColorSelection()) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionExtractVideo(), SIGNAL(triggered(bool)), this, SLOT(displayExtractVideo()) ) ;

    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQActionOpenColorSelections(), SIGNAL(triggered(bool)), this, SLOT(displayColorSelection()) ) ;

    // Association des slots à appeler lorsque des évènements de l'utilisateur sont détectés sur la vidéo
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay(), SIGNAL(mouseMoved(int,int)), this, SLOT(mouseMoving(int,int))) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay(), SIGNAL(mousePressed(int,int)), this, SLOT(mousePressed(int,int))) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay(), SIGNAL(mouseReleased(int,int)), this, SLOT(mouseReleased(int,int))) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay(), SIGNAL(mousePressedAndMoving(int,int)), this, SLOT(mouseMovingAndPressed(int,int))) ;

    // Association des slots à appeler lorsque le curseur du temps de la vidéo est déplacé
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQSlider(), SIGNAL(sliderMoved(int)), this, SLOT(sliderVideoMoved(int))) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQSlider(), SIGNAL(sliderPressed()), this, SLOT(sliderVideoPressed()) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQSlider(), SIGNAL(sliderReleased()), this, SLOT(sliderVideoReleased()))  ;

    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonFastBackward(), SIGNAL(clicked(bool)), this, SLOT(fastBackward()) );
    // Association des slots à appeler lorsque les différents boutons d'interaction de la vidéo sont cliqués
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonFastForward(), SIGNAL(clicked(bool)), this, SLOT(fastForward()) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonBackward(), SIGNAL(clicked(bool)), this, SLOT(backward()));
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonForward(), SIGNAL(clicked(bool)), this, SLOT(forward()) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonStart(), SIGNAL(clicked(bool)), this, SLOT(beginningVideo()));
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonEnd(), SIGNAL(clicked(bool)), this, SLOT(endVideo()) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause(), SIGNAL(clicked(bool)), this, SLOT(playStopVideo())  ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonSaveCoordinate(), SIGNAL(clicked(bool)), this, SLOT(saveTrajectory()) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonClear(), SIGNAL(clicked(bool)), this, SLOT(clearList()) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonStartVideoToExtract(), SIGNAL(clicked(bool)), this, SLOT(setStartVideoExtraction())) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQPushButtonEndVideoToExtract(), SIGNAL(clicked(bool)), this, SLOT(setEndVideoExtraction())) ;
    // Association des slots à appeler lorsque les cases "visualisation" et "traitement" sont sélectionnées, et lorsque la case d'activation du suivi de la trajection est cochée/décochée
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQCheckBox(), SIGNAL(stateChanged(int)), this, SLOT(trajectoryChecked(int)) ) ;
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQRadioButtonTreatment(), SIGNAL(clicked(bool)), this, SLOT(treatmentChecked()) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQRadioButtonVisualize(), SIGNAL(clicked(bool)), this, SLOT(visualizeChecked()) );

    // Association des slots à appeler lorsque l'utilisateur zoom ou dézoom sur l'affichage du signal sonore
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(onSoundXRangeChanged(QCPRange)) );
    QObject::connect( ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(onSoundYRangeChanged(QCPRange)) );

    // Association du slot à appeler lorsque le bouton "confirmer" de la fenêtre de choix du déplacement maximum est cliqué
    QObject::connect( ((ShiftMaxWindow*)qWidgetShiftMaxWindow)->getQPushButton(), SIGNAL(clicked(bool)), this, SLOT(shiftMaxEdited()) ) ;


    // Association du slot à appeler lorsque le bouton "confirmer" de la fenêtre d'extraction de la vidéo est cliqué
    QObject::connect( ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->getQPushButton(), SIGNAL(clicked(bool)), this ,SLOT(extractVideo()) ) ;

    // Association des slots à appeler lorsque les signaux sont envoyés par les processus de lecture et de traitement de la vidéo sont envoyés
    QObject::connect( model->getVideo(), SIGNAL(processedImage(QImage, int, QString)), this, SLOT(updateVideo(QImage, int, QString)));
    QObject::connect( model->getVideo(), SIGNAL(processedThresholdImage(QImage)), this, SLOT(updateThresholdVideo(QImage)));

    // Connexion pour mettre à jour la couleur traquée directement au moment de la sélection dans la fenêtre de dialogue
    QObject::connect((QColorDialog*)(qWidgetColorSelection), SIGNAL(colorSelected(QColor)), this, SLOT(applyColor()));

    // Appel forcé au slot pour activer l'algorithme de couleur par défaut
    ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmColors()->trigger();
    ((MainWindow*)qWidgetMainWindow)->getQRadioButtonTreatment()->click();
    model->getSound()->setSoundFileToCompare("");

    // Raccourcis
    QShortcut* shortcutPlay = new QShortcut(QKeySequence(Qt::Key_Space),((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause());
    QObject::connect(shortcutPlay, SIGNAL(activated()),this, SLOT(playStopVideo()));
    QShortcut* shortcutForward = new QShortcut(QKeySequence(Qt::Key_Right),((MainWindow*)qWidgetMainWindow)->getQPushButtonForward());
    QObject::connect(shortcutForward, SIGNAL(activated()),this, SLOT(forward()));
    QShortcut* shortcutBackward = new QShortcut(QKeySequence(Qt::Key_Left),((MainWindow*)qWidgetMainWindow)->getQPushButtonBackward());
    QObject::connect(shortcutBackward, SIGNAL(activated()),this, SLOT(backward()));
    QShortcut* shortcutFForward = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right),((MainWindow*)qWidgetMainWindow)->getQPushButtonFastForward());
    QObject::connect(shortcutFForward, SIGNAL(activated()),this, SLOT(fastForward()));
    QShortcut* shortcutFBackward = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left),((MainWindow*)qWidgetMainWindow)->getQPushButtonFastBackward());
    QObject::connect(shortcutFBackward, SIGNAL(activated()),this, SLOT(fastBackward()));
    QShortcut* shortcutEnd = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Right),((MainWindow*)qWidgetMainWindow)->getQPushButtonEnd());
    QObject::connect(shortcutEnd, SIGNAL(activated()),this, SLOT(endVideo()));
    QShortcut* shortcutStart = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Left),((MainWindow*)qWidgetMainWindow)->getQPushButtonStart());
    QObject::connect(shortcutStart, SIGNAL(activated()),this, SLOT(beginningVideo()));
    QShortcut* shortcutStartExtract = new QShortcut(QKeySequence(Qt::Key_D),((MainWindow*)qWidgetMainWindow)->getQPushButtonStartVideoToExtract());
    QObject::connect(shortcutStartExtract, SIGNAL(activated()),this, SLOT(setStartVideoExtraction()));
    QShortcut* shortcutEndExtract = new QShortcut(QKeySequence(Qt::Key_F),((MainWindow*)qWidgetMainWindow)->getQPushButtonEndVideoToExtract());
    QObject::connect(shortcutEndExtract, SIGNAL(activated()),this, SLOT(setEndVideoExtraction()));
    QShortcut* shortcutSave = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S),((MainWindow*)qWidgetMainWindow)->getQPushButtonSaveCoordinate());
    QObject::connect(shortcutSave, SIGNAL(activated()),this, SLOT(saveTrajectory()));
    QShortcut* shortcutClear = new QShortcut(QKeySequence(Qt::Key_Delete),((MainWindow*)qWidgetMainWindow)->getQPushButtonClear());
    QObject::connect(shortcutClear, SIGNAL(activated()),this, SLOT(clearList()));
    ((MainWindow*)qWidgetMainWindow)->getQActionOpenVideo()->setShortcut(QKeySequence(Qt::Key_F1));
    ((MainWindow*)qWidgetMainWindow)->getQActionOpenColorSelections()->setShortcut(QKeySequence(Qt::Key_F2));
    ((MainWindow*)qWidgetMainWindow)->getQActionOpenThresholdVideo()->setShortcut(QKeySequence(Qt::Key_F3));
    ((MainWindow*)qWidgetMainWindow)->getQActionExtractVideo()->setShortcut(QKeySequence(Qt::Key_F4));

    /**
    QShortcut* shortcutVisualize = new QShortcut(QKeySequence(Qt::Key_V),((MainWindow*)qWidgetMainWindow)->getQRadioButtonVisualize());
    QObject::connect(shortcutVisualize, SIGNAL(activated()),this, SLOT(visualizeChecked()));
    QShortcut* shortcutTreatment = new QShortcut(QKeySequence(Qt::Key_T),((MainWindow*)qWidgetMainWindow)->getQRadioButtonTreatment());
    QObject::connect(shortcutTreatment, SIGNAL(activated()),this, SLOT(treatmentChecked()));
    **/
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonClear()->setEnabled(false);
}

/**
 * @brief Controller::displayShiftMaxDialog
 * Méthode pour ouvrir la fenêtre de déplacement maximum.
 */
void Controller::displayShiftMaxDialog(){ ((ShiftMaxWindow*)qWidgetShiftMaxWindow)->show();}

/**
 * @brief Controller::displayVideoThreshold
 * Méthode pour faire apparaitre la fenêtre de la vidéo seuillée.
 */
void Controller::displayVideoThreshold(){ ((ThresholdVideoWindow*)qWidgetVideoThresholdWindow)->show();}

/**
 * @brief Controller::displayColorSelection
 * Méthode pour faire apparaitre la fenêtre de sélection de couleurs.
 */
void Controller::displayColorSelection(){
    ((QColorDialog*)qWidgetColorSelection)->show();
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");

}

/**
 * @brief Controller::displayExtractVideo
 * Méthode pour faire apparaitre la fenêtre d'extraction vidéo.
 */
void Controller::displayExtractVideo(){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");
    std::stringstream s ;
    if((((MainWindow*)qWidgetMainWindow)->getQLabelStartVideoToExtract()->text().compare("--:--") == 0) ||
            (((MainWindow*)qWidgetMainWindow)->getQLabelEndVideoToExtract()->text().compare("--:--") ==0)){
        s << "Veuillez choisir une durée à extraire !" ;
        ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->getQPushButton()->setVisible(false);
    }else if( ((MainWindow*)qWidgetMainWindow)->getQLabelStartVideoToExtract()->text().compare(((MainWindow*)qWidgetMainWindow)->getQLabelEndVideoToExtract()->text()) == 0 ){
        s << "Vous ne pouvez pas choisir la même durée !" ;
        ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->getQPushButton()->setVisible(false);
    }else{
        s << "Confirmez-vous de vouloir extraire la video sur la durée de [" << ((MainWindow*)qWidgetMainWindow)->getQLabelStartVideoToExtract()->text().toStdString() << " " << ((MainWindow*)qWidgetMainWindow)->getQLabelEndVideoToExtract()->text().toStdString() << "] ?" ;
        ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->getQPushButton()->setVisible(true);
    }
    ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->setQLabel(QString::fromStdString(s.str()));
    ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->show();
}

/**
 * @brief Controller::openVideoFile
 * Méthode pour faire apparaitre la fenêtre modal de sélection de fichier d'extensions *.avi *.mpg *.mp4 *.asf *.mkv.
 * Initialise le curseur sous la vidéo.
 */
void Controller::openVideoFile(){
    model->getVideo()->stop();
    model->getSound()->stop();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");
    filename = QFileDialog::getOpenFileName(((MainWindow*)qWidgetMainWindow), "Ouvrir une vidéo", "../Videos", "Fichiers vidéos (*.avi *.mpg *.mp4 *.asf *.mkv)") ;
    QMessageBox msgBox;

    folderName="";

    std::cout << filename.toStdString() << std::endl;
    if(!filename.isEmpty()){
        if(!model->getVideo()->loadVideo(filename.toStdString())){
            msgBox.setText("Le fichier sélectionné n'a pas pu être ouvert!");
            msgBox.exec() ;
        }

        QStringList list = filename.split("/") ;
        folderName = "";

        for(int i = 0 ; i < list.size()-1;i++)
        {
            folderName += list.at(i).toStdString().c_str();
            folderName += "/";
        }
        filename = list.at(list.size()-1).toStdString().c_str() ;
        list = filename.split(".") ;
        filename = list.at(0).toStdString().c_str() ;

        // update sound signal
        if(!QFile(folderName + filename+".wav").exists()){
            //msgBox.setText("in extract wav file!");
            //msgBox.exec() ;
            model->getSound()->extractSound(folderName + filename);
        }
        try {
            ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->clearPlottables();
            ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->clearItems();
            ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->replot();

            if(QFile(folderName + filename+".wav").exists())
            {   
                model->getSound()->initPlotVectors(model->getVideo()->getNumberOfFrames(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal(), folderName + filename);
                model->getSound()->initCursor(*((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
                this->hasSound = true;
            }else{
                this->hasSound = false;
            }
        }
        catch(std::exception)
        {
            msgBox.setText("Cannot init plot vectors");
            msgBox.exec();
        }

        model->getSound()->setMedia(folderName + filename+".wav");

        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getNumberOfFrames()) ;
        std::stringstream s ;
        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();
        ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Pause");
        ((MainWindow*)qWidgetMainWindow)->enableControlButtons(true);
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setEnabled(true);
        ((MainWindow*)qWidgetMainWindow)->setWindowTitle(filename);
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getFirstFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        ((MainWindow*)qWidgetMainWindow)->setQLabelFinishTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString("00:00"));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setMinimum(1);
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setMaximum(model->getVideo()->getNumberOfFrames());
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(1);
        ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setChecked(false);
        ((MainWindow*)qWidgetMainWindow)->getQActionExtractVideo()->setEnabled(true);
        ((MainWindow*)qWidgetMainWindow)->enableVideoButtons(false);

        model->getVideo()->start();
        model->getVideo()->setTrajectoryChecked(false);
        ((MainWindow*)qWidgetMainWindow)->getQListWidget()->clear();


    }
}

/**
 * @brief Controller::playStopVideo
 * Méthode pour reprendre la lecture ou faire pause dans la vidéo.
 */
void Controller::playStopVideo(){
    if(model->getVideo()->isStopped()){
        model->getVideo()->play();
        model->getSound()->play();
        ((MainWindow*)qWidgetMainWindow)->enableVideoButtons(false);
        ((MainWindow*)qWidgetMainWindow)->setPlayPauseButton("Pause");



    }else{
        model->getSound()->pause();
        model->getVideo()->stop();
        model->getSound()->setPosition(model->getVideo()->getMsecPos());
        ((MainWindow*)qWidgetMainWindow)->enableVideoButtons(true);
        ((MainWindow*)qWidgetMainWindow)->setPlayPauseButton("Play");
    }

}

/**
 * @brief Controller::displayObjectHistogram
 * Méthode pour afficher l'histogramme de couleur de l'objet selectionné.
 */
void Controller::displayObjectHistogram(){
    model->getVideo()->computeHistogramChosenObject();
    ((ObjectHistogramWindow*)qWidgetObjectHistogramWindow)->setQLabelHistogram(model->getVideo()->getHistRed(),
                                                                               model->getVideo()->getHistGreen(),
                                                                               model->getVideo()->getHistBlue(),
                                                                               model->getVideo()->getRedMin(),
                                                                               model->getVideo()->getRedMax(),
                                                                               model->getVideo()->getGreenMin(),
                                                                               model->getVideo()->getGreenMax(),
                                                                               model->getVideo()->getBlueMin(),
                                                                               model->getVideo()->getBlueMax());
    ((ObjectHistogramWindow*)qWidgetObjectHistogramWindow)->show();
}

/**
 * @brief Controller::updateVideo
 * @param img QImage L'image à afficher
 * @param currentFrame int Numéro de la frame courante
 * @param info QString Coordonées de l'objet tracké
 * Méthode pour lire chaque frame de la vidéo. Met à jour la liste des coordonées de l'objet tracké.
 */
void Controller::updateVideo(QImage img, int currentFrame, QString info){
    //qDebug() << "update : " << QThread::currentThreadId();
    //qDebug() << model->getVideo()->getCurrentFrameID();
    if(currentFrame == 1) { ((MainWindow*)qWidgetMainWindow)->setPlayPauseButton("Play"); }
    ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(img);
    ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(currentFrame);
    int nbSeconds = model->getVideo()->getSecFromNbFrame(currentFrame) ;
    std::stringstream s ;
    QString str;
    str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
    s << str.toStdString();
    //s << nbSeconds/60 << ":" << nbSeconds%60 ;
    ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));

    if (this->hasSound)
    {
        model->getSound()->updateCursor(currentFrame, *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
    }

    if(info != ""){
        QListWidgetItem* qListWidgetItem = new QListWidgetItem(info, ((MainWindow*)qWidgetMainWindow)->getQListWidget());
        ((MainWindow*)qWidgetMainWindow)->getQListWidget()->addItem(qListWidgetItem);
        ((MainWindow*)qWidgetMainWindow)->getQListWidget()->scrollToBottom();
        ((MainWindow*)qWidgetMainWindow)->getQPushButtonClear()->setEnabled(true);
        ((MainWindow*)qWidgetMainWindow)->getQPushButtonSaveCoordinate()->setEnabled(true);
    }
}

/**
 * @brief Controller::updateThresholdVideo
 * @param img
 * Met à jour la vidéo seuillée
 */
void Controller::updateThresholdVideo(QImage img){
    ((ThresholdVideoWindow*)qWidgetVideoThresholdWindow)->displayImage(img);
}

/**
 * @brief Controller::saveTrajectory
 * Sauvegarde la trajectoire trouvée dans un fichier .csv dans le dossier de la vidéo source.
 */
void Controller::saveTrajectory(){
    std::stringstream s ;
    s << folderName.left(folderName.length() - 4).toStdString() << ".csv" ;

    QString filename = QFileDialog::getSaveFileName(((MainWindow*)qWidgetMainWindow), "Sauvegarder la trajectoire", QString::fromStdString("output.csv"), "Fichiers csv (*.csv);;All files (*.*)") ;
    QFile data(filename) ;

    if(data.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream output(&data) ;
        int count = ((MainWindow*)qWidgetMainWindow)->getQListWidget()->count() ;
        for(int i = 0 ; i < count ; i++){
            output << "" ;
            output << ((MainWindow*)qWidgetMainWindow)->getQListWidget()->item(i)->text() << "\n";
        }
    }

    data.close();
}


void Controller::loadTrajectory(){
    QString filename = QFileDialog::getOpenFileName(((MainWindow*)qWidgetMainWindow),
          tr("Ouvrir un fichier"), "",
          tr("Fichiers csv (*.csv);;All files (*.*)"));
    QFile data(filename);
    if(data.open(QFile::ReadOnly)) {
        QTextStream input(&data);
        while(!input.atEnd()) {
            QString line = input.readLine();
            ((MainWindow*)qWidgetMainWindow)->getQListWidget()->addItem(line);
        }
    }
    data.close();
}

/**
 * @brief Controller::clearList
 * Nettoie la liste des coordonées
 */
void Controller::clearList(){
    ((MainWindow*)qWidgetMainWindow)->getQListWidget()->clear();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonClear()->setEnabled(false);
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonSaveCoordinate()->setEnabled(false);
}

/**
 * @brief Controller::shiftMaxEdited
 *
 */
void Controller::shiftMaxEdited(){
    model->getVideo()->setShiftMax(((ShiftMaxWindow*)qWidgetShiftMaxWindow)->getQLineEdit()->text().toInt());
    ((ShiftMaxWindow*)qWidgetShiftMaxWindow)->close() ;
}

/**
 * @brief Controller::sliderVideoPressed
 * Arrete la vidéo et le son quand le slider sous la vidéo est cliqué.
 */
void Controller::sliderVideoPressed(){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");
}

/**
 * @brief Controller::sliderVideoMoved
 * @param position int La position du slider
 * Met à jour le label indiquant le temps courant en fonction de la postion du slider.
 */
void Controller::sliderVideoMoved(int position){

    positionSlide = position;
    std::stringstream s ;
    int nbSeconds = model->getVideo()->getSecFromNbFrame(position) ;

    QString str;
    str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
    s << str.toStdString();

    ((MainWindow*)qWidgetMainWindow)->setPlayPauseButton("Play");
    ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));

}

/**
 * @brief Controller::sliderVideoReleased
 * Met à jour la frame courante de la vidéo et et label du temps courant en fonction de la position du slider.
 */
void Controller::sliderVideoReleased()
{
   model->getVideo()->setCurrentFrame(positionSlide);
   std::stringstream s ;
   int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;


   QString str;
   str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
   s << str.toStdString();

   ((MainWindow*)qWidgetMainWindow)->setPlayPauseButton("Play");
   ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));

   if (this->hasSound)
   {
   model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
   model->getSound()->setPosition(model->getVideo()->getMsecPos());
   }
   ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getCurrentFrame());
   ((MainWindow*)qWidgetMainWindow)->enableVideoButtons(true);
   //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getCurrentFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
}

/**
 * @brief Controller::beginningVideo
 * Initialise le lecteur vidéo à la première frame, place le slider au début et réinitialise le label du temps.
 */
void Controller::beginningVideo(){
    model->getVideo()->stop();
    model->getSound()->stop();
    ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getFirstFrame());
    //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage((model->getVideo()->getFirstFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");
    if (this->hasSound)
    {
    model->getSound()->updateCursor(1, *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
    }
    ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString("00:00"));
    ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(1);


}

/**
 * @brief Controller::endVideo
 * Affiche la dernière frame de la vidéo, place le slider à la dernière position, et met à jour le label du temps.
 */
void Controller::endVideo(){
    int nbFrames = model->getVideo()->getNumberOfFrames();
    int nbSeconds = model->getVideo()->getSecFromNbFrame(nbFrames);

    model->getVideo()->stop();
    model->getSound()->stop();
    ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getLastFrame());

    ((MainWindow*)qWidgetMainWindow)->setPlayPauseButton("Play");
    //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage((model->getVideo()->getLastFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation)));
    std::stringstream s ;
    QString str;
    str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
    s << str.toStdString();
    if (this->hasSound)
    {
    model->getSound()->updateCursor(nbFrames-1, *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
    }
    ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
    ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(nbFrames);
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");
}

/**
 * @brief Controller::backward
 * Fais le recule d'une frame dans la vidéo. Met à jour le slider et le label du temps.
 */
void Controller::backward(){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");

    if(model->getVideo()->getCurrentFrameID() > 0){
        ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getPreviousFrame());
        //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getPreviousFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        std::stringstream s ;
        //qDebug() << model->getVideo()->getCurrentFrameID();
        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
        model->getVideo()->setCurrentFrame(model->getVideo()->getCurrentFrameID());

        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();

        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(model->getVideo()->getCurrentFrameID());

        if (this->hasSound)
        {
         model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
         model->getSound()->setPosition(model->getVideo()->getMsecPos());
        }

    }

}

/**
 * @brief Controller::forward
 * Fais avancer la vidéo d'une frame. Met à jour le slider et le label du temps.
 */
void Controller::forward(){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");

    if(model->getVideo()->getCurrentFrameID() < model->getVideo()->getNumberOfFrames()){
        ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getNextFrame());
        //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getNextFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        //qDebug() << model->getVideo()->getCurrentFrameID();

        std::stringstream s ;
        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
        model->getVideo()->setCurrentFrame(model->getVideo()->getCurrentFrameID());


        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();


        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(model->getVideo()->getCurrentFrameID());

        if (this->hasSound)
        {
        model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
        model->getSound()->setPosition(model->getVideo()->getMsecPos());
        }

    }
}

/**
 * @brief Controller::fastBackward
 * Fais le recule de 10 frames dans la vidéo. Met à jour le slider et le label du temps.
 */
void Controller::fastBackward(){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");

    if(model->getVideo()->getCurrentFrameID() > 10){
        ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getPreviousFrameExtended(11));

        //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getPreviousFrameExtended(11)).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        //qDebug() << model->getVideo()->getCurrentFrameID();

        std::stringstream s ;
        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
        model->getVideo()->setCurrentFrame(model->getVideo()->getCurrentFrameID());


        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();


        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(model->getVideo()->getCurrentFrameID());

        if (this->hasSound)
        {
        model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
        model->getSound()->setPosition(model->getVideo()->getMsecPos());
        }


    }
    else {
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getFirstFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        std::stringstream s ;
        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
        model->getVideo()->setCurrentFrame(model->getVideo()->getCurrentFrameID());


        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();


        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(model->getVideo()->getCurrentFrameID());
        model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
        model->getSound()->setPosition(model->getVideo()->getMsecPos());
    }
}

/**
 * @brief Controller::fastForward
 * Fais avancer la vidéo de 10 frame. Met à jour le slider et le label du temps.
 */
void Controller::fastForward(){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");


    if(model->getVideo()->getCurrentFrameID() < model->getVideo()->getNumberOfFrames() - 10){
        ((MainWindow*)qWidgetMainWindow)->displayImageQLabelVideo(model->getVideo()->getNextFrameExtended(9));
        //((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getNextFrameExtended(9)).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));

        std::stringstream s ;
        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
        model->getVideo()->setCurrentFrame(model->getVideo()->getCurrentFrameID());


        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();


        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(model->getVideo()->getCurrentFrameID());

        if (this->hasSound)
        {
        model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
        model->getSound()->setPosition(model->getVideo()->getMsecPos());
        }

    }
    else {
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getLastFrame()).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        std::stringstream s ;
        int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
        model->getVideo()->setCurrentFrame(model->getVideo()->getCurrentFrameID());

        QString str;
        str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
        s << str.toStdString();


        ((MainWindow*)qWidgetMainWindow)->setQLabelCurrentTime(QString::fromStdString(s.str()));
        ((MainWindow*)qWidgetMainWindow)->getQSlider()->setValue(model->getVideo()->getCurrentFrameID());

        if (this->hasSound)
        {
        model->getSound()->updateCursor(model->getVideo()->getCurrentFrameID(), *((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal());
        model->getSound()->setPosition(model->getVideo()->getMsecPos());
        }

    }
}

/**
 * @brief Controller::algorithmColorsChecked
 * @param toogled
 * Choisis l'algorithme de couleurs
 * Active / Desactive les boutons disponibles liés à l'algorithme de couleurs (MouseTracking, Ouvrir vidéo seuillée, ouvrir sections de couleurs)
 */
void Controller::algorithmColorsChecked(bool toogled){
    if(toogled)
    {
        ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmSubstraction()->setChecked(!toogled);
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setMouseTracking(!toogled);
        ((MainWindow*)qWidgetMainWindow)->getQActionOpenThresholdVideo()->setDisabled(!toogled);
        ((MainWindow*)qWidgetMainWindow)->getQActionOpenColorSelection()->setDisabled(!toogled);
        model->getVideo()->stop();
        model->getSound()->pause();
        ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");

        model->getVideo()->setAlgoIDtoRun(1);
        if(((MainWindow*)qWidgetMainWindow)->getQRadioButtonTreatment()->isChecked())
            ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(toogled);
    }
    else
    {
        ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmSubstraction()->setChecked(!toogled);
        algorithmSubstractionChecked(!toogled);
    }

}

/**
 * @brief Controller::algorithmSubstractionChecked
 * @param toogled
 * Choisis l'algorithme de soustraction
 * Active / Desactive les boutons disponibles liés à l'algorithme de soustraction (MouseTracking, Ouvrir vidéo seuillée, ouvrir sections de couleurs)
 */
void Controller::algorithmSubstractionChecked(bool toogled){
    if(toogled)
    {
        ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmColors()->setChecked(!toogled);
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setMouseTracking(toogled);
        ((MainWindow*)qWidgetMainWindow)->getQActionOpenThresholdVideo()->setDisabled(toogled);
        ((MainWindow*)qWidgetMainWindow)->getQActionOpenColorSelection()->setDisabled(toogled);
        model->getVideo()->stop();
        model->getSound()->pause();
        ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");

        model->getVideo()->setAlgoIDtoRun(0);
        ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(!toogled);
        ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setChecked(!toogled);

    }
    else
    {
        ((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmColors()->setChecked(!toogled);
        algorithmColorsChecked(!toogled);
    }
}

/**
 * @brief Controller::trajectoryChecked
 * @param checked
 * Coche la case trajectoire
 * Active le bouton pour sauvegarder les coordonées
 */
void Controller::trajectoryChecked(int checked){
    bool c = (checked == 2) ? true : false ;
    model->getVideo()->setTrajectoryChecked(c);
    /**
    if (c == true && ((MainWindow*)qWidgetMainWindow)->getQListWidget()->count() == 0) {
        this->displayColorSelection();
    }**/

}

/**
 * @brief Controller::visualizeChecked
 * Coche la case "Visualiser"
 * Desactive les cases liés au traitement
 */
void Controller::visualizeChecked(){

    model->getVideo()->setVisualize(true);
    model->getVideo()->setTreatment(false);
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonSaveCoordinate()->setEnabled(false);
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonClear()->setEnabled(false);
    ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setChecked(false);
    ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(false);
    model->getVideo()->setTrajectoryChecked(false);
}

/**
 * @brief Controller::treatmentChecked
 * Coche la case "Traitement"
 * Désactive les cases liées à la visualisation
 */
void Controller::treatmentChecked(){
    model->getVideo()->setVisualize(false);
    model->getVideo()->setTreatment(true);
    if(((MainWindow*)qWidgetMainWindow)->getQActionAlgorithmColors()->isChecked())
        ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(true);
    else if(((MainWindow*)qWidgetMainWindow)->getQActionObjectHistogram()->isEnabled())
    {
        model->getVideo()->isFirstFrame(true) ;
        model->getVideo()->toKeepRunning(false);
    }

}

/**
 * @brief Controller::mouseMoving
 * @param x coordonées x du curseur de la souris
 * @param y coordonées y du curseur de la souris
 * Met à jour la barre de status avec les positions du curseur de la souris
 */
void Controller::mouseMoving(int x, int y){ ((MainWindow*)qWidgetMainWindow)->updateSatusBar(QString("Mouse's moving in :(%1,%2)").arg(x).arg(y));}

/**
 * @brief Controller::mousePressed
 * @param x coordonées x du curseur de la souris
 * @param y coordonées y du curseur de la souris
 * Met à jour la barre de status avec l'indication qu'un bouton de la souris est pressé avec ses coordonées
 * Affiche les coordonées sur l'image
 */
void Controller::mousePressed(int x, int y){
    model->getVideo()->stop();
    model->getSound()->pause();
    ((MainWindow*)qWidgetMainWindow)->getQPushButtonPlayPause()->setText("Play");
    int imgHeight = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().height();
    int videoDiplayHeight = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size().height();
    int imgWidth = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().width();
    int videoDiplayWidth = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size().width();

    if(x > (videoDiplayWidth - imgWidth)/2 && x < videoDiplayWidth - (videoDiplayWidth - imgWidth)/2
            && y > (videoDiplayHeight - imgHeight)/2 && y < videoDiplayHeight - (videoDiplayHeight - imgHeight)/2)
    {
        isMousePressed = false ;
        int ydecal = (videoDiplayHeight - imgHeight)/2;
        int xdecal = (videoDiplayWidth - imgWidth)/2;


        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(model->getVideo()->getImageMousePressed(x - xdecal, y - ydecal,((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().height(),((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().width())).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->adjustSize();
        ((MainWindow*)qWidgetMainWindow)->updateSatusBar(QString("Mouse pressed at :(%1,%2)").arg(x).arg(y));
        //((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setChecked(false);
        //((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(false);
    }
    else
    {
        isMousePressed = false ;
        //((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setChecked(false);
        //((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(false);
    }
    ((MainWindow*)qWidgetMainWindow)->getQActionObjectHistogram()->setDisabled(true);

}
/**
 * @brief Controller::mouseMovingAndPressed
 * @param x coordonée x du curseur de la souris
 * @param y coordonée y du curseur de la souris
 * Met à jour la barre de status avec l'indication qu'un bouton de la souris est pressé et en mouvement avec ses coordonées
 * Trace un rectangle en fonction des coordonées.
 */
void Controller::mouseMovingAndPressed(int x, int y){
    int imgHeight = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().height();
    int videoDiplayHeight = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size().height();
    int imgWidth = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().width();
    int videoDiplayWidth = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size().width();

    if(x > (videoDiplayWidth - imgWidth)/2 && x < videoDiplayWidth - (videoDiplayWidth - imgWidth)/2
            && y > (videoDiplayHeight - imgHeight)/2 && y < videoDiplayHeight - (videoDiplayHeight - imgHeight)/2)
    {
        isMousePressed = true ;
        int ydecal = (videoDiplayHeight - imgHeight)/2;
        int xdecal = (videoDiplayWidth - imgWidth)/2;
        QImage img = model->getVideo()->getImageMousePressedAndMoving(x - xdecal, y - ydecal,((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().height(),((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().width());
        if(img != (QImage)NULL)
        {
            ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(img).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
            ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->adjustSize();
            ((MainWindow*)qWidgetMainWindow)->updateSatusBar(QString("Mouse pressed while moving :(%1,%2)").arg(x).arg(y));
            ((MainWindow*)qWidgetMainWindow)->getQActionObjectHistogram()->setDisabled(false);
        }

    }
    else
    {
        isMousePressed = true ;
    }
}

/**
 * @brief Controller::mouseReleased
 * @param x coordonnée x du curseur de la souris
 * @param y coordonnée y du curseur de la souris
 * Met à jour la barre de status avec l'indication qu'un bouton de la souris est relaché avec ses coordonées
 * Affiche les coordonées sur l'image
 */
void Controller::mouseReleased(int x, int y){
    int imgHeight = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().height();
    int videoDiplayHeight = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size().height();
    int imgWidth = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().width();
    int videoDiplayWidth = ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size().width();

    if(isMousePressed)
    {
        int ydecal = (videoDiplayHeight - imgHeight)/2;
        int xdecal = (videoDiplayWidth - imgWidth)/2;
        QImage img = model->getVideo()->getImageMouseReleased(x - xdecal, y - ydecal,((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().height(),((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->pixmap()->size().width());
        if(img != (QImage)NULL)
        {
            ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->setPixmap(QPixmap::fromImage(img).scaled(((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
            ((MainWindow*)qWidgetMainWindow)->getQLabelVideoDisplay()->adjustSize();
            ((MainWindow*)qWidgetMainWindow)->updateSatusBar(QString("Mouse released at :(%1,%2)").arg(x).arg(y));
            ((MainWindow*)qWidgetMainWindow)->getQActionObjectHistogram()->setDisabled(false);
        }
        else
        {
            ((MainWindow*)qWidgetMainWindow)->getQActionObjectHistogram()->setDisabled(true);
        }
        if(((MainWindow*)qWidgetMainWindow)->getQRadioButtonTreatment()->isChecked() && img != (QImage)NULL){
            model->getVideo()->isFirstFrame(true) ;
            model->getVideo()->toKeepRunning(false);
            ((MainWindow*)qWidgetMainWindow)->getQCheckBox()->setEnabled(true);
        }
        isMousePressed = false;
    }
}
/**
 * @brief Controller::applyColor
 * Change la couleur traquée. La nouvelle couleur traquée est celle qui a été choisie dans le fenêtre "sélection couleur"
 */
void Controller::applyColor(){
    QColor color = ((QColorDialog*)qWidgetColorSelection)->selectedColor();

    const int hueRange = 25;
    const int satRange = 50;
    const int valueRange = 50;

    // Division par 2 à la fin parce qu'OpenCV ne support que les valeurs entre 0 et 180° au lieu des 360° classiques
    int minHue = ((color.hue() - hueRange + 360) % 360) / 2;
    int maxHue = ((color.hue() + hueRange) % 360) / 2;

    model->getVideo()->setMinHue(minHue);
    model->getVideo()->setMaxHue(maxHue);

    int minSat = color.saturation() - satRange;
    int maxSat = color.saturation() + satRange;

    if (minSat < 0)
    {
        minSat = 0;
        maxSat = satRange * 2;
    }
    else if (maxSat > 255)
    {
        minSat = 255 - satRange * 2;
        maxSat = 255;
    }

    model->getVideo()->setMinSaturation(minSat);
    model->getVideo()->setMaxSaturation(maxSat);

    int minValue = color.value() - valueRange;
    int maxValue = color.value() + valueRange;

    if (minValue < 0)
    {
        minValue = 0;
        maxValue = valueRange * 2;
    }
    else if (maxValue > 255)
    {
        minValue = 255 - valueRange * 2;
        maxValue = 255;
    }

    model->getVideo()->setMinValue(minValue);
    model->getVideo()->setMaxValue(maxValue);

}

/**
 * @brief Controller::setStartVideoExtraction
 * Met à jour le texte d'indication du temps de début d'extraction lorsque le bouton "Début extraction" est cliqué
 */
void Controller::setStartVideoExtraction(){
    int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
    std::stringstream s ;
    QString str;
    str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
    s << str.toStdString();
    //s << nbSeconds/60 << ":" << nbSeconds%60 ;
    ((MainWindow*)qWidgetMainWindow)->setQLabelStartVideoToExtract(QString::fromStdString(s.str()));
    ((MainWindow*)qWidgetMainWindow)->setQLabelEndVideoToExtract("--:--");
    model->getVideo()->setFrameEndExtraction(0);
    model->getVideo()->setFrameStartExtraction(model->getVideo()->getCurrentFrameID());

}

/**
 * @brief Controller::setEndVideoExtraction
 * Met à jour le texte d'indication du temps de fin d'extraction lorsque le bouton "Fin extraction" est cliqué
 */
void Controller::setEndVideoExtraction(){
    int nbSeconds = model->getVideo()->getSecFromNbFrame(model->getVideo()->getCurrentFrameID()) ;
    std::stringstream s ;
    QString str;
    str.sprintf("%02d:%02d", nbSeconds/60, nbSeconds%60);
    s << str.toStdString();
    //s << nbSeconds/60 << ":" << nbSeconds%60 ;
    ((MainWindow*)qWidgetMainWindow)->setQLabelEndVideoToExtract(QString::fromStdString(s.str()));

    model->getVideo()->setFrameEndExtraction(model->getVideo()->getCurrentFrameID());
}

/**
 * @brief Controller::extractVideo
 * Lance l'extraction de la vidéo entre les bornes choisies et ferme la fenêtre de dialogue lorsque le bouton de confirmation d'extraction de la vidéo est cliqué
 */
void Controller::extractVideo(){

    model->getVideo()->extractVideo(folderName + filename);
    ((ExtractVideoWindow*)qWidgetExtractVideoWindow)->close() ;
}

/**
 * @brief Controller::onSoundXRangeChanged
 * @param newRange le nouvel intervalle de valeurs du son sur l'axe x
 * Met à jour l'affichage du signal sonore lorsque l'utilisateur zoom sur l'axe x
 */
void Controller::onSoundXRangeChanged(const QCPRange &newRange)
{
    QCPRange limitRange(0, model->getVideo()->getNumberOfFrames());
    auto lowerBound = limitRange.lower;
    auto upperBound = limitRange.upper;

    QCPRange fixedRange(newRange);
    if (fixedRange.lower < lowerBound)
    {
        fixedRange.lower = lowerBound;
        fixedRange.upper = lowerBound + newRange.size();
        if (fixedRange.upper > upperBound || qFuzzyCompare(newRange.size(), upperBound-lowerBound))
            fixedRange.upper = upperBound;
        ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->xAxis->setRange(fixedRange);
    } else if (fixedRange.upper > upperBound)
    {
        fixedRange.upper = upperBound;
        fixedRange.lower = upperBound - newRange.size();
        if (fixedRange.lower < lowerBound || qFuzzyCompare(newRange.size(), upperBound-lowerBound))
            fixedRange.lower = lowerBound;
        ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->xAxis->setRange(fixedRange);
    }
    ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->xAxis->setRange(fixedRange);
}

/**
 * @brief Controller::onSoundYRangeChanged
 * @param newRange le nouvel intervalle de valeurs du son sur l'axe y
 * Met à jour l'affichage du signal sonore lorsque l'utilisateur zoom sur l'axe y. (Cette fonction est désactivée pour l'axe y)
 */
void Controller::onSoundYRangeChanged(const QCPRange &newRange)
{
    Q_UNUSED(newRange)

    QCPRange limitRange(-60, 60);
    ((MainWindow*)qWidgetMainWindow)->getQCPsoundSignal()->yAxis->setRange(limitRange);
}

/**
 * @brief Controller::~Controller
 * Destructeur
 */
Controller::~Controller(){

}
