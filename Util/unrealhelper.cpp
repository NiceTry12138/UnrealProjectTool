#include "unrealhelper.h"
#include "QStandardPaths"
#include "QDebug"
#include "QDir"
#include "QFile"
#include "QMessageBox"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"

UnrealHelper *UnrealHelper::Get()
{
    static UnrealHelper Helper;
    return &Helper;
}

std::set<QString>& UnrealHelper::GetInstalledUnrealVersion()
{
    return InstalledUnreal;
}

UnrealHelper::UnrealHelper() {
    Init();
}

void UnrealHelper::Init()
{
    InitData();
}

void UnrealHelper::InitData()
{
    InitUnrealVersionList();
}

// Unreal 将所有安装的引擎保存在 ApplicationSettingDir 在 Windows 中就是 C:/ProgramData/Epic/UnrealEngineLauncher/LauncherInstalled.dat 文件中
// 对应代码在 DesktopPlatformBase.cpp 文件中，内容如下
// FString InstalledListFile = FString(FPlatformProcess::ApplicationSettingsDir()) / TEXT("UnrealEngineLauncher/LauncherInstalled.dat");
void UnrealHelper::InitUnrealVersionList()
{
    // 通过获取自己程序的 AppConigLocation 得到上级路径 进而得到 C:/ProgramData
    QString configDataDirString = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
    QDir dir(configDataDirString);          // "C:/ProgramData/<APPNAME>"
    dir.cdUp();                             // cd .. => "C:/ProgramData/"
    QString LauncherInstalledConf = dir.filePath("Epic/UnrealEngineLauncher/LauncherInstalled.dat");

    QFile ConfFile(LauncherInstalledConf);
    if(!ConfFile.exists())
    {
        QMessageBox::information(nullptr, "没装引擎?", "请先安装 Unreal Engine 再使用本程序");
        return;
    }

    if(!ConfFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(nullptr, "无法打开文件", "无法打开 LauncherInstalled.dat 文件");
        return;
    }

    QByteArray fileData = ConfFile.readAll();
    ConfFile.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::information(nullptr, "JSON 解析错误", "错误原因: " + parseError.errorString());
        return;
    }

    // 确保文档是一个 JSON 对象
    if (!jsonDoc.isObject()) {
        QMessageBox::information(nullptr, "JSON 解析错误", "无法解析成对象...");
        return;
    }

    auto JsonObject = jsonDoc.object();
    auto InstallationList = JsonObject.find("InstallationList");
    InstallationList->isArray();
    auto Arr = InstallationList->toArray();
    for(int ArrIndex = 0; ArrIndex < Arr.size(); ++ArrIndex)
    {
        auto InstallationItem = Arr.at(ArrIndex);
        auto InstallationItemObj = InstallationItem.toObject();
        auto ItemInstallLocation = InstallationItemObj.find("InstallLocation");
        QString InstallLocationPath = ItemInstallLocation->toString();
        // qDebug() << "ItemInstallLocationPath -> " << InstallLocationPath;
        InstalledUnreal.insert(InstallLocationPath);
    }
}
