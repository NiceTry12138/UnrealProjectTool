#include "generateuproject.h"
#include "ui_generateuproject.h"

#include "QDir"
#include "QMessageBox"

#include "../../Util/unrealhelper.h"

static const QString G_BaseTitle = "Running Generate Project Files";

GenerateUProjectDialogue::GenerateUProjectDialogue(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GenerateUProject)
{
    ui->setupUi(this);

    ui->label->setText(G_BaseTitle);

    connect(&Timer, &QTimer::timeout, this, &GenerateUProjectDialogue::Update);
    Timer.start(200);
}

GenerateUProjectDialogue::~GenerateUProjectDialogue()
{
    delete ui;
}

void GenerateUProjectDialogue::Init()
{
    QString ExePath = QCoreApplication::applicationDirPath();
    QDir Dir(ExePath);
    QStringList Filters;
    Filters << "*.uproject";

    Dir.setNameFilters(Filters);

    QFileInfoList FileList = Dir.entryInfoList(QDir::Files);

    if(FileList.empty())
    {
        QMessageBox::information(nullptr, "No .UProject File", "Can't Find .UProject File In Current Directory", QMessageBox::No, QMessageBox::No);
        this->close();
        return;
    }

    QFileInfo FileInfo = FileList.at(0);
    TargetUProjectFileName = FileInfo.fileName();



    // UnrealHelper::Get()->GenerateProject("UE_5.3", "E:/QtProject/UnrealProjectTool/build/x86_windows_msvc2019_pe_64bit-Debug/Custom53.uproject");
    UnrealHelper::Get()->GenerateProject("UE_5.3", FileInfo.filePath());

    this->close();
}

void GenerateUProjectDialogue::Update()
{
    if(bIsFirstRunning)
    {
        bIsFirstRunning = false;
        Init();
    }

    QString Title = G_BaseTitle + " => " + TargetUProjectFileName;
    Count = (Count + 1) % 5;

    for (int var = 0; var < Count; ++var) {
        Title += ".";
    }

    ui->label->setText(Title);
}
