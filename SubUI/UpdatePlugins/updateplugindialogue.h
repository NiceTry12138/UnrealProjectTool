#ifndef UPDATEPLUGINDIALOGUE_H
#define UPDATEPLUGINDIALOGUE_H

#include <QDialog>
#include "QStringListModel"
#include "QStyledItemDelegate"
#include "../../CommonData.h"
#include "QListWidgetItem"

typedef std::shared_ptr<QListWidgetItem> QListWidgetItemPtr;

class GitPathStyleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    GitPathStyleDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

protected:
    QPoint GetButtonAhchorPoint(const QRect& Rect) const;

signals:
    void ButtonClicked(const QModelIndex &index);
};

namespace Ui {
class UpdatePluginDialogue;
}

class UpdatePluginDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatePluginDialogue(QWidget *parent = nullptr);
    ~UpdatePluginDialogue();

protected:
    void Init();
    void BindCallback();
    void InitData();
    void InitWidgets();

    EErrorType CheckGitPathValid(const QString& GitPath);

    void AddListWidgetItem(const QString& InInfo);
    void RemoveListWidgetItem(int ID, bool DelteID = true);

    void CloneRepo(const QString& URL);

protected:
    void OnCloseBtnClicked();
    void OnAddBtnClicked();
    void OnUpdateBtnClicked();

    void OnDeleteListItemClicked(const QString& Info, int OutID);
    void OnDetailListItemClicked(const QString& Info, int OutID);

    QString GetRepoNameByPath(const QString& InGitPath);
    void LogWithAddRepo(EErrorType InType);

    void UpdateConfig();

private:
    Ui::UpdatePluginDialogue *ui;

private:

private:
    int WidgetIndex = 0;

    // Datas
    QStringList GitPathResources;

    std::map<int, QListWidgetItemPtr> WidgetItemList;
};

#endif // UPDATEPLUGINDIALOGUE_H
