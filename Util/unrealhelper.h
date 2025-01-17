#ifndef UNREALHELPER_H
#define UNREALHELPER_H

#include "set"
#include "QString"

class UnrealHelper
{
public:
    static UnrealHelper* Get();

    UnrealHelper& operator = (const UnrealHelper& Other) = delete;
    UnrealHelper(const UnrealHelper& Other) = delete;
    UnrealHelper(const UnrealHelper&& Other) = delete;

    std::set<QString>& GetInstalledUnrealVersion();

protected:
    UnrealHelper();

protected:
    void Init();
    void InitData();

    void InitUnrealVersionList();

private:
    std::set<QString> InstalledUnreal;

};

#endif // UNREALHELPER_H
