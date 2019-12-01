#include <QApplication>
#include "View/view.h"
#include "model/model.h"
#include "controller/controller.h"
#include "view/cascadestylesheetloader.h"


// Main of the application, it will create a translator according to the location of the device and then
// create and instance of the view, the model and then the controller.
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    //QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf8") );
    QTranslator qtTranslator;
       qtTranslator.load("qt_" + QLocale::system().name(),
               QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("myapp_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);
    // Chargement du style
    CascadeStyleSheetLoader& loader = CascadeStyleSheetLoader::getInstance();
    loader.addStyleSheetFile(":/style.css");
    loader.addPropertiesFile(":/properties.txt");
    View view;
    Model model;
    Controller controller(&model, &view) ;

    loader.reload(a);

    return a.exec();
}
