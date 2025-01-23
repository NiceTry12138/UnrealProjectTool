#include "updatepluginprocessdialogue.h"
#include "ui_updatepluginprocessdialogue.h"

#include "QDir"
#include "QDebug"

#include "../../Util/gitoperationtool.h"

static const QString G_PLUGINDIR = "Plugins";

UpdatePluginProcessDialogue::UpdatePluginProcessDialogue(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdatePluginProcessDialogue)
{
    ui->setupUi(this);

    connect(&Timer, &QTimer::timeout, this, &UpdatePluginProcessDialogue::CheckProcess);
    Timer.start(100);
}

UpdatePluginProcessDialogue::~UpdatePluginProcessDialogue()
{
    delete ui;
}

void UpdatePluginProcessDialogue::StartWithGitPathResources(const QStringList &InGitPathResources)
{
    GitPathResources = InGitPathResources;
    ui->TotalProgress->setValue(0);
    Model.setStringList(ModelContent);
    ui->StateListView->setModel(&Model);

    ui->TxtDoing->setText("Create Git Process ...");
    for (const auto& RepoItem : GitPathResources) {
        CloneRepo(RepoItem);
        ModelContent << "create process -> " + GitRepoObject::GetRepoNameByPath(RepoItem);
        Model.setStringList(ModelContent);
    }
}

void UpdatePluginProcessDialogue::CloneRepo(const QString &URL)
{
    QString RepoName = GitRepoObject::GetRepoNameByPath(URL);

    QString ExePath = QCoreApplication::applicationDirPath();
    QDir BaseDir(ExePath);

    if(!BaseDir.exists(G_PLUGINDIR))
    {
        BaseDir.mkdir(G_PLUGINDIR);
    }

    BaseDir.cd(G_PLUGINDIR);
    GitRepoManager::Get()->CloneRepo(URL, BaseDir.filePath(RepoName));
}

void UpdatePluginProcessDialogue::CheckProcess()
{
    int RunningCount = GitRepoManager::Get()->GetRunningRepoCount();
    int TotalCount = GitPathResources.size();
    int Value = 100 - RunningCount * 100 / TotalCount;
    qDebug() << "RunningCount = " << RunningCount << " TotalCount = " << TotalCount << " Value = " << Value;

    ui->TotalProgress->setValue(Value);

    if(RunningCount == 0)
    {
        this->close();
    }
}

void UpdatePluginProcessDialogue::closeEvent(QCloseEvent *Event)
{
    // qDebug() << "Close UpdatePluginProcessDialogue";

    QDialog::closeEvent(Event);
}
