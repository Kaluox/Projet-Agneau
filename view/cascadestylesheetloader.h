#ifndef CASCADESTYLESHEETLOADER_H
#define CASCADESTYLESHEETLOADER_H

// Inclusions
#include <QApplication>
#include <QString>
#include <QList>
#include <QPair>
#include <QFile>

class CascadeStyleSheetLoader
{
private:

    /**
    * @brief CascadeStyleSheetLoader::CascadeStyleSheetLoader
    */
    CascadeStyleSheetLoader();

public:


    /**
     * @brief CascadeStyleSheetLoader::getInstance
     * @return instance
     */
    static CascadeStyleSheetLoader& getInstance();


    /**
    * @brief CascadeStyleSheetLoader::addPropertiesFile
    * @param propertiesFileName (QString) : The path of the file to load the properties from
    */
    void addPropertiesFile(const QString& propertiesFileName);


    /**
    * @brief CascadeStyleSheetLoader::addStyleSheetFile
    * @param stylesheetFileName (QString) : The path of the file to load the style from
    */
    void addStyleSheetFile(const QString& stylesheetFileName);

    /**
     * @brief CascadeStyleSheetLoader::reload
     * @param app (QApplication) : The target on which to apply the style
     */
    void reload(QApplication &app) const;
    QString getProperty(const QString& propertyName);

private:

    ///List of (QString, QString) properties
    QList<QPair<QString, QString>> propertiesList;

    ///Base Document
    QString baseDocument;
};

#endif // CASCADESTYLESHEETLOADER_H
