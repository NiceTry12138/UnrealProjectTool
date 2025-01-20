#include "mainwindow.h"
#include "QDebug"
#include "./ui_mainwindow.h"
#include "./SubUI/UpdatePlugins/updateplugindialogue.h"
#include "Util/unrealhelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
    UpdatePluginDialoguePtr.reset();
}

void MainWindow::Init()
{
    BindCallabck();
}

void MainWindow::BindCallabck()
{
    connect(ui->BtnUpdatePlugin, &QPushButton::clicked, this, &MainWindow::OnUpdatePluginBtnClicked);
}

void MainWindow::OnUpdatePluginBtnClicked()
{
    UnrealHelper::Get();

    // qDebug() << "更新插件";
    if(UpdatePluginDialoguePtr == nullptr)
    {
        UpdatePluginDialoguePtr = std::make_shared<UpdatePluginDialogue>(this);
        connect(UpdatePluginDialoguePtr.get(), &QDialog::finished, this, &MainWindow::OnUpdatePluginDialogueClosed);
    }

    UpdatePluginDialoguePtr->open();
}

void MainWindow::OnUpdatePluginDialogueClosed()
{
    UpdatePluginDialoguePtr.reset();
}
