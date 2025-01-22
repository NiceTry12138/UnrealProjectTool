#include "gitpathlistwidgetitem.h"
#include "ui_gitpathlistwidgetitem.h"

#include "QMessageBox"

GitPathListWidgetItem::GitPathListWidgetItem(QWidget *parent)
    : QWidget(parent), ui(new Ui::GitPathListWidgetItem)
{
    ui->setupUi(this);

    connect(ui->BtnDelete, &QPushButton::clicked, this, &GitPathListWidgetItem::OnDeleteButtonClicked);
    connect(ui->BtnDetail, &QPushButton::clicked, this, &GitPathListWidgetItem::OnDetailButtonClicked);
}

GitPathListWidgetItem::~GitPathListWidgetItem()
{
    delete ui;
}

void GitPathListWidgetItem::UpdateInfo(const QString &InInfo, int InID)
{
    CacheInfo = InInfo;
    ui->TxtGitPath->setText(InInfo);
    ID = InID;
}

void GitPathListWidgetItem::OnDeleteButtonClicked()
{
    OnDelete(CacheInfo, ID);
}

void GitPathListWidgetItem::OnDetailButtonClicked()
{
    QMessageBox::information(nullptr, " 没功能 ", " 功能未实现 ", QMessageBox::No, QMessageBox::No);
    OnDetail(CacheInfo, ID);
}
