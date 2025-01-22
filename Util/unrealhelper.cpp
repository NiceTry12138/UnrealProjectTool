#include "unrealhelper.h"
#include "QStandardPaths"
#include "QDebug"
#include "QDir"
#include "QFile"
#include "QMessageBox"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QProcess"

UnrealHelper *UnrealHelper::Get()
{
    static UnrealHelper Helper;
    return &Helper;
}

std::map<QString, QString>& UnrealHelper::GetInstalledUnrealVersion()
{
    return InstalledUnreal;
}

// UnrealHelper::Get()->GenerateProject("UE_5.3", "E:/Project/Custom53/Custom53.uproject");
bool UnrealHelper::GenerateProject(const QString& EngineAppName, const QString &UProjectFilePath)
{
    QFile ProjectFile(UProjectFilePath);
    auto AppConf = InstalledUnreal.find(EngineAppName);

    if(!ProjectFile.exists() || AppConf == InstalledUnreal.end())
    {
        return false;
    }

    static QString G_UBTPATH = "Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.exe";

    QDir InstallDir(AppConf->second);
    QString ExeUBPPAth = InstallDir.filePath(G_UBTPATH);
    QFile ExeUBPFile(ExeUBPPAth);

    if(!ExeUBPFile.exists())
    {
        return false;
    }

    QProcess GenerateProcess;

    QStringList Params;
    Params << "-projectfiles" << "-project=" + UProjectFilePath << "-game" << "-engine";

    GenerateProcess.start(ExeUBPPAth, Params);
    GenerateProcess.waitForFinished(5000);

    // 读取标准输出
    QString output = GenerateProcess.readAllStandardOutput();
    qDebug() << "Command output:" << output;

    return true;
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
        auto ItemAppName = InstallationItemObj.find("AppName")->toString();
        QString InstallLocationPath = ItemInstallLocation->toString();
        if(ItemAppName.startsWith("UE_"))
        {
            InstalledUnreal[ItemAppName] = InstallLocationPath;
        }
        // qDebug() << "ItemInstallLocationPath -> " << InstallLocationPath;
    }
}
