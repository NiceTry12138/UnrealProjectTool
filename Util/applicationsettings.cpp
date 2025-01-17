#include "applicationsettings.h"

static const QString AddedGitPath = "AddedGitPath";

ApplicationSettings::ApplicationSettings()
{
    Init();
}

ApplicationSettings *ApplicationSettings::Get()
{
    static ApplicationSettings Setting;
    return &Setting;
}

QStringList ApplicationSettings::GetAddedPluginResources()
{
    return settings.value(AddedGitPath).toStringList();
}

void ApplicationSettings::SetAddedPluginResources(const QStringList &InList)
{
    settings.setValue(AddedGitPath, InList);
}

void ApplicationSettings::Init()
{
}
