#include <QApplication>
#include "view.h"
#include "model.h"
#include "controller.h"
#include "cascadestylesheetloader.h"

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
