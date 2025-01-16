#include "updateplugindialogue.h"
#include "ui_updateplugindialogue.h"
#include "QMessageBox"
#include "QPainter"
#include "QMouseEvent"
#include "QDebug"


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
    delete ui;
    GitPathDelegatePtr.reset();
}

void UpdatePluginDialogue::Init()
{
    InitData();
    InitWidgets();

    BindCallback();
}

void UpdatePluginDialogue::BindCallback()
{
    connect(ui->BtnUpdate, &QPushButton::clicked, this, &UpdatePluginDialogue::OnUpdateBtnClicked);
    connect(ui->BtnClose, &QPushButton::clicked, this, &UpdatePluginDialogue::OnCloseBtnClicked);
    connect(ui->BtnAddGit, &QPushButton::clicked, this, &UpdatePluginDialogue::OnAddBtnClicked);
    connect(GitPathDelegatePtr.get(), &GitPathStyleDelegate::ButtonClicked, this, &UpdatePluginDialogue::OnDeleteListItemClicked);
}

void UpdatePluginDialogue::InitData()
{
    // TODO 从配置文件中读取已经配置了的 Git 仓库路径到 GitPathResources 属性中

    GitPathListModel.setStringList(GitPathResources);
    GitPathDelegatePtr = std::make_shared<GitPathStyleDelegate>(ui->GitPathListView);

}

void UpdatePluginDialogue::InitWidgets()
{
    ui->GitPathListView->setModel(&GitPathListModel);
    ui->GitPathListView->setItemDelegate(GitPathDelegatePtr.get());


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

    // TODO 通过正则检查链接是否正确

    return EErrorType::E_NONE;
}

void UpdatePluginDialogue::OnCloseBtnClicked()
{
    this->close();
}

void UpdatePluginDialogue::OnAddBtnClicked()
{
    QString InGitPath = ui->gitLineEdit->text();
    ui->gitLineEdit->clear();
    if(CheckGitPathValid(InGitPath) != EErrorType::E_NONE)
    {
        QMessageBox::information(nullptr, "添加路径失败", "请检查路径是否有效 ", QMessageBox::No, QMessageBox::No);
        return;
    }

    GitPathResources.append(InGitPath);
    GitPathListModel.setStringList(GitPathResources);
}

void UpdatePluginDialogue::OnUpdateBtnClicked()
{

}

void UpdatePluginDialogue::OnDeleteListItemClicked(const QModelIndex &index)
{
    qDebug() << "Clicked";
}
