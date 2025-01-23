#include "updateplugindialogue.h"
#include "ui_updateplugindialogue.h"
#include "QMessageBox"
#include "QPainter"
#include "QMouseEvent"
#include "QDebug"
#include "QRegularExpression"
#include "QRegularExpressionMatch"
#include "QListWidget"

#include "gitpathlistwidgetitem.h"
#include "../../Util/applicationsettings.h"
#include "../../Util/gitoperationtool.h"
#include "../Common/generateuproject.h"
#include "updatepluginprocessdialogue.h"

QWidget *GitPathStyleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return nullptr; // 返回 nullptr 表示不可编辑
}

void GitPathStyleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    // 创建按钮并绘制在项内
    QPushButton button;
    button.setText("删除");
    button.resize(QSize(option.rect.width() / 4, option.rect.height() / 2));

    QPoint AnchorPoint = GetButtonAhchorPoint(option.rect);

    QPixmap pixmap(button.size());
    button.render(&pixmap);
    painter->drawPixmap(AnchorPoint, pixmap);
}

QSize GitPathStyleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 限制高度大小为 40
    return QSize(option.rect.width(), 40);
}

bool GitPathStyleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() != QEvent::MouseButtonPress) {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    // 计算按钮的矩形区域
    QPoint AnchorPoint = GetButtonAhchorPoint(option.rect);
    QRect buttonRect = option.rect;
    buttonRect.setSize(QSize(option.rect.width() / 4, option.rect.height() / 2)); // 按钮宽度为项宽度的一半
    buttonRect.moveTo(AnchorPoint);

    QPoint MousePoint = mouseEvent->pos();
    if (buttonRect.contains(MousePoint)) {
        emit ButtonClicked(index);
        return true;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QPoint GitPathStyleDelegate::GetButtonAhchorPoint(const QRect &Rect) const
{
    QPoint Result = Rect.center();
    Result.setX(Result.x() + Rect.width() / 4);
    Result.setY(Result.y() - Rect.height() / 4);
    return Result;
}

UpdatePluginDialogue::UpdatePluginDialogue(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdatePluginDialogue)
{
    ui->setupUi(this);

    Init();
}

UpdatePluginDialogue::~UpdatePluginDialogue()
{
    for(auto Item = WidgetItemList.rbegin(); Item != WidgetItemList.rend(); ++Item)
    {
        RemoveListWidgetItem(Item->first, false);
    }
    WidgetItemList.clear();
    delete ui;
}

void UpdatePluginDialogue::Init()
{
    InitData();
    InitWidgets();

    BindCallback();

    connect(&Timer, &QTimer::timeout, this, &UpdatePluginDialogue::DelayCheck);
    Timer.start(1000);
}

void UpdatePluginDialogue::BindCallback()
{
    connect(ui->BtnUpdate, &QPushButton::clicked, this, &UpdatePluginDialogue::OnUpdateBtnClicked);
    connect(ui->BtnClose, &QPushButton::clicked, this, &UpdatePluginDialogue::OnCloseBtnClicked);
    connect(ui->BtnAddGit, &QPushButton::clicked, this, &UpdatePluginDialogue::OnAddBtnClicked);
}

void UpdatePluginDialogue::InitData()
{
    GitPathResources = ApplicationSettings::Get()->GetAddedPluginResources();

}

void UpdatePluginDialogue::InitWidgets()
{
    for(const auto& SavedItem : GitPathResources)
    {
        AddListWidgetItem(SavedItem);
    }
    ui->GitPathListWidget->setUpdatesEnabled(true);
}

EErrorType UpdatePluginDialogue::CheckGitPathValid(const QString &GitPath)
{
    if(GitPath.isEmpty())
    {
        return EErrorType::E_ERRORCONTENT;
    }

    for (const auto& PathItem : GitPathResources) {
        if(PathItem == GitPath)
        {
            return EErrorType::E_SAMETYPE;
        }
    }

    bool bStartCheck = GitPath.startsWith("https://") || GitPath.startsWith("git@");
    bool bEndCheck = GitPath.endsWith(".git");

    if(!(bStartCheck && bEndCheck))
    {
        return EErrorType::E_ERRORCONTENT;
    }

    QString NewRepoName = GitRepoObject::GetRepoNameByPath(GitPath);

    for(const auto& AddedRepo : GitPathResources)
    {
        QString AddedRepoName = GitRepoObject::GetRepoNameByPath(AddedRepo);
        if(AddedRepoName == NewRepoName)
        {
            return EErrorType::E_SAMETYPE;
        }
    }

    return EErrorType::E_NONE;
}

void UpdatePluginDialogue::AddListWidgetItem(const QString& InInfo)
{
    ++WidgetIndex;

    // 创建自定义窗口
    auto ItemWidget = new GitPathListWidgetItem(ui->GitPathListWidget);
    ItemWidget->UpdateInfo(InInfo, WidgetIndex);
    connect(ItemWidget, &GitPathListWidgetItem::OnDelete, this, &UpdatePluginDialogue::OnDeleteListItemClicked);
    connect(ItemWidget, &GitPathListWidgetItem::OnDetail, this, &UpdatePluginDialogue::OnDetailListItemClicked);

    // 创建 Item
    QListWidgetItemPtr ListItem = std::make_shared<QListWidgetItem>(ui->GitPathListWidget);
    ListItem->setSizeHint(ItemWidget->sizeHint());

    // 存储引用
    WidgetItemList[WidgetIndex] = ListItem;

    // 添加并刷新
    ui->GitPathListWidget->addItem(ListItem.get());
    ui->GitPathListWidget->setItemWidget(ListItem.get(), ItemWidget);
    ItemWidget->show();

    ui->GitPathListWidget->update();
    // ui->GitPathListWidget->repaint();
}

void UpdatePluginDialogue::RemoveListWidgetItem(int ID, bool DeleteID)
{
    if(WidgetItemList.find(ID) == WidgetItemList.end())
    {
        // 未找到 不做处理
        return;
    }

    // 获取指定行的 QListWidgetItem
    auto Item = WidgetItemList[ID];
    if (!Item)
    {
        // 如果行不存在，直接返回
        return;
    }


    // 获取关联的小部件
    QWidget* widget = ui->GitPathListWidget->itemWidget(Item.get());
    ui->GitPathListWidget->removeItemWidget(Item.get());
    if (widget)
    {
        delete widget;  // 删除小部件
    }

    // 删除 QListWidgetItem
    Item.reset();
    if(DeleteID)
    {
        WidgetItemList.erase(ID);
    }
    ui->GitPathListWidget->setUpdatesEnabled(true);
    ui->GitPathListWidget->update();
    // ui->GitPathListWidget->repaint();
}

void UpdatePluginDialogue::DelayCheck()
{
    Timer.stop();

    bool CheckCMD = GitRepoObject::CheckCommandGit();
    if(!CheckCMD)
    {
        QMessageBox::information(nullptr, "Can not Find Git Command", " Please Install Git Or Check Git Command Line ", QMessageBox::No, QMessageBox::No);
        this->close();
        return;
    }
}

void UpdatePluginDialogue::OnCloseBtnClicked()
{
    this->close();
}

void UpdatePluginDialogue::OnAddBtnClicked()
{
    QString InGitPath = ui->gitLineEdit->text();
    ui->gitLineEdit->clear();
    auto AddedErrorType = CheckGitPathValid(InGitPath);
    if(AddedErrorType != EErrorType::E_NONE)
    {
        LogWithAddRepo(AddedErrorType);
        return;
    }

    GitPathResources.append(InGitPath);

    UpdateConfig();
    AddListWidgetItem(InGitPath);
}

void UpdatePluginDialogue::OnUpdateBtnClicked()
{
    QString Msg = "如果本地存在仓库,会 Reset 该仓库，复原所有修改并删除新增文件，请确定";
    auto Result = QMessageBox::information(nullptr, "Clone Or Reset Plugins", Msg, QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
    if(Result == QMessageBox::No)
    {
        return;
    }

    UpdatePluginProcessDialogue ProcessDialogue(this);
    ProcessDialogue.StartWithGitPathResources(GitPathResources);
    ProcessDialogue.exec();

    GenerateUProjectDialogue GenerateDialogue(this);
    GenerateDialogue.exec();
}

void UpdatePluginDialogue::OnDeleteListItemClicked(const QString &Info, int OutID)
{
    GitPathResources.removeAll(Info);
    RemoveListWidgetItem(OutID);
    UpdateConfig();
}

void UpdatePluginDialogue::OnDetailListItemClicked(const QString &Info, int OutID)
{

}

void UpdatePluginDialogue::LogWithAddRepo(EErrorType InType)
{
    switch (InType) {
    case EErrorType::E_SAMETYPE:
        QMessageBox::information(nullptr, "添加路径失败", "已经存在同名仓库 ", QMessageBox::No, QMessageBox::No);
        break;
    case EErrorType::E_ERRORCONTENT:
        QMessageBox::information(nullptr, "添加路径失败", "请检查路径是否有效 ", QMessageBox::No, QMessageBox::No);
        break;
    case EErrorType::E_NOEXISTS:
        QMessageBox::information(nullptr, "添加路径失败", "请检查路径是否有效 ", QMessageBox::No, QMessageBox::No);
        break;
    default:
        break;
    }
}

void UpdatePluginDialogue::UpdateConfig()
{
    QStringList NewList;
    for(const auto& AddedRepo : GitPathResources)
    {
        NewList.append(AddedRepo);
    }

    ApplicationSettings::Get()->SetAddedPluginResources(NewList);
}
