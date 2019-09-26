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
    // Constructors and destructor
    CascadeStyleSheetLoader();

public:

    static CascadeStyleSheetLoader& getInstance();

    // Methods
    void addPropertiesFile(const QString& propertiesFileName);
    void addStyleSheetFile(const QString& stylesheetFileName);
    void reload(QApplication &app) const;
    QString getProperty(const QString& propertyName);

private:
    QList<QPair<QString, QString>> propertiesList;
    QString baseDocument;
};

#endif // CASCADESTYLESHEETLOADER_H
