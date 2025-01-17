#include "gitpathlistwidgetitem.h"
#include "ui_gitpathlistwidgetitem.h"

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

void GitPathListWidgetItem::UpdateInfo(const QString &InInfo)
{
    CacheInfo = InInfo;
    ui->TxtGitPath->setText(InInfo);
}

void GitPathListWidgetItem::OnDeleteButtonClicked()
{
    OnDelete(CacheInfo);
}

void GitPathListWidgetItem::OnDetailButtonClicked()
{
    OnDetail(CacheInfo);
}