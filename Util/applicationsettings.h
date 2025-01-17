#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "QSettings"

class ApplicationSettings
{
public:
    ApplicationSettings();

    static ApplicationSettings* Get();

    QStringList GetAddedPluginResources();
    void SetAddedPluginResources(const QStringList& InList);

protected:
    void Init();


private:
    QSettings settings = QSettings("UnrealProjectTool.ini", QSettings::IniFormat);

};

#endif // APPLICATIONSETTINGS_H
