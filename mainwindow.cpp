#include "mainwindow.h"
#include "QDebug"
#include "./ui_mainwindow.h"
#include "./SubUI/UpdatePlugins/updateplugindialogue.h"

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
    // qDebug() << "更新插件";
    if(UpdatePluginDialoguePtr == nullptr)
    {
        UpdatePluginDialoguePtr = std::make_shared<UpdatePluginDialogue>(this);
    }

    UpdatePluginDialoguePtr->open();
}

void MainWindow::OnUpdatePluginDialogueClosed()
{
    // TODO 看情况 是否需要清理 UpdatePluginDialoguePtr 对象
}
