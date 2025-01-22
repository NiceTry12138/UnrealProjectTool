#ifndef GITOPERATIONTOOL_H
#define GITOPERATIONTOOL_H

#include "QStringList"
#include "QProcess"
#include "QObject"

#include "map"

enum class EGitToolState : int
{
    E_NONE,         // 未做事情
    E_CLONE,        // 正在 Clone 操作
    E_Reset,        // 正在 Reset 操作
    E_CheckOut,     // 正在 CheckOut 操作
};

class GitRepoObject : public QObject
{
    Q_OBJECT

public:
    static bool CheckCommandGit();
    static QStringList GetBranchList(const QString& RepoDirPath);
    static QString GetRepoNameByPath(const QString &InGitPath);

public:
    GitRepoObject(QObject* Parent = nullptr);
    ~GitRepoObject();

    void SetRepoID(int InID);
    void Stop();
    bool IsRunning();

    bool CloneGitRepo(const QString& RepoPathUrl, const QString& ClonePath);
    bool RevertGitRepo(const QString& RepoDirPath);

Q_SIGNALS:
    void OnFinished(int OutRepoID, bool Success);

protected:
    void OnProcessFinished();
    void OnProcessFinished_2(QProcess::ProcessState state);

    bool CloneFinishedWithOutput(const QString& Output);
    bool ResetFinishedWithOutput(const QString& Output);

private:
    QString URL;

    QProcess* ProcessPtr = nullptr;
    EGitToolState CurrentState = EGitToolState::E_NONE;
    bool bIsProcessFinished = true;

    int RepoID = -1;
};

typedef std::shared_ptr<GitRepoObject> GitRepoObjectPtr;


class GitRepoManager : public QObject
{
    Q_OBJECT
public:
    static GitRepoManager* Get();

    void CloneRepo(const QString& InRepoUrl, const QString& InLocalPath);
    int GetRunningRepoCount();

protected:
    void OnRepoToolFinished(int InRepoID, bool Success);


private:
    int RepoID = 0;

    std::map<int, GitRepoObjectPtr> Repos;

};

#endif // GITOPERATIONTOOL_H
