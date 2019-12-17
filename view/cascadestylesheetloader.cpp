// Inclusions
#include "cascadestylesheetloader.h"

#include <QStringList>
#include <QDebug>

// Code *******************************************************************************************


/// Constructors
CascadeStyleSheetLoader::CascadeStyleSheetLoader()
{
}


 ///Get the instance
CascadeStyleSheetLoader& CascadeStyleSheetLoader::getInstance()
{
    static CascadeStyleSheetLoader instance;
    return instance;
}


///Load a list of properties from a file and add them to the list of the loaded properties for the next reload
void CascadeStyleSheetLoader::addPropertiesFile(const QString& propertiesFileName)
{
    QFile propertiesFile(propertiesFileName);

    if (propertiesFile.open(QFile::Text | QFile::ReadOnly))
    {
        const QString& propertiesContent = propertiesFile.readAll();
        const QStringList properties = propertiesContent.split('\n');

        for (const QString& propertyLine : properties)
        {
            QStringList splittedLine = propertyLine.split(' ');
            const QString key = splittedLine.first();
            splittedLine.removeFirst();
            const QString value = splittedLine.join(' ');

            propertiesList.append(QPair<QString, QString>(key, value));
        }

        propertiesFile.close();
    }
}


///Load a stylesheet and add it to the global stylesheet for the next reload
void CascadeStyleSheetLoader::addStyleSheetFile(const QString& stylesheetFileName)
{
    QFile stylesheetFile(stylesheetFileName);

    if (stylesheetFile.open(QFile::Text | QFile::ReadOnly))
    {
        baseDocument += stylesheetFile.readAll() + "\n";
        stylesheetFile.close();
    }
}

///Apply the style to the target application based on all the previously added stylesheet and properties
void CascadeStyleSheetLoader::reload(QApplication &app) const
{
    QString finalStyle = baseDocument;

    for (const auto& property : propertiesList)
    {
        finalStyle.replace(property.first, property.second);
    }

    app.setStyleSheet(finalStyle);
}


///Returns the property (which is a QString)
QString CascadeStyleSheetLoader::getProperty(const QString &propertyName)
{
    for (int i = 0; i < propertiesList.size(); i++)
    {
        if (propertiesList.at(i).first == propertyName)
        {
            return propertiesList.at(i).second;
        }
    }

    return "";
}
