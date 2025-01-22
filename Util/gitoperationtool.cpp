#include "gitoperationtool.h"
#include "QDebug"
#include "QDir"

static const QString GitCmd = "git";

bool GitRepoObject::CheckCommandGit()
{
    QProcess Process;

    QStringList Params;
    Params << "--version";
    Process.start(GitCmd, Params);
    Process.waitForFinished();

    QString Output = Process.readAllStandardOutput();
    QString errorOutput = Process.readAllStandardError();

    qDebug() << Output;
    if(Output.contains("git version"))
    {
        return true;
    }
    else {
        return false;
    }
}

QString GitRepoObject::GetRepoNameByPath(const QString &InGitPath)
{
    // https://github.com/NiceTry12138/UnrealProjectTool.git
    // git@github.com:NiceTry12138/UnrealProjectTool.git
    QString Result;
    auto SplitStrs = InGitPath.split("/");
    if(SplitStrs.size() <= 0)
    {
        return Result;
    }

    Result = SplitStrs.last();
    if(Result.endsWith(".git"))
    {
        Result.chop(4);
    }

    return Result;
}

QStringList GitRepoObject::GetBranchList(const QString &RepoDirPath)
{
    return QStringList();
}

GitRepoObject::GitRepoObject(QObject* Parent) : QObject(Parent)
{
    ProcessPtr = new QProcess();

    // connect(ProcessPtr.get(), &QProcess::readyReadStandardOutput, this, &GitRepoObject::OnProcessFinished_2);
    connect(ProcessPtr, &QProcess::stateChanged, this, &GitRepoObject::OnProcessFinished_2);
}

GitRepoObject::~GitRepoObject()
{
    bIsProcessFinished = true;
    if(ProcessPtr)
    {
        ProcessPtr->deleteLater();
    }
    ProcessPtr = nullptr;
}

void GitRepoObject::SetRepoID(int InID)
{
    RepoID = InID;
}

void GitRepoObject::Stop()
{
    // 添加强制停止逻辑
    bIsProcessFinished = true;
    ProcessPtr->terminate();
    ProcessPtr->deleteLater();
}

bool GitRepoObject::IsRunning()
{
    return !bIsProcessFinished;
}

bool GitRepoObject::CloneGitRepo(const QString &RepoPathUrl, const QString& ClonePath)
{
    if(!bIsProcessFinished)
    {
        return false;
    }

    URL = RepoPathUrl;
    bIsProcessFinished = false;

    CurrentState = EGitToolState::E_CLONE;

    QStringList Params;
    Params << "clone" << RepoPathUrl << ClonePath;

    ProcessPtr->start(GitCmd, Params);

    return true;
}

bool GitRepoObject::RevertGitRepo(const QString &RepoDirPath)
{
    if(!bIsProcessFinished)
    {
        return false;
    }

    bIsProcessFinished = false;

    CurrentState = EGitToolState::E_Reset;

    QStringList Params;
    ProcessPtr->setWorkingDirectory(RepoDirPath);
    Params << "reset" << "--hard";

    ProcessPtr->start(GitCmd, Params);

    return true;
}

void GitRepoObject::OnProcessFinished()
{
    bIsProcessFinished = true;
    QString output = ProcessPtr->readAllStandardOutput();

    Stop();
    bool bExecuteSuccess = false;
    switch (CurrentState)
    {
    case EGitToolState::E_CLONE:
        bExecuteSuccess = CloneFinishedWithOutput(output);
        break;
    case EGitToolState::E_Reset:
        bExecuteSuccess = ResetFinishedWithOutput(output);
        break;
    default:
        break;
    }
    OnFinished(RepoID, bExecuteSuccess);
}

void GitRepoObject::OnProcessFinished_2(QProcess::ProcessState state)
{
    QString StateStr;
    switch (state) {
    case QProcess::ProcessState::NotRunning:
        StateStr = "NotRunning";
        break;
    case QProcess::ProcessState::Running:
        StateStr = "Running";
        break;
    case QProcess::ProcessState::Starting:
        StateStr = "Starting";
        break;
    default:
        break;
    }

    qDebug() << URL << " State => " << StateStr << " bIsProcessFinished => " << bIsProcessFinished << " ProcessPtr => " << (ProcessPtr == nullptr);
    if(state != QProcess::ProcessState::NotRunning || bIsProcessFinished || ProcessPtr == nullptr)
    {
        return;
    }

    qDebug() << "Finished Clone => " << URL;

    bIsProcessFinished = true;
    QString output = ProcessPtr->readAllStandardOutput();
    bool bExecuteSuccess = false;

    ProcessPtr->terminate();
    OnFinished(RepoID, bExecuteSuccess);
}

bool GitRepoObject::CloneFinishedWithOutput(const QString &Output)
{
    qDebug() << Output;
    return true;
}

bool GitRepoObject::ResetFinishedWithOutput(const QString &Output)
{
    qDebug() << Output;
    return true;
}

GitRepoManager *GitRepoManager::Get()
{
    static GitRepoManager Manager;
    return &Manager;
}

void GitRepoManager::CloneRepo(const QString &InRepoUrl, const QString &InLocalPath)
{
    QDir LocalPath(InLocalPath);

    ++RepoID;
    GitRepoObjectPtr GitTool = std::make_shared<GitRepoObject>();

    GitTool->SetRepoID(RepoID);
    connect(GitTool.get(), &GitRepoObject::OnFinished, this, &GitRepoManager::OnRepoToolFinished);

    bool cdSuccess = LocalPath.exists(".git");
    qDebug() << LocalPath.path();
    if(LocalPath.exists(".git"))
    {
        // 本地存在 则直接 Revert
        GitTool->RevertGitRepo(InLocalPath);
    }
    else {
        // 本地不存在 则 Clone
        GitTool->CloneGitRepo(InRepoUrl, InLocalPath);
    }

    Repos[RepoID] = GitTool;
}

int GitRepoManager::GetRunningRepoCount()
{
    int Count = 0;
    for(const auto& ProcessItem : Repos)
    {
        if(ProcessItem.second->IsRunning())
        {
            ++Count;
        }
    }
    return Count;
}

void GitRepoManager::OnRepoToolFinished(int InRepoID, bool Success)
{
    auto Repo = Repos.find(InRepoID);
    if(Repo == Repos.end())
    {
        return;
    }

    Repos.erase(InRepoID);
}
