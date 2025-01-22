#ifndef UNREALHELPER_H
#define UNREALHELPER_H

#include "map"
#include "QString"

class UnrealHelper
{
public:
    static UnrealHelper* Get();

    UnrealHelper& operator = (const UnrealHelper& Other) = delete;
    UnrealHelper(const UnrealHelper& Other) = delete;
    UnrealHelper(const UnrealHelper&& Other) = delete;

    std::map<QString, QString>& GetInstalledUnrealVersion();

    bool GenerateProject(const QString& EngineAppName, const QString& UProjectFilePath);

protected:
    UnrealHelper();

protected:
    void Init();
    void InitData();

    void InitUnrealVersionList();

private:
    std::map<QString, QString> InstalledUnreal;

};

#endif // UNREALHELPER_H
